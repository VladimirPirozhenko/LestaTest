#include "Job.h"

Job::Job(void(*jobFunction)(Job&)) :
    _jobFunction{ jobFunction }
{
    _unfinishedJobs = 1;
} 

void Job::run()
{
    _jobFunction(*this);
    finish();
}

void Job::finish()
{
    _unfinishedJobs--;
}

bool Job::isFinished() const
{
    return _unfinishedJobs == 0;
}