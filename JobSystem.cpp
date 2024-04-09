#include "JobSystem.h"
#include <iostream>

#include "JobSystem.h"
#include <iostream>
#include "Profiler.h"

namespace LestaTest
{
	void JobSystem::init()
	{
		jobCounter_.store(0);

		threadsCount_ = std::max(1u, std::thread::hardware_concurrency());

		for (size_t threadID = 0; threadID < threadsCount_; threadID++)
		{
			std::thread worker([this] {
				Job job;
				while (true)
				{
					doWork(job);
				}
				});
			worker.detach(); 
		}
	}

	void JobSystem::execute(const Job& job)
	{
		jobCounter_.fetch_add(1);
		while (!jobPool_.tryPush(job))
		{
			wakeCondition_.notify_all();
			std::this_thread::yield();
		}
	}

	void JobSystem::doWork(Job& job)
	{
		if (jobPool_.tryPop(job))
		{
			job();
			jobCounter_.fetch_sub(1);
			return;
		}

		std::unique_lock<std::mutex> lock(wakeMutex_);
		wakeCondition_.wait(lock);
	}

	bool JobSystem::hasWork()
	{
		return jobCounter_.load() > 0;
	}

	void JobSystem::wait()
	{
		while (hasWork())
		{
			wakeCondition_.notify_one(); 
			std::this_thread::yield(); 
		}
	}
}
