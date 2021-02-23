#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#include <mutex>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 1024
#define READ 3
#define WRITE 5

DWORD WINAPI EchoThreadMain(LPVOID CompltetionPortIO);
void ErrorHandling(const char* message);


class SocketInfo {
public:
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
};

class BufferInfo {
public:
	OVERLAPPED overlapped;
	WSABUF wsabuf;
	char buffer[BUF_SIZE];
	int rwMode; //read or write

};