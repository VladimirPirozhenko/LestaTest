#include "Random.h"

namespace LestaTest
{
	thread_local std::mt19937 Random::randomEngine_;
	std::uniform_int_distribution<std::mt19937::result_type> Random::distribution_;
}