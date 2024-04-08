#include "JobQueue.h"

JobQueue::JobQueue(std::size_t maxJobs)
{
    _jobs.resize(maxJobs);
}

bool JobQueue::push(const std::function<void()>& job)
{
    int bottom = _bottom.load(std::memory_order_acquire);

    if (bottom < static_cast<int>(_jobs.size()))
    {
        _jobs[bottom] = job;
        _bottom.store(bottom + 1, std::memory_order_release);

        return true;
    }
    else
    {
        return false;
    }
}

bool JobQueue::pop(std::function<void()>& job)
{
    int bottom = _bottom.load(std::memory_order_acquire);
    bottom = std::max(0, bottom - 1);
    _bottom.store(bottom, std::memory_order_release);
    int top = _top.load(std::memory_order_acquire);

    if (top <= bottom)
    {    
        job = _jobs[bottom];

        if (top != bottom)
        {
            // More than one job left in the queue
            return true;
        }
        else
        {
            int expectedTop = top;
            int desiredTop = top + 1;

            if (!_top.compare_exchange_weak(expectedTop, desiredTop,
                std::memory_order_acq_rel))
            {
                // Someone already took the last item, abort
                job = nullptr;
                return false;
            }

            _bottom.store(top + 1, std::memory_order_release);
            return true;
        }
    }
    else
    {
        // Queue already empty
        _bottom.store(top, std::memory_order_release);
        return false;
    }
}

std::size_t JobQueue::size() const
{
    return _jobs.size();
}

bool JobQueue::isEmpty() const
{
    return _jobs.size() == 0;
}
