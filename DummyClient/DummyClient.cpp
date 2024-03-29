﻿#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include <set>
#include "Service.h"
#include "Listener.h"
#include "Session.h"
#include "SendBuffer.h"
#include "ClientPacektHandler.h"
#include "ServerPacketHandler.h"
char sendDataC[] = "hello server";
class ServerSession :public PacketSession
{
	virtual void OnConnected() override {
		//cout << "Connected To Server" << endl;
		
	}
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override
	{
		ClientPacektHandler::HandlerPacket(buffer, len);
		return len;
	}
	virtual void OnSend(int32 len) override
	{
		cout << "OnSend len= " << len << endl;
	}
};

auto factory = []() {return make_shared<ServerSession>(); };
int main()
{
	this_thread::sleep_for(1s);
	ServiceRef clientService = make_shared<Service>(make_shared<IocpCore>(), NetAddress(L"127.0.0.1", 7777), factory);
	ASSERT_CRASH(clientService->ClientStarts())
	
	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					clientService->GetIocpCore()->Dispatch();
				}
			});
	}
	int a, b;
	while (true)
	{
		a = b = 0;
		scanf_s("%d %d", &a, &b);
		SendBufferRef sendBuffer = ServerPacketHandler::MakeMoveTo(a, b);
		clientService->BroadCast(sendBuffer);
	}
	GThreadManager->Join();

}