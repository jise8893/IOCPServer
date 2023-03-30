#include "pch.h"
#include "Service.h"
#include "Listener.h"
#include "Session.h"
#include "SendBuffer.h"

Service::Service( IocpCoreRef iocpCore,  NetAddress netaddress,SessionFactory factory): netaddr(netaddress),iocpcore(iocpCore),_factory(factory)
{

}


void Service::AddSession(SessionRef session)
{
	WRITE_LOCK;
	sessions.insert(session);
}

void Service::ReleaseSession(SessionRef session)
{
	WRITE_LOCK;
	sessions.erase(session);
}

bool Service::Start()
{
	_listener = make_shared<Listener>(NetAddress(L"127.0.0.1",7777));
	if (_listener == nullptr)
		return false;


	if (_listener->StartAccept(shared_from_this())==false)
		return false;

	return false;
}

bool Service::ClientStarts()
{
	const int sessionCnt = 4;
	for (int32 i = 0; i < sessionCnt; i++)
	{
		SessionRef session = CreateSession();
		if (false == session->Connect())
			return false;

	}
	return true;
}


SessionRef Service::CreateSession()
{
	
	shared_ptr<Session> session = _factory();

	session->SetService(shared_from_this());
	iocpcore->Register(session);
	return session;
}

void Service::BroadCast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (SessionRef session : sessions)
	{
		session->Send(sendBuffer);
	}
}
