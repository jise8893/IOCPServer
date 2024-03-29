#pragma once
#include <vector>
class SendBuffer : enable_shared_from_this<SendBuffer>
{
public:
	SendBuffer(int32 bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }
	int32 WritePos() { return _writeSize; }
	int32 Capacity() { return static_cast<int32>(_buffer.size()); }
	
	void CopyData( int32 len);

private:
	vector<BYTE> _buffer;
	int32 _writeSize = 0;
};

