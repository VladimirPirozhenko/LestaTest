#pragma once
#include <atomic>

class Job
{
public:
    Job() = default;
    Job(void(*jobFunction)(Job&));

    void run();
    bool isFinished() const;

private:
    void(*_jobFunction)(Job&);
    std::atomic_size_t _unfinishedJobs;

    void finish();
};