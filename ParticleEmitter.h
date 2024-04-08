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
#include <atomic>
#include "ParticleCulling.h"



namespace LestaTest
{
	class ParticleEmitter;
	using ParticleEmitterPtr = Ptr<ParticleEmitter>;
#define ARRAY2D
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
#ifndef ARRAY2D
			poolIndex_ = particleBank - 1;
#else
			poolIndex_ = particleBank / emitRate - 1;//particleBank - 1;
#endif
			particlePools_.resize(particleBank / emitRate);
			for (auto& particlePool : particlePools_)
			{
				particlePool.resize(emitRate);
			}
			particlePool_.resize(particleBank);
		}


//#define THREADED
		void particleExpired(Particle& particle)
		{
			particle.isAlive = false;
			aliveParticleCount_.fetch_sub(1);
			onExpireCallback_(std::move(particle));
		}
		
		void update(float dt)
		{
			PROFILE_FUNCTION();
#ifndef ARRAY2D
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
//}

//#ifndef THREADED
			//for (auto& pool : particlePools_)
			//{
			//	for (auto& particle : pool)
			//	{
			//		//for (Particle& particle : particlePool_)
			//		{
			//			if (!particle.isAlive)
			//				continue;
			//			if (particle.lifeTimeRemains <= 0.f)
			//			{
			//				particleExpired(particle);
			//				continue;
			//			}
			//			particle.lifeTimeRemains -= dt;
			//			//s = s0 + v + at ^ 2 / 2
			//			particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
			//		}
			//	}
			//}
			//if (aliveParticleCount_ == 0)
			//	isAlive_ = false;
#else
			auto updateParticle = [&, this](std::vector<Particle>& pool)
			{
					for (auto& particle : pool)
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
					}
			};
			auto updateParticles = [&, this]()
			{
					std::for_each(particlePools_[0].begin(), particlePools_[0].end(), [&](Particle& particle)
						{
							//for (auto& particle : pool)
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
							}
						});
			};
//			std::unique_lock<std::mutex> lock(partilcePoolMutex_);
			for (auto& pool : particlePools_)
			{
				JobSystem::get().execute([&, this]  {
					//std::for_each(std::execution::par, pool.begin(), pool.end(), [&](Particle& particle)
					//{
					//	{
					//		if (!particle.isAlive)
					//			return;

					//		if (particle.lifeTimeRemains <= 0.f)
					//		{
					//			particleExpired(particle);
					//			return;
					//		}
					//		particle.lifeTimeRemains -= dt;
					//		//s = s0 + v + at ^ 2 / 2
					//		particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
					//	}
					//});
					//std::for_each(line.begin(), line.end(), updateParticle);
					for (auto& particle : pool)
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
					});
			}
			JobSystem::get().wait();
			if (aliveParticleCount_.load() == 0)
				isAlive_ = false;
#endif
		}

		void render(IParticleRendererPtr renderer)
		{
			//std::unique_lock<std::mutex> lock(partilcePoolMutex_);
			//for (Particle& particle : particlePool_)
			//{
			//auto currentPool = particlePools_[poolIndex_];
			///for (size_t i = 0; i < emitRate_; i++)
#ifndef ARRAY2D
	
			for (Particle& particle : particlePool_)
			{
				if (!particle.isAlive)
					continue;
				if (!renderer->isInFrustum(particle.position))
				{
					particle.isAlive = false;
					aliveParticleCount_.fetch_sub(1);
					continue;
				}
				renderer->render(particle.position, particle.color);
				
			}
#else
			for (auto& pool : particlePools_)
			{
				for (auto& particle : pool)
				{
					if (!particle.isAlive)
						continue;
					if (!renderer->isInFrustum(particle.position))
					{
						particle.isAlive = false;
						aliveParticleCount_.fetch_sub(1);
						continue;
					}
					renderer->render(particle.position, particle.color);
				}
			}
#endif
		}

		void emit()
		{
			//PROFILE_FUNCTION();
#ifndef ARRAY2D
			isAlive_ = true;
			for (size_t i = 0; i < emitRate_; i++)
			{

				//std::unique_lock<std::mutex> lock(partilcePoolMutex_);
				Particle& particle = particlePool_[poolIndex_];

				if (!particle.isAlive)
				{
					particle.isAlive = true;
					aliveParticleCount_.fetch_add(1);
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
			}
		
#else
			isAlive_ = true;
			//
			auto& currentPool = particlePools_[poolIndex_];
			//for (size_t i = 0; i < emitRate_; i++)
			for (auto& particle : currentPool)
			{
				
				//std::unique_lock<std::mutex> lock(partilcePoolMutex_);
				//Particle& particle = particlePool_[poolIndex_];
				
				if (!particle.isAlive)
				{
					particle.isAlive = true;
					aliveParticleCount_.fetch_add(1);
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
			}	
			if (poolIndex_ == 0)
			{
				poolIndex_ = particlePools_.size() - 1;
			}
			else
			{
				poolIndex_--;
				poolIndex_ = poolIndex_ % particlePools_.size();
			}
#endif
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
		ParticleCulling culler_;
		Action<const Particle&> onExpireCallback_;

		size_t emitRate_ = 0;
		bool isAlive_ = false;
		std::atomic_size_t aliveParticleCount_ = 0;
		bool hasRandomVelocity_ = false;

		Math::Vec2 initialVelocity_;
		Math::Vec2 randomVelocityMin_;
		Math::Vec2 randomVelocityMax_;
		float minLifeTime = 0;
		float maxLifeTime = 0;
		std::vector<std::vector<Particle>> particlePools_;
		std::vector<Particle> particlePool_;

		size_t poolIndex_ = 0;
		Particle particlePrefab_;

	};


}
