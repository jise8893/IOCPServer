#pragma pack(1)
struct PacketHeader
{
	uint16 size;
	uint16 id; //프로토콜 id (ex id=1 로그인 2 이동요청)
};

struct PacketMsg_S_TEST:PacketHeader
{
	char buffer[100];
};
struct PacketMsgMoveTo:PacketHeader
{
	uint32 x;
	uint32 y;
};
