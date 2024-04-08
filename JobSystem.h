#pragma once
#include <functional>
#include <RingBuffer.h>
#include <algorithm>    // std::max
#include <atomic>    // to use std::atomic<uint64_t>
#include <thread>    // to use std::thread
#include <mutex>
#include <condition_variable>    // to use std::condition_variablle
#include <sstream>
#include <assert.h>
#include "JobQueue.h"
namespace LestaTest
{
	class JobSystem
	{
	public:
		static JobSystem& get()
		{
			static JobSystem instance;
			return instance;
		};
		~JobSystem() = default;
		// Create the internal resources such as worker threads, etc. Call it once when initializing the application.
		void init();

		// Add a job to execute asynchronously. Any idle thread will execute this job.
		void execute(const std::function<void()>& job);
		// Check if any threads are working currently or not
		bool isBusy();

		// wait until all threads become idle
		void wait();

		JobSystem(JobSystem const&) = delete;
		JobSystem(JobSystem&&) = delete;
		JobSystem& operator=(JobSystem const&) = delete;
		JobSystem& operator=(JobSystem&&) = delete;
	private:
		JobSystem() = default;
		void poll();


		uint32_t numThreads = 0;    // number of worker threads, it will be initialized in the init() function
		ThreadSafeRingBuffer<std::function<void()>, 16> jobPool;    // a thread safe queue to put pending jobs onto the end (with a capacity of 256 jobs). A worker thread can grab a job from the beginning
		//std::unique_ptr<JobQueue> jobPool;
		std::condition_variable wakeCondition;    // used in conjunction with the wakeMutex below. Worker threads just sleep when there is no job, and the main thread can wake them up
		std::mutex wakeMutex;    // used in conjunction with the wakeCondition above
		uint64_t currentLabel = 0;    // tracks the state of execution of the main thread
		std::atomic<uint64_t> finishedLabel;    // track the state of execution across background worker threads
	};
}
