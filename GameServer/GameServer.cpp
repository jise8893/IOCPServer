#include "pch.h"
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
char sendData[] = "hello world";
class GameSession :public Session {
public:
	virtual void OnConnected() override{
		cout << "Connected To Server" << endl;
		SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
		sendBuffer->CopyData(sendData, sizeof(sendData));
		Send(sendBuffer);	
	}
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len= " << len << endl;
		SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
		sendBuffer->CopyData(buffer,len);
		Send(sendBuffer);
		return len;
	}
	virtual void OnSend(int32 len) override
	{
		cout << "OnSend len= " << len << endl;
	}
	
};
auto factory = []() {return make_shared<GameSession>(); };
int main()
{

	ServiceRef service = make_shared<Service>(make_shared<IocpCore>(), NetAddress(L"127.0.0.1", 7777),factory);
	service->Start();
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}
	GThreadManager->Join();
}