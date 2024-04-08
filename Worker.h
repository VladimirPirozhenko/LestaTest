#pragma once
#include "Job.h"
#include <thread>
#include <vector>
#include <atomic>
#include "JobQueue.h"



class Worker
{
public:

    Worker(std::size_t maxJobs);
    ~Worker() {} ;
    enum class State
    {
        Idle,
        Running,
        Stopping
    };
    void start();
    void stop();
    bool isRunning() const;
    void submit(Job* job);
    void wait(Job* job);

private:
   // JobQueue _queue;
    std::thread::id _threadId;
    std::thread _thread;
    std::atomic<State> _state;
    Job* getJob();
    void join();
};

