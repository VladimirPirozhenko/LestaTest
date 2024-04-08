#include "Random.h"

namespace LestaTest
{
	thread_local std::random_device Random::randomDevice_; // obtain a random number from hardware
	thread_local std::mt19937 Random::randomEngine_;
	thread_local std::uniform_int_distribution<std::mt19937::result_type> Random::distribution_;
}