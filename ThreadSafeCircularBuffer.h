#pragma once
#include <thread>
#include <mutex>

namespace LestaTest
{
	template <typename T, size_t Capacity>
	class ThreadSafeCircularBuffer
	{
	public:
		inline bool tryPush(const T& item) noexcept
		{
			bool result = false;
			std::unique_lock<std::mutex> lock(bufferMutex_);
			size_t next = increment(head_);
			if (next != tail_)
			{
				items_[head_] = std::move(item);
				head_ = next;
				result = true;
			}
			return result;
		}

		inline bool tryPop(T& item) noexcept
		{
			bool result = false;
			std::unique_lock<std::mutex> lock(bufferMutex_);
			if (tail_ != head_)
			{
				item = items_[tail_];
				tail_ = increment(tail_);
				result = true;
			}
			return result;
		}

	private:
		inline size_t increment(size_t index)
		{
			return (index + 1) % Capacity;
		}
		size_t head_ = 0;
		size_t tail_ = 0;

		T items_[Capacity];
		std::mutex bufferMutex_;
	};
}