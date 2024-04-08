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

class JobSystem
{
public:
	// Create the internal resources such as worker threads, etc. Call it once when initializing the application.
	void init();

	// Add a job to execute asynchronously. Any idle thread will execute this job.
	void execute(const std::function<void()>& job);
	// Check if any threads are working currently or not
	bool isBusy();

	// wait until all threads become idle
	void wait();

private:
	void poll();


	uint32_t numThreads = 0;    // number of worker threads, it will be initialized in the init() function
	ThreadSafeRingBuffer<std::function<void()>, 256> jobPool;    // a thread safe queue to put pending jobs onto the end (with a capacity of 256 jobs). A worker thread can grab a job from the beginning
	std::condition_variable wakeCondition;    // used in conjunction with the wakeMutex below. Worker threads just sleep when there is no job, and the main thread can wake them up
	std::mutex wakeMutex;    // used in conjunction with the wakeCondition above
	uint64_t currentLabel = 0;    // tracks the state of execution of the main thread
	std::atomic<uint64_t> finishedLabel;    // track the state of execution across background worker threads
};