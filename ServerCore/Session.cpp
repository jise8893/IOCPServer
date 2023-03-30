#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"
#include "Session.h"
#include "SendBuffer.h"

Session::Session() :_recvBuffer(BUFFER_SIZE)
{

	_socket = SocketUtils::CreateSocket();

}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr; // RELEASE_REF

	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 재정의
	OnConnected();

	// 수신 등록
	RegisterRecv();
}

void Session::SetNetAddress(NetAddress address)
{
	_netAddress = address;
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this(); // ADD_REF

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			DisConnect(L"Recv Error");
			_recvEvent.owner = nullptr; // RELEASE_REF
			
		}
	}
}

bool Session::RegisterConnect()
{
	if (IsConnected())
		return false;

	
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::BindAnyAddress(_socket, 0/*남는거*/) == false)
		return false;

	_connectEvent.Init();
	_connectEvent.owner = shared_from_this(); // ADD_REF

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_connectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}





void Session::Send(SendBufferRef sendBuffer)
{
	//register send가 걸리지 않았다면 걸어준다.

	WRITE_LOCK;
	_sendQueue.push(sendBuffer);
	
	if (_sendRegistered.exchange(true) == false)
		RegisterSend();
}

void Session::DisConnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout  <<"DisConnected: " << cause << endl;


	

	RegisterDisconnect();
}

bool Session::RegisterDisconnect()
{
	_disconnectEvent.Init();
	_disconnectEvent.owner = shared_from_this(); // ADD_REF

	if (false == SocketUtils::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_disconnectEvent.owner = nullptr; // RELEASE_REF
			return false;
		}
	}

	return true;
}

void Session::RegisterSend()
{
	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();
	{
		WRITE_LOCK;
		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();
			writeSize += sendBuffer->WritePos();
			//예외 체크
			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}

	}
	//	Scatter-Gather (흩어져 있는 데이터들을 모아서 한 방에 보낸다
	vector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.sendBuffers.size());
	for (SendBufferRef sendBuffer : _sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<LONG>(sendBuffer->WritePos());
		wsaBufs.push_back(wsaBuf);
	}
	
	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), OUT & numOfBytes, 0, &_sendEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			DisConnect(L"Send Error");
			_sendEvent.owner = nullptr; // RELEASE_REF
			_sendEvent.sendBuffers.clear();
			_sendRegistered.store(false);

		}
	}
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::ProcessDisconnect()
{
	_disconnectEvent.owner = nullptr; // RELEASE_REF
	OnDisConnected(); // 컨텐츠 코드에서 재정의
	GetService()->ReleaseSession(GetSessionRef());//registerdisconnect 에 두면 cycle이 생겨서 for문을 돌면서 session을 erase하는 오류가 생길 수 있음.
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr;
	if (numOfBytes == 0)
	{
		DisConnect(L"Recv len 0");
		return;
		
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		DisConnect(L"OnWrite Overflow");
		return;
	}

	int32 processLen=OnRecv(_recvBuffer.ReadPos(), numOfBytes);
	if (processLen < 0 || _recvBuffer.DataSize() <processLen||_recvBuffer.OnRead(processLen)==false)
	{
		DisConnect(L"Onread Overflow");
		return;
	}
	_recvBuffer.Clean();
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{

	_sendEvent.owner = nullptr; // RELEASE_RE
	_sendEvent.sendBuffers.clear();
	if (numOfBytes == 0)
	{
		DisConnect(L"Send len 0");
		return;
	}

	// 컨텐츠 코드에서 재정의
	OnSend(numOfBytes);
	WRITE_LOCK;
	if (_sendQueue.empty())
		_sendRegistered.store(false);
	else
		RegisterSend();
}
