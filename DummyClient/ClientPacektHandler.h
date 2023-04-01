#pragma once
enum
{
	S_TEST = 1,
	S_MOVE = 2
};
class ClientPacektHandler
{
public:
	static void Handle_S_Test(BYTE* buffer, int32 len);
	static void HandleMoveTo(BYTE* buffer, int32 len);
	static void HandlerPacket(BYTE* buffer,int32 len);
};

