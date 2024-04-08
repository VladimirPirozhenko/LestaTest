#include "JobSystem.h"


void JobSystem::init()
{
	// init the worker execution state to 0:
	finishedLabel.store(0);

	// Retrieve the number of hardware threads in this system:
	auto numCores = std::thread::hardware_concurrency();

	// Calculate the actual number of worker threads we want:
	numThreads = std::max(1u, numCores);

	// Create all our worker threads while immediately starting them:
	for (uint32_t threadID = 0; threadID < numThreads; ++threadID)
	{
		std::thread worker([this] {

			std::function<void()> job; // the current job for the thread, it's empty at start.

			// This is the infinite loop that a worker thread will do 
			while (true)
			{
				if (jobPool.pop_front(job)) // try to grab a job from the jobPool queue
				{
					// It found a job, execute it:
					job(); // execute job
					finishedLabel.fetch_add(1); // update worker label state
				}
				else
				{
					// no job, put thread to sleep
					std::unique_lock<std::mutex> lock(wakeMutex);
					wakeCondition.wait(lock);
				}
			}

			});

#ifdef _WIN32
		// Do Windows-specific thread setup:
		//HANDLE handle = (HANDLE)worker.native_handle();

		//// Put each thread on to dedicated core
		//DWORD_PTR affinityMask = 1ull << threadID;
		//DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
		//assert(affinity_result > 0);

		////// Increase thread priority:
		////BOOL priority_result = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
		////assert(priority_result != 0);

		//// Name the thread:
		//std::wstringstream wss;
		//wss << "JobSystem_" << threadID;
		//HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
		//assert(SUCCEEDED(hr));
#endif // _WIN32

		worker.detach(); // forget about this thread, let it do it's job in the infinite loop that we created above
	}
}

// This little helper function will not let the system to be deadlocked while the main thread is waiting for something
inline void JobSystem::poll()
{
	wakeCondition.notify_one(); // wake one worker thread
	std::this_thread::yield(); // allow this thread to be rescheduled
}

void JobSystem::execute(const std::function<void()>& job)
{
	// The main thread label state is updated:
	currentLabel += 1;

	// Try to push a new job until it is pushed successfully:
	while (!jobPool.push_back(job)) 
	{
		poll(); 
	}

	wakeCondition.notify_one(); // wake one thread
}

bool JobSystem::isBusy()
{
	// Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
	return finishedLabel.load() < currentLabel;
}

void JobSystem::wait()
{
	while (isBusy()) { poll(); }
}

