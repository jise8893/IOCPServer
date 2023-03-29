#pragma once
#include "IocpCore.h"
#include "NetAddress.h"
#include "IocpEvent.h"
#include <functional>
#include "RecvBuffer.h"
#include <queue>

class Session :public IocpObject
{
	enum {
		BUFFER_SIZE = 0X1000,
	};
public:
	friend class Listener;
	friend class Service;
	Session();
	virtual ~Session();
	SOCKET				GetSocket() { return _socket; }
	NetAddress			GetAddress() { return _netAddress; }
	bool				IsConnected() { return _connected; }

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;
	void ProcessConnect();
	void SetNetAddress(NetAddress address);
	void RegisterRecv();
	bool RegisterConnect();
	


	void Send(SendBufferRef sendBuffer);
	void				DisConnect(const WCHAR* cause);
	bool				RegisterDisconnect();
	void				RegisterSend();
	bool				Connect();

	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);
	void				SetService(shared_ptr<Service> service) { _service = service; }
	shared_ptr<Service>				GetService() { return _service.lock(); }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
protected:
	virtual void OnConnected(){}
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) {}
	virtual void OnDisConnected(){}
	
private:
	
	RecvBuffer			_recvBuffer;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;
	queue<SendBufferRef> _sendQueue;
	Atomic<bool>		_sendRegistered=false;
private:
	USE_LOCK;
	/* IocpEvent Àç»ç¿ë */
	weak_ptr<Service>	_service;
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
	SendEvent			_sendEvent;
	
};

