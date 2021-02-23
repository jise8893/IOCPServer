
#pragma once
#include "IOCPServer.h"

class Listener {
    
    WSADATA wsaData;
    HANDLE hComPort;
    SYSTEM_INFO sysinfo;
    SocketInfo* ioInfo;
    BufferInfo* handleInfo;
    SOCKET hServSock;
    SOCKADDR_IN servAdr;
    int recvBytes, flags = 0;
    int i = 0;
public:
    void Init();
    void RegisterAccept();


};