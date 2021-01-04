#pragma once

template<class T, size_t capacity>
class RingBuffer
{
public:
	TRICKSTER_API RingBuffer();
	TRICKSTER_API inline bool push_back(const T& item)
	{
		size_t next = (head + 1) % capacity;
	}


private:
	T data[capacity];
	size_t tail = 0;
	size_t head = 0;

	
};

