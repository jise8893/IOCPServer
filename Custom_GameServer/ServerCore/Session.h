#pragma once
#include "IocpCore.h"
#include "NetAddress.h"
#include "IocpEvent.h"
class Session :public IocpObject
{
	friend class Listener;
public:
	Session();
	~Session();
	SOCKET				GetSocket() { return _socket; }
	NetAddress			GetAddress() { return _netAddress; }
	bool				IsConnected() { return _connected; }
	
public :
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;
	void ProcessConnect();
	void SetNetAddress(NetAddress address);
	void RegisterRecv();
	


	
	bool				RegisterDisconnect();
	void				RegisterSend();

	void				ProcessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);
	void				SetService(shared_ptr<Service> service) { _service = service; }
	shared_ptr<Service>				GetService() { return _service.lock(); }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
private:
	char recvbuffer[1000];
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;
	

private:
	USE_LOCK;
	/* IocpEvent Àç»ç¿ë */
	weak_ptr<Service>	_service;
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;
	SendEvent			_sendEvent;
};

