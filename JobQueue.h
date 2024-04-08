#pragma once
#include "Job.h"
#include <thread>
#include <vector>
#include <atomic>
#include <functional>


class JobQueue
{
public:
    JobQueue(std::size_t maxJobs);

    bool push(const std::function<void()> &job);
    bool pop(std::function<void()>& job);
    std::size_t size() const;
    bool isEmpty() const;

private:
    std::vector<std::function<void()>> _jobs;
    std::atomic<int> _top, _bottom;
};