#include "pch.h"
#include "SendBuffer.h"
#include "Session.h"

SendBuffer::SendBuffer(int32 bufferSize)
{
	_buffer.resize(bufferSize);

}

SendBuffer::~SendBuffer()
{

}

void SendBuffer::CopyData( int32 len)
{
	ASSERT_CRASH(Capacity() >= len);
	//::memcpy(_buffer.data()+sizeof(PacketHeader), data, len);
	_writeSize = len;
}
