#pragma once
#include <type_traits>
#include <future>

class ThreadPool {
public:
	  ThreadPool(size_t workers_count);
	
 template < class F,
          class... Args,
          class R = std::result_of_t < F()>>
  std::future<R> enqueue(F && func, Args&&... args);
};

