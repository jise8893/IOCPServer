#pragma once
#include "IocpCore.h"
#include "NetAddress.h"
#include <vector>
class AcceptEvent;
class Listener :public IocpObject
{
public:
	Listener(NetAddress netaddr);
	~Listener();

public:
	/* �ܺο��� ��� */
	bool StartAccept(ServiceRef service );
	void CloseSocket();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;
private:
	
private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
	NetAddress _netaddr;
	ServiceRef _service;
};

