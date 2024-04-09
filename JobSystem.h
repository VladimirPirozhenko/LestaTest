#pragma once
#include <functional>
#include <ThreadSafeCircularBuffer.h>
#include <algorithm>    // std::max
#include <atomic>    // to use std::atomic<uint64_t>
#include <thread>    // to use std::thread
#include <mutex>
#include <condition_variable>    // to use std::condition_variablle
#include <sstream>
#include <assert.h>

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
		bool hasWork();
		void wait();

		JobSystem(JobSystem const&) = delete;
		JobSystem(JobSystem&&) = delete;
		JobSystem& operator=(JobSystem const&) = delete;
		JobSystem& operator=(JobSystem&&) = delete;
	private:
		JobSystem() = default;

		void poll();
		void run();

		size_t threadsCount_ = 0;    // number of worker threads, it will be initialized in the init() function
		ThreadSafeCircularBuffer<Job, 512> jobPool_;    // a thread safe queue to put pending jobs onto the end (with a Capacity of 256 jobs). A worker thread can grab a job from the beginning
		std::condition_variable wakeCondition_;    // used in conjunction with the wakeMutex_ below. Worker threads just sleep when there is no job, and the main thread can wake them up
		std::mutex wakeMutex_;    // used in conjunction with the wakeCondition_ above
		std::atomic_size_t jobCounter_;    // track the state of execution across background worker threads
	};
}
