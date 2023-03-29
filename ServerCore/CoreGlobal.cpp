#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Service.h"
#include "SocketUtils.h"
ThreadManager* GThreadManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();

		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		SocketUtils::Clear();
	}
} GCoreGlobal;

