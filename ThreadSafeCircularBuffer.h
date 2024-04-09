#pragma once
#include <thread>
#include <mutex>
#include "Profiler.h"

namespace LestaTest
{
	template <typename T, size_t Capacity>
	class ThreadSafeCircularBuffer
	{
	public:
		
		bool tryPush(const T& item) noexcept
		{
			bool success = false;
			std::unique_lock<std::mutex> lock(bufferMutex_);
			size_t next = increment(head_);
			if (next != tail_)
			{
				items_[head_] = item;
				head_ = next;
				success = true;
			}
			return success;
		}

		bool tryPop(T& item) noexcept
		{
			bool success = false;
			std::unique_lock<std::mutex> lock(bufferMutex_);
			if (tail_ != head_)
			{
				item = items_[tail_];
				tail_ = increment(tail_);
				success = true;
			}
			return success;
		}

	private:

		size_t increment(size_t index)
		{
			return (index + 1) % Capacity;
		}

		size_t head_ = 0;
		size_t tail_ = 0;

		T items_[Capacity];
		std::mutex bufferMutex_;
	};
}
