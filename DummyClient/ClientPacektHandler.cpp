#include "pch.h"
#include "ClientPacektHandler.h"
#include "Session.h"

void ClientPacektHandler::Handle_S_Test(BYTE* buffer, int32 len)
{
	PacketMsg_S_TEST data =*(PacketMsg_S_TEST*)(buffer);
	cout << " id : " << data.id << " data : " << data.buffer << endl;
}
void ClientPacektHandler::HandleMoveTo(BYTE* buffer, int32 len)
{
	PacketMsgMoveTo data = *(PacketMsgMoveTo*)(buffer);
	cout << " id : " << data.id << " x: " << data.x <<" y: "<<data.y << endl;
}

void ClientPacektHandler::HandlerPacket(BYTE* buffer, int32 len)
{
	PacketHeader header = *(PacketHeader*)buffer;
	switch(header.id)
	{
	case S_TEST:
		Handle_S_Test(buffer,len);
		break;
	case S_MOVE:
		HandleMoveTo(buffer, len);
	}
}
