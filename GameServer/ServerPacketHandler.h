#pragma once
class ServerPacketHandler
{
	enum
	{
		S_TEST = 1,
		S_MOVE = 2
	};

public:
	static SendBufferRef MakeS_Test();
	static SendBufferRef MakeMoveTo(uint32 x, uint32 y);
	static SendBufferRef HandlerPacket(BYTE* buffer, int32 len);

};

