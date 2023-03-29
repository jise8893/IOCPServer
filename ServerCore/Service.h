
#include <set>
#include "NetAddress.h"
#include <functional>

using SessionFactory = function<SessionRef(void)>;
class Service : public enable_shared_from_this<Service> {
public:
	USE_LOCK;
	Service( IocpCoreRef iocpCore,NetAddress netaddress,SessionFactory factory);
	
	void				AddSession(SessionRef session);
	void				ReleaseSession(SessionRef session);
	NetAddress			GetNetAddress(){ return netaddr; };
	bool				Start();
	bool				ClientStarts();
	IocpCoreRef&		GetIocpCore() { return iocpcore; }
	SessionRef			CreateSession();
	
protected:
	set<SessionRef> sessions;
	NetAddress netaddr;
	ListenerRef _listener;
	IocpCoreRef iocpcore;
	SessionFactory _factory;
};