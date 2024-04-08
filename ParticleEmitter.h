#pragma once
#include "Particle.h"
#include "Random.h"
#include "Profiler.h"
#include "ParticleRenderer.h"
#include <vector>
#include <functional>
#include "Action.h"
#include "ParticlePack.h"
#include <execution>
#include "JobSystem.h"
#include "PtrTypedefs.h"
#include <future>

#define PACKED_
static std::mutex partilcePoolMutex_;
namespace LestaTest
{

	class ParticleEmitter
	{
	public:
		ParticleEmitter() = default;
		ParticleEmitter(const Math::Vec2& spawnPosition,const Math::Vec2& initialVelocity, const Color &color, size_t particleBank,size_t emitRate,const Math::Vec2& lifetimeMinMax)
		{
			initialVelocity_ = initialVelocity;
			emitRate_ = emitRate;
			particlePrefab_.position = spawnPosition;
			particlePrefab_.velocity = initialVelocity;
			minLifeTime = lifetimeMinMax.x;
			maxLifeTime = lifetimeMinMax.y;
			particlePrefab_.color = color;
			poolIndex_ = particleBank - 1;
		
			//jobSystem_ = std::make_shared<JobSystem>();

			//jobSystem_ = J();
			//jobSystem_.init();
#ifndef PACKED
			particlePool_.resize(particleBank);
#else
			particles_.resize(particleBank);
#endif
		}
#define THREADED
		void particleExpired(Particle& particle)
		{
			particle.isAlive = false;
			aliveParticleCount_--;
			onExpireCallback_(std::move(particle));
		}
		
