#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>

#include <stdlib.h>

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")



#define BUFSIZE 1024

void ErrorHandling(char* msg);



int main(int argc, char* argv[])

{

	WSADATA wsaData;

	SOCKET hSocket;

	SOCKADDR_IN servAddr;

	char msg[BUFSIZE];

	int strLen, readLen;



	

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)

	{

		

	}



	hSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)

	{


	}



	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;

	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	servAddr.sin_port = htons(8000);



	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)

	{

	}else
	{

		puts("connected...");

	}



	while (1)

	{

		fputs("Input Message(Q to quit): ", stdout);

		fgets(msg, BUFSIZE, stdin);

		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))

			break;



		strLen = strlen(msg);

		readLen = 0;

		while (1)

		{
			send(hSocket, msg, strLen, 0);
			readLen += recv(hSocket, &msg[readLen], BUFSIZE - 1, 0);

			if (readLen >= strLen)

				break;

		}

		msg[strLen] = 0;

		printf("Message from server: %s", msg);

	}



	closesocket(hSocket);

	WSACleanup();

	return 0;

}



void ErrorHandling(char* msg)

{

	fputs(msg, stderr);

	fputc('\n', stderr);

	exit(1);

}
