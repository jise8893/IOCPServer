#include "pch.h"
#include "ServerPacketHandler.h"
#include "SendBuffer.h"
#include "Session.h"

char sendData[] = "hello my world";
SendBufferRef ServerPacketHandler::MakeS_Test()
{
	SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
	PacketMsg_S_TEST* data = (PacketMsg_S_TEST*)(sendBuffer->Buffer());
	data->id = 1;
	data->size = sizeof(sendData) + sizeof(PacketHeader);
	::memcpy(data->buffer, sendData, sizeof(sendData));
	sendBuffer->CopyData(sizeof(sendData) + sizeof(PacketHeader));

	return sendBuffer;
}

SendBufferRef ServerPacketHandler::MakeMoveTo(uint32 x, uint32 y)
{
	SendBufferRef sendBuffer = make_shared<SendBuffer>(4096);
	PacketMsgMoveTo* data = (PacketMsgMoveTo*)(sendBuffer->Buffer());
	data->id = 2;
	data->size = sizeof(PacketMsgMoveTo);
	data->x = x;
	data->y = y;
	sendBuffer->CopyData(sizeof(PacketMsgMoveTo));
	return sendBuffer;
}

void ServerPacketHandler::HandlerPacket(BYTE* buffer, int32 len)
{
}
