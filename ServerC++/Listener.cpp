#include "Listener.h"
#define SERVER_PORT 8000

void Listener::Init()
{
	int nResult;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() Error!");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysinfo);
	
	for (i = 0; i < sysinfo.dwNumberOfProcessors; i++) {
		_beginthreadex(NULL, 0,(_beginthreadex_proc_type)EchoThreadMain,(LPVOID)hComPort, 0, NULL);
	}

	hServSock = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hServSock == INVALID_SOCKET) {
		printf_s("[ERROR] 소켓 생성 실패");
		return;
	}
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htons(INADDR_ANY);
	servAdr.sin_port = htons(SERVER_PORT);

	nResult=bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	if (nResult == SOCKET_ERROR) {
		printf_s("[ERROR] Bind 실패");
		return;
	}
	nResult=listen(hServSock, 10);
	if (nResult == SOCKET_ERROR)
	{
		printf_s("[ERROR] listen 실패");
		closesocket(hServSock);
		WSACleanup();
	}


	RegisterAccept();
	
}

void Listener::RegisterAccept()
{
	while (true) {
		int nResult = 0;
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = WSAAccept(hServSock, (SOCKADDR*)&clntAdr, &addrLen, NULL, NULL);
		if (hClntSock == INVALID_SOCKET) {
			printf_s("[ERROR] Accept 실패");
			return;
		}
		ioInfo = new SocketInfo();
		ioInfo->hClntSock = hClntSock;
		memcpy(&(ioInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (ULONG_PTR)ioInfo, 0);
		handleInfo = new BufferInfo();
		memset(&(handleInfo->overlapped), 0, sizeof(BufferInfo));
		handleInfo->wsabuf.len = BUF_SIZE;
		handleInfo->rwMode = READ;
		handleInfo->wsabuf.buf = handleInfo->buffer;
		nResult = WSARecv(ioInfo->hClntSock, &(handleInfo->wsabuf), 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &(handleInfo->overlapped), NULL);
		if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
			printf_s("[ERROR] IO Pending 실패");
			printf_s("[ERROR] IO Pending 실패 :%d", WSAGetLastError());
			return;
		}
	}
}

