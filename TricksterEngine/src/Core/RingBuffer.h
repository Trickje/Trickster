#pragma once

template<class T, size_t capacity>
class RingBuffer
{
public:
	RingBuffer();
	inline bool push_back(const T& item)
	{
		size_t next = (head + 1) % capacity;
	}


private:
	T data[capacity];
	size_t tail = 0;
	size_t head = 0;

	
};

