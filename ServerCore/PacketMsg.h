#pragma pack(1)
struct PacketHeader
{
	uint16 size;
	uint16 id; //�������� id (ex id=1 �α��� 2 �̵���û)
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
