#include "IOCPServer.h"
#include "Listener.h"
extern std::mutex ul;
extern std::vector<SOCKET> userlist;

DWORD __stdcall EchoThreadMain(LPVOID CompltetionPortIO)
{
    HANDLE hComport = (HANDLE)CompltetionPortIO;
    SOCKET socket;
    DWORD flags = 0;
    SocketInfo *ioInfo;
    BufferInfo *handleInfo;
    DWORD bytesTrans;
    char msg[BUF_SIZE];
    while (1) {
        if (!GetQueuedCompletionStatus(hComport, &bytesTrans, (PULONG_PTR)&ioInfo, (LPOVERLAPPED*)&handleInfo, INFINITE) && bytesTrans == 0)
        {
            ul.lock();
            auto itr = find(userlist.begin(), userlist.end(), ioInfo->hClntSock);
            userlist.erase(itr);
            ul.unlock();
            printf_s("[INFO]  Socket() Failed\n");
            delete handleInfo;
            continue;
        }
        socket = ioInfo->hClntSock;
        if (handleInfo->rwMode == READ)
        {
            puts("message received!");
            if (bytesTrans == 0)
            {
                closesocket(socket);
  
                continue;
            }
            
            handleInfo->wsabuf.len = bytesTrans;
            handleInfo->rwMode = WRITE;
            int nResult = 0;
            memset(&(handleInfo->overlapped), 0, sizeof(OVERLAPPED));
            memcpy(msg, handleInfo->wsabuf.buf, BUF_SIZE);
            msg[bytesTrans] = '\0';
            printf("buffer:%s\n",msg);
            
            printf_s("[INFO]  WSASEND success\n");
            
            ul.lock();
            
            for (auto itr = userlist.begin(); itr != userlist.end(); itr++) {
                 handleInfo = new BufferInfo();
                 memset(&(handleInfo->overlapped), 0, sizeof(OVERLAPPED));
                 int len = strlen(msg);
                 handleInfo->wsabuf.len = len+10;
                 sprintf_s(handleInfo->buffer,1024,"%d:%s", socket,msg);
                 
                 handleInfo->wsabuf.buf = handleInfo->buffer;
                 handleInfo->rwMode = WRITE;

                 nResult = WSASend(*itr, &(handleInfo->wsabuf), 1, NULL, 0, &(handleInfo->overlapped), NULL);
                 if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                     printf_s("[ERROR]  WSASEND ����\n");
                 }
                 
            }
            ul.unlock();
        
            handleInfo = new BufferInfo();
            memset(&(handleInfo->overlapped), 0, sizeof(OVERLAPPED));
            handleInfo->wsabuf.len = BUF_SIZE;
            handleInfo->wsabuf.buf = handleInfo->buffer;
            handleInfo->rwMode = READ;
            WSARecv(socket, &(handleInfo->wsabuf), 1, NULL, &flags, &(handleInfo->overlapped), NULL);

        }
        else {
            puts("message send!");
            delete handleInfo;
        }
    }
    return 0;
}

void ErrorHandling(const char* message)
{
    
}