		void update(float dt)
		{
		//	PROFILE_FUNCTION();
#ifndef THREADED
			for (Particle& particle : particlePool_)
			{
				if (!particle.isAlive)
					continue;
				if (particle.lifeTimeRemains <= 0.f)
				{
					particleExpired(particle);
					continue;
				}
				particle.lifeTimeRemains -= dt;
				//s = s0 + v + at ^ 2 / 2
				particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
			}
			if (aliveParticleCount_ == 0)
				isAlive_ = false;
#else
			auto updateParticle = [&, this](Particle& particle)
			{
				if (!particle.isAlive)
					return;
				if (particle.lifeTimeRemains <= 0.f)
				{
					particleExpired(particle);
					return;
				}
				particle.lifeTimeRemains -= dt;
				//s = s0 + v + at ^ 2 / 2
				particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
			};
		
			//auto a1 = std::async(&X::foo, &x, 42, "Hello");

			const size_t numJobs = 2;
			size_t chunkSize = particlePool_.size() / numJobs;
			//std::vector<std::thread> threads_;
		
		//	for (int i = 0; i < numJobs; ++i)
		//	{
		//		int start = i * chunkSize;
		//		int end = (i + 1) * chunkSize;
		//		JobSystem::get().execute([&, this] {
		//			//std::unique_lock<std::mutex> lock(partilcePoolMutex_);
		//			for (size_t j = start; j < end; j++)
		//			{
		//				Particle& particle = particlePool_[j];
		//				if (!particle.isAlive)
		//					continue;
		//				if (particle.lifeTimeRemains <= 0.f)
		//				{
		//					particleExpired(particle);
		//					continue;
		//				}
		//				particle.lifeTimeRemains -= dt;
		//				//s = s0 + v + at ^ 2 / 2
		//				particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
		//			}
		//			
		//			});
		//		JobSystem::get().wait();
		////		//	
		/////*		std::thread t([&,this]()
		////		{
		////			for (size_t j = start; j < end; j++)
		////			{
		////				updateParticle(particlePool_[j]);
		////			}
		////		});
		////		t.join();*/
		////		//threads_.emplace_back(std::move(t));			
		//	}
	
			//JobSystem::get().wait();

			auto updateParticles = [&, this](size_t start, size_t end)
			{
				//std::unique_lock<std::mutex>(m);
				for (size_t j = start; j < end; j++)
				{
					Particle& particle = particlePool_[j];
					if (!particle.isAlive)
						continue;
					if (particle.lifeTimeRemains <= 0.f)
					{
						particleExpired(particle);
						continue;
					}
					particle.lifeTimeRemains -= dt;
					//s = s0 + v + at ^ 2 / 2
					particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
				}
			};
			JobSystem::get().execute([&, this] {	
				std::for_each(std::execution::par,particlePool_.begin(), particlePool_.begin() + particlePool_.size() / 2, updateParticle);
				//updateParticles(0, particlePool_.size() / 2);
			});
			JobSystem::get().execute([&, this] {
				std::for_each(std::execution::par, particlePool_.begin() + particlePool_.size()/2, particlePool_.end(), updateParticle);
				//updateParticles(particlePool_.size() / 2, particlePool_.size());
			});;
		/*	JobSystem::get().execute([&, this] {
				updateParticles(particlePool_.size() / 2/3, particlePool_.size());
				});*/
			JobSystem::get().wait();
			if (aliveParticleCount_ == 0)
				isAlive_ = false;
			//JobSystem::get().execute([&, this] {

			//	updateParticles(particlePool_.size() / 2 / 8, particlePool_.size() / 3 / 8);
			//	});
			//JobSystem::get().execute([&, this] {

			//	updateParticles(particlePool_.size() / 3 / 8, particlePool_.size() / 4 / 8);
			//	});
			//JobSystem::get().execute([&, this] {

			//	updateParticles(particlePool_.size() / 4 / 8, particlePool_.size() / 5 / 8);
			//	});
			//JobSystem::get().execute([&, this] {

			//	updateParticles(particlePool_.size() / 5 / 8, particlePool_.size() / 6 / 8);
			//	});
			//JobSystem::get().execute([&, this] {

			//	updateParticles(particlePool_.size() / 6 / 8, particlePool_.size() / 7 / 8);
			//	});
			//JobSystem::get().execute([&, this] {

			//	updateParticles(particlePool_.size() / 7 / 8, particlePool_.size());
			//	});
		
			//std::thread t1([&, this]()
			//{
			//	updateParticles(0, particlePool_.size() / 8);
			//});
			//std::thread t2([&, this]()
			//{
			//	updateParticles(particlePool_.size() / 8, particlePool_.size() / 2/8);
			//});
			//std::thread t3([&, this]()
			//{
			//		updateParticles(particlePool_.size() / 2 / 8, particlePool_.size() / 3 / 8);
			//});
			//std::thread t4([&, this]()
			//	{
			//		updateParticles(particlePool_.size() / 3 / 8, particlePool_.size() / 4/ 8);
			//	});
			//std::thread t5([&, this]()
			//	{
			//		updateParticles(particlePool_.size() / 4 / 8, particlePool_.size() / 5/ 8);
			//	});
			//std::thread t6([&, this]()
			//	{
			//		updateParticles(particlePool_.size() / 5 / 8, particlePool_.size() / 6 / 8);
			//	});
			//std::thread t7([&, this]()
			//	{
			//		updateParticles(particlePool_.size() / 6 / 8, particlePool_.size() / 7 / 8);
			//	});
			//std::thread t8([&, this]()
			//	{
			//		updateParticles(particlePool_.size() / 7 / 8, particlePool_.size());
			//	});
	
			//t1.join();
			//t2.join();
			//t3.join();
			//t4.join();
			//t5.join();
			//t6.join();
			//t7.join();
			//t8.join();
		//	//for (int i = 0; i < threads_.size();i++)
		//	{
		//		//threads_[i].join();
		//	}


#endif
		}

		void render(IParticleRendererPtr renderer)
		{
#ifndef PACKED
			//std::unique_lock<std::mutex> lock(partilcePoolMutex_);
			for (Particle& particle : particlePool_)
			{
				if (!particle.isAlive)
					continue;
				if (!renderer->isInFrustum(particle.position))
				{
					particle.isAlive = false;
					aliveParticleCount_--;
					//particleExpired(particle);
					continue;
				}
				renderer->render(particle.position, particle.color);
			}
#else
			for (size_t i = 0; i < particles_.size(); i++)
			{
				if (!particles_.isAlive[i])
					continue;
				platform::drawPoint(particles_.position[i].x,
									particles_.position[i].y, 
									particles_.color[i].r, 
									particles_.color[i].g, 
									particles_.color[i].b, 
									particles_.color[i].a);
			}
#endif // PACKED

	
		}
		
