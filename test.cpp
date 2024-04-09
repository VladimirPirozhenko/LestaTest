#include <thread>
#include <mutex>
#include <atomic>

#include "./nvToolsExt.h"

#include "test.h"
#include <GL/glut.h>

#include "Random.h"
#include "ExplosionEffect.h"
#include "JobSystem.h"

// Just some hints on implementation
// You could remove all of them

static std::atomic_int globalTime;
static std::atomic_bool workerMustExit = false;

using namespace LestaTest;
using namespace LestaTest::Math;

namespace test
{
	static LestaTest::ExplosionEffect explosionEffect;
}

using namespace LestaTest;
using namespace LestaTest::Math;

static Math::Vec2 mousePosition;
static std::atomic_int inputDataReady{ 0 };


void WorkerThread(void)
{
	while (!workerMustExit)
	{
		nvtxRangePush(__FUNCTION__);

		static int lastTime = 0;
		const int time = globalTime.load();
		const int delta = time - lastTime;
		lastTime = time;

		if (delta > 0)
		{
			while (inputDataReady.load(std::memory_order_acquire) == 1) 
			{
				inputDataReady.store(0);
				test::explosionEffect.setPosition(mousePosition);
				test::explosionEffect.explode();
			}
			float deltaTime = delta / 1000.f;
			test::explosionEffect.update(deltaTime);
		}
	
		static const int MIN_UPDATE_PERIOD_MS = 10;
		if (delta < MIN_UPDATE_PERIOD_MS)
			std::this_thread::sleep_for(std::chrono::milliseconds(MIN_UPDATE_PERIOD_MS - delta));

		nvtxRangePop();
	}
}

void test::init(void)
{
	LestaTest::Random::init();
	LestaTest::JobSystem::get().init();

	std::thread workerThread(WorkerThread);
	workerThread.detach(); // Glut + MSVC = join hangs in atexit()
}

void test::term(void)
{
	workerMustExit = true;
}

void test::render(void)
{
	test::explosionEffect.render();
}

void test::update(int dt)
{
	globalTime.fetch_add(dt);
	float deltaTime = dt / 1000.f;
}

void test::on_click(int x, int y)
{
	mousePosition = Math::Vec2(x, y);
	inputDataReady.store(1, std::memory_order_release);
}

