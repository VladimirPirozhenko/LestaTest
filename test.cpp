#include <thread>
#include <mutex>
#include <atomic>

#include "./nvToolsExt.h"

#include "test.h"
#include <GL/glut.h>

#include "Input.h"
#include <thread>
#include "ParticleSystem.h"
#include "ParticleRenderer.h"
#include "Random.h"

#include "Color.h"
#include "ExplosionEffect.h"
#include <future>
// Just some hints on implementation
// You could remove all of them

static std::atomic_int globalTime;
static std::atomic_bool workerMustExit = false;

using namespace LestaTest;
using namespace LestaTest::Math;

namespace test
{
	static LestaTest::ExplosionEffect explosionEffect;
	static LestaTest::ParticleSystem particleSystem_;
	static LestaTest::Particle particlePrefab_; 
}

using namespace LestaTest;
using namespace LestaTest::Math;
static std::mutex renderMutex;
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

			Math::vec2 mousePos = Input::getMousePosition();

			//test::particlePrefab_.position = Math::Vec2(mousePos.x, test::SCREEN_HEIGHT - mousePos.y);

			//test::particlePrefab_.lifeTime = 2.f;
			//ParticleEmitter emitter;
			//(test::particlePrefab_.velocity, test::particlePrefab_.velocity,Color::blue, 1000, 5.f);
			
			//test::particleSystem_.emit();
			
			float deltaTime = delta / 1000.f;
			std::unique_lock<std::mutex>(renderMutex);
			test::explosionEffect.update(deltaTime);
			//std::unique_lock<std::mutex>(renderMutex);
			//test::explosionEffect.update(deltaTime);
			//test::explosionEffect.update(deltaTime);
			//test::particleSystem_.update(deltaTime);
	
			// some code
		}
	
		static const int MIN_UPDATE_PERIOD_MS = 10;
		if (delta < MIN_UPDATE_PERIOD_MS)
			std::this_thread::sleep_for(std::chrono::milliseconds(MIN_UPDATE_PERIOD_MS - delta));

		nvtxRangePop();
	}
}

void test::mouseButtonCallback(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		Input::onMouseButtonPressed((eMouseCode)button);
	}
	else if (state == GLUT_UP)
		Input::onMouseButtonReleased((eMouseCode)button);
}

void mousePosCallback(int x, int y)
{
	Input::onMousePosChanged(Math::vec2{x, y});
	glutPostRedisplay();
}

void test::init(void)
{
	LestaTest::Random::init();
	LestaTest::JobSystem::get().init();
	glutMotionFunc(mousePosCallback);
	test::particlePrefab_.velocity = Math::Vec2(Random::getFloat(-10, 10), Random::getFloat(-10, 10)) * 20;
	//for (int i = 0; i < 2048; i++)

	
		//ParticleEmitter emitter = ParticleEmitter(test::particlePrefab_.position, { 1,1 }, Color(Random::getFloat(), Random::getFloat(), Random::getFloat()), 64/*Random.getFloat(64, 64)*/, 20.f);
		//emitter.setRandomVelocity({ -100,-100 }, { 100,100 });

		//test::particleSystem_.addEmitter(emitter);
	//}
	//std::vector<std::future<void>> threads(std::thread::hardware_concurrency());
	//for (auto thread = threads.begin(); thread != threads.end(); ++thread)
	//{
	//	*thread = std::async(std::launch::async, [&threads](std::ptrdiff_t id)
	//	{
	//	}
	//}

	std::thread workerThread(WorkerThread);
	workerThread.detach(); // Glut + MSVC = join hangs in atexit()
}

void test::term(void)
{
	// some code

	workerMustExit = true;

	// some code
}

void test::render(void)
{
	//ParticleManager - render
	//static int lastTime = 0;
	//const int time = globalTime.load();
	//const int delta = time - lastTime;
	//lastTime = time;
	//float deltaTime = delta / 1000.f;
	//test::explosionEffect.update(deltaTime);
	//test::particleSystem_.render();
	std::unique_lock<std::mutex>(renderMutex);
	test::explosionEffect.render();
}

void test::update(int dt)
{
	globalTime.fetch_add(dt);
	float deltaTime = dt / 1000.f;
	
	//float deltaTime = dt / 1000.f;
	
	//test::explosionEffect.update(deltaTime);
	//Math::vec2 mousePos = Input::getMousePosition();

	//test::particlePrefab_.position = Math::Vec2(mousePos.x, test::SCREEN_HEIGHT - mousePos.y);

	//test::particlePrefab_.lifeTime = 2.f;
	////ParticleEmitter emitter;
	////(test::particlePrefab_.velocity, test::particlePrefab_.velocity,Color::blue, 1000, 5.f);
	//test::particleSystem_.setPosition(Math::Vec2(mousePos.x, test::SCREEN_HEIGHT - mousePos.y));
	////test::particleSystem_.emit();
	//test::particleSystem_.emit();
	//float deltaTime = dt / 1000.f;

	//test::particleSystem_.update(deltaTime);

}


void test::on_click(int x, int y)
{
	//test::particleSystem_.setPosition(Math::Vec2(x, test::SCREEN_HEIGHT - y));
	//test::particleSystem_.emit();
	explosionEffect.setPosition(Math::Vec2(x, y));
	explosionEffect.explosion();
	//test::particleSystem_.setPosition(Math::Vec2(x, y));
//	test::particleSystem_.emit();
	//particlePrefab_.position = Math::Vec2(x, test::SCREEN_HEIGHT - y);
	//particlePrefab_.velocity = Math::Vec2::down()*20;
	//particlePrefab_.lifeTime = 10.f;
	//particleSystem_.emit(particlePrefab_);
	//for (size_t i = 0; i < 2; ++i)
	//	platform::drawPoint(x, test::SCREEN_HEIGHT - y, 0, 1, 0, 0);

}