		void emit()
		{
			//PROFILE_FUNCTION();
			isAlive_ = true;
			//std::unique_lock<std::mutex> lock(partilcePoolMutex_);
			for (size_t i = 0; i < emitRate_; i++)
			{
#ifndef PACKED
				//
				Particle& particle = particlePool_[poolIndex_];
				
				if (!particle.isAlive)
				{
					particle.isAlive = true;
					aliveParticleCount_++;
				}

				particle.position = particlePrefab_.position;
				
				if (hasRandomVelocity_)
					particle.velocity = initialVelocity_ + Random::getVec2(randomVelocityMin_, randomVelocityMax_); 
				else
					particle.velocity = particlePrefab_.velocity;

				particle.lifeTime = Random::getFloat(minLifeTime, maxLifeTime);
				particle.lifeTimeRemains = particle.lifeTime;
				particle.color = particlePrefab_.color;
				particle.acceleration = Math::Vec2(0.f, 10.0f);
				if (poolIndex_ == 0)
				{
					poolIndex_ = particlePool_.size() - 1;
				}
				else
				{
					poolIndex_--;
					poolIndex_ = poolIndex_ % particlePool_.size();
				}
#else

			
				particles_.isAlive[poolIndex_] = true;
				aliveParticleCount_++;

				particles_.position[poolIndex_] = particlePrefab_.position;
			

				if (hasRandomVelocity_)
	
					particles_.velocity[poolIndex_] = initialVelocity_ + Random::getVec2(randomVelocityMin_, randomVelocityMax_);
				else
					particles_.velocity[poolIndex_] = particlePrefab_.velocity;

				particles_.lifeTime[poolIndex_] = Random::getFloat(1, 5);//particlePrefab_.lifeTime;
				particles_.lifeTimeRemains[poolIndex_] = particles_.lifeTime[poolIndex_];
				particles_.color[poolIndex_] = particlePrefab_.color;

				if (poolIndex_ == 0)
				{
					poolIndex_ = particles_.size() - 1;
				}
				else
				{
					poolIndex_--;
					poolIndex_ = poolIndex_ % particles_.size();
				}
#endif

				
			}		
		}

		inline void setPosition(const Math::Vec2 position) { particlePrefab_.position = position; }
		inline Math::Vec2 getPosition() { return particlePrefab_.position; }
		inline bool isAlive() const { return isAlive_; }
		inline Math::Vec2 getInitialVelocity() const { return initialVelocity_; }
		inline void setRandomVelocity(const Math::Vec2& min, const Math::Vec2& max) //sets random additional velocity to initial velocity
		{
			if (min == Math::Vec2::zero() && max == Math::Vec2::zero())
			{
				hasRandomVelocity_ = false;
				return;
			}
			randomVelocityMin_ = min;
			randomVelocityMax_ = max;
			hasRandomVelocity_ = true;
		}

		void addParticleExpireCallback(const std::string &callbackTag,std::function<void(const Particle&)> callback)
		{
			onExpireCallback_.addSubscriber(callbackTag, callback);
		}
		
		void removeParticleExpireCallback(const std::string& callbackTag)
		{
			onExpireCallback_.removeSubscriber(callbackTag);
		}

	private:
		//std::shared_ptr<JobSystem> jobSystem_;
		
		Action<const Particle&> onExpireCallback_;

		size_t emitRate_ = 0;
		bool isAlive_ = false;
		size_t aliveParticleCount_ = 0;
		bool hasRandomVelocity_ = false;

		Math::Vec2 initialVelocity_;
		Math::Vec2 randomVelocityMin_;
		Math::Vec2 randomVelocityMax_;
		float minLifeTime = 0;
		float maxLifeTime = 0;

		std::vector<Particle> particlePool_;
		ParticlePack particles_;

		size_t poolIndex_ = 0;
		Particle particlePrefab_;

	};


}
