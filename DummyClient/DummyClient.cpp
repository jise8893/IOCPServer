#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>

#include <stdlib.h>

#include <winsock2.h>
#include <conio.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")



#define BUFSIZE 1024
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4
void ErrorHandling(char* msg);
int KeyControl();

void init() {
	system("mode con cols=56 lines=20 | title GameStart");
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCursor);

}

void gotoxy(int x, int y) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);

}
char map[14][56] = {
	{"1111111111111111111111111111111111111111111111111111111"},
	{"1000000000000000001000000000000000000000000000000000001"},
	{"1000000000000000001000000000000000000000000000000000001"},
	{"100000000000000000100000000000000000000000000000000q001"},
	{"1000000000000000001000000000000000000000000000000000001"},
	{"1000000010000000001000000000000000000000000000000000001"},
	{"1000000010000000001000000000000000000000000000000000001"},
	{"1000000010000000001000000000000111111111111111111111111"},
	{"1000000010000000001000000000000000000000000000000000001"},
	{"1000000010000000000000000000000000000000000000000000001"},
	{"1000000010000000000000000000000000000000000000000000001"},
	{"1000s00010000000000000000000000000000000000000000000001"},
	{"1000000010000000000000000000000000000000000000000000001"},
	{"1111111111111111111111111111111111111111111111111111111"}
	
};
int menuDraw()
{
	int x = 24;
	int y = 12;
	gotoxy(22, 12);
	printf("> 게 임 시 작");
	gotoxy(24, 13);



	printf("게 임 정 보");
	while (true) {
		int n = KeyControl();
		switch (n) {
		case UP: {
			if (y > 12)
			{
				gotoxy(x - 2, y);
				printf(" ");
				gotoxy(x - 2, --y);
				printf(">");
			}
			break;
		}
		case DOWN: {
			if (y < 13) {
				gotoxy(x - 2, 12);
				printf(" ");
				gotoxy(x - 2, ++y);
				printf(">");
			}break;
		}
		case SUBMIT: {
			return (y - 12);
		}
		}
	}
}
void DrawMap() {
	system("cls");
	int h, w;
	for (h = 0; h < 14; h++) {
		for (w = 0; w < 56; w++) {
			char temp = map[h][w];
			if (temp == '0')
				printf(" ");
			else if (temp == '1')
				printf("#");
			else if (temp == 's')
				printf("O");
			else if (temp == 'q')
				printf("@");
		}
		printf("\n");
	}
}
DWORD __stdcall RecvFunc(SOCKET Clntsock) {

	int strLen = 0;
	char msg[BUFSIZE];

	while (true) {


		strLen += recv(Clntsock, msg, BUFSIZE - 1, 0);
		

		msg[strLen] = 0;

		printf("Message from server: %s\n", msg);
		memset(msg, 0, BUFSIZE);
	}
}
void ConnectGame() {

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

	}
	else
	{

		puts("connected...");

	}

	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)RecvFunc,(void *)hSocket, 0, NULL);

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
			

				break;

		}
		

	}



	closesocket(hSocket);

	WSACleanup();

	return;
}
void titleDraw() {
	printf("\n\n\n\n");
	printf("		######  ##    ##    ##  \n");
	printf("		##            ##    ##  \n");
	printf("		######  ##    ##    ##  \n");
	printf("		##      ##    ##    ##  \n");
	printf("		##      ##    ##    ##  \n\n");

	printf("				  made my jise8893\n\n\n");
	
	


}
void infoDraw() {

	printf("\n\n\n	[게 임 방 법]\n");
	printf("	[W] 위로 \n");
	printf("	[A] 왼쪽 \n");
	printf("	[S] 아래로 \n");
	printf("	[D] 오른쪽 \n");
	while (true) {
		if (KeyControl() == SUBMIT)
			break;
	}
}

int main(int argc, char* argv[])

{
	init();

	titleDraw();
	while (true) {
		int enter = menuDraw();
		if (enter == 0) {
			system("cls");
			DrawMap();
			ConnectGame();
		}
		else if (enter == 1) {
			system("cls");
			infoDraw();

		}
	}

}



void ErrorHandling(char* msg)

{

	fputs(msg, stderr);

	fputc('\n', stderr);

	exit(1);

}

int KeyControl()
{
	char temp = _getch();
	if (temp == 'w' || temp == 'W')
		return UP;
	else if (temp == 'a' || temp == 'A')
		return LEFT;
	else if (temp == 's' || temp == 'S')
		return DOWN;
	else if (temp == 'd' || temp == 'D')
		return RIGHT;
	else if (temp == '\r')
		return SUBMIT;

}
