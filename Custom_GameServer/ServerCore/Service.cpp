#include "pch.h"
#include "Service.h"
#include "Listener.h"
#include "Session.h"


Service::Service( IocpCoreRef iocpcore) : iocpcore(iocpcore)
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

SessionRef Service::CreateSession()
{
	SessionRef session = make_shared<Session>();
	session->SetService(shared_from_this());
	iocpcore->Register(session);
	return session;
}
