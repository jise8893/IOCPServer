#include "IOCPServer.h"

DWORD __stdcall EchoThreadMain(LPVOID CompltetionPortIO)
{
    HANDLE hComport = (HANDLE)CompltetionPortIO;
    SOCKET socket;
    DWORD flags = 0;
    SocketInfo *ioInfo;
    BufferInfo *handleInfo;
    DWORD bytesTrans;

    while (1) {
        if (!GetQueuedCompletionStatus(hComport, &bytesTrans, (PULONG_PTR)&ioInfo, (LPOVERLAPPED*)&handleInfo, INFINITE) && bytesTrans == 0)
        {
            printf_s("[INFO]  Socket() Á¢¼Ó ²÷¾îÁü\n");
            free(handleInfo);
            continue;
        }
        socket = ioInfo->hClntSock;
        if (handleInfo->rwMode == READ)
        {
            puts("message received!");
            if (bytesTrans == 0)
            {
                closesocket(socket);
                free(handleInfo);
                free(ioInfo);
                continue;
            }
            memset(&(handleInfo->overlapped), 0, sizeof(OVERLAPPED));
            handleInfo->wsabuf.len = bytesTrans;
            handleInfo->rwMode = WRITE;
            int nResult=WSASend(socket, &(handleInfo->wsabuf), 1, NULL, 0, &(handleInfo->overlapped), NULL);
            if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
                printf_s("[ERROR]  WSASEND ½ÇÆÐ\n");
            }
            printf_s("[INFO]  WSASEND ¼º°ø\n");
            handleInfo = new BufferInfo();
            memset(&(handleInfo->overlapped), 0, sizeof(OVERLAPPED));
            handleInfo->wsabuf.len = BUF_SIZE;
            handleInfo->wsabuf.buf = handleInfo->buffer;
            handleInfo->rwMode = READ;
            WSARecv(socket, &(handleInfo->wsabuf), 1, NULL, &flags, &(handleInfo->overlapped), NULL);

        }
        else {
            puts("message sned!");
            free(handleInfo);
        }
    }
    return 0;
}

void ErrorHandling(const char* message)
{
    
}
