#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"
Session::Session()
{
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		ASSERT_CRASH(0);
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
		//ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		//ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr; // RELEASE_REF

	_connected.store(true);

	GetService()->AddSession(GetSessionRef());
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
	wsaBuf.buf = reinterpret_cast<char*>(recvbuffer);
	wsaBuf.len = len32(recvbuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{

			_recvEvent.owner = nullptr; // RELEASE_REF
		}
	}
}



bool Session::RegisterDisconnect()
{
	return false;
}

void Session::RegisterSend()
{
}

void Session::ProcessDisconnect()
{
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr;
	if (numOfBytes == 0)
	{
		return;
	}

	cout << "Recv Len= " << numOfBytes << endl;
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
}
