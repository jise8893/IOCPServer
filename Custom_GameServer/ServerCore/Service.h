
#include <set>
#include "NetAddress.h"


class Service : public enable_shared_from_this<Service> {
public:
	USE_LOCK;
	Service( IocpCoreRef iocpcore);
	void				AddSession(SessionRef session);
	void				ReleaseSession(SessionRef session);
	NetAddress			GetNetAddress(){ return netaddr; };
	bool				Start();
	IocpCoreRef&		GetIocpCore() { return iocpcore; }
	SessionRef			CreateSession();
protected:
	set<SessionRef> sessions;
	NetAddress netaddr;
	ListenerRef _listener;
	IocpCoreRef iocpcore;
	
};