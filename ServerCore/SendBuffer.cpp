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
	_writeSize = len;
}
