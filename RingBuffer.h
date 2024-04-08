#pragma once
#include <thread>
#include <mutex>

// Fixed size very simple thread safe ring buffer
template <typename T, size_t capacity>
class ThreadSafeRingBuffer
{
public:
	// Push an item to the end if there is free space
	//	Returns true if succesful
	//	Returns false if there is not enough space
	inline bool push_back(const T& item)
	{
		bool result = false;
		mtx_.lock();
		size_t next = (head + 1) % capacity;
		if (next != tail)
		{
			data[head] = item;
			head = next;
			result = true;
		}
		mtx_.unlock();
		return result;
	}

	// Get an item if there are any
	//	Returns true if succesful
	//	Returns false if there are no items
	inline bool pop_front(T& item)
	{
		bool result = false;
		mtx_.lock();
		if (tail != head)
		{
			item = data[tail];
			tail = (tail + 1) % capacity;
			result = true;
		}
		mtx_.unlock();
		return result;
	}

private:
	T data[capacity];
	size_t head = 0;
	size_t tail = 0;
	std::mutex mtx_; // this just works better than a spinlock here (on windows)
};