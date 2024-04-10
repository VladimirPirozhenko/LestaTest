#pragma once
#include <functional>
#include <ThreadSafeCircularBuffer.h>
#include <atomic>    
#include <thread>   
#include <mutex>
#include <condition_variable>    

namespace LestaTest
{
	using Job = std::function<void()>;

	class JobSystem
	{
	public:
		static JobSystem& get()
		{
			static JobSystem instance;
			return instance;
		};

		~JobSystem() = default;

		void init();
		void execute(const Job& job);
		void doWork(Job& job);
		bool hasWork();
		void wait();

		JobSystem(JobSystem const&) = delete;
		JobSystem(JobSystem&&) = delete;
		JobSystem& operator=(JobSystem const&) = delete;
		JobSystem& operator=(JobSystem&&) = delete;
	private:
		JobSystem() = default;

		size_t threadsCount_ = 0;    
		constexpr static size_t jobPoolSize_ = 512;
		ThreadSafeCircularBuffer<Job, jobPoolSize_> jobPool_;
		std::condition_variable wakeCondition_;    
		std::mutex wakeMutex_;  
		std::atomic_size_t jobCounter_;   
	};
}
