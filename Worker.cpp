#include "Worker.h"

Worker::Worker(std::size_t maxJobs)
{
   // _queue = { maxJobs };
}//

void Worker::start()
{
}

void Worker::stop()
{
}

bool Worker::isRunning() const
{
    return false;
}

void Worker::submit(Job* job)
{
    //_queue.push(job);
}

void Worker::wait(Job* job)
{
}
