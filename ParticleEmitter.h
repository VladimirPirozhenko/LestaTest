#pragma once
#include "Particle.h"
#include "Random.h"
#include "Profiler.h"
#include "ParticleRenderer.h"
#include <vector>
#include <functional>
#include "Action.h"
#include "ParticlePack.h"

#define PACKED_
namespace LestaTest
{

	class ParticleEmitter
	{
	public:
		ParticleEmitter() = default;
		ParticleEmitter(const Math::Vec2 &spawnPosition,const Math::Vec2 &initialVelocity, const Color &color, size_t particleBank,size_t emitRate,const Math::Vec2& lifetimeMinMax)
		{
			initialVelocity_ = initialVelocity;
			emitRate_ = emitRate;
			particlePrefab_.position = spawnPosition;
			particlePrefab_.velocity = initialVelocity;
			minLifeTime = lifetimeMinMax.x;
			maxLifeTime = lifetimeMinMax.y;
			particlePrefab_.color = color;
			poolIndex_ = particleBank - 1;
#ifndef PACKED
			particlePool_.resize(particleBank);
#else
			particles_.resize(particleBank);
#endif
		}

		void particleExpired(Particle& particle)
		{
			particle.isAlive = false;
			aliveParticleCount_--;
			onExpireCallback_(std::move(particle));
		}

		void update(float dt)
		{
#ifndef PACKED
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
				particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x,2),std::powf(particle.acceleration.y, 2))/2) * dt;
			}
			if (aliveParticleCount_ == 0)
				isAlive_ = false;
#else
			for (size_t i = 0; i < particles_.size(); i++)
			{
				if (particles_.isAlive[i])
					continue;
				particles_.position[i] += particles_.velocity[i];
			}

			for (size_t i = 0; i < particles_.size(); i++)
			{
				if (particles_.isAlive[i])
					continue;
				if (particles_.lifeTimeRemains[i] <= 0.f)
				{
					particles_.isAlive[i] = false;
					aliveParticleCount_--;
					continue;
				}
			}

			for (size_t i = 0; i < particles_.size(); i++)
			{
				if (particles_.isAlive[i])
					continue;
				particles_.lifeTimeRemains[i] -= dt;
			}
			if (aliveParticleCount_ == 0)
				isAlive_ = false;
#endif

		}

		void render(IParticleRendererPtr renderer)
		{
#ifndef PACKED
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
		
			for (size_t i = 0; i < emitRate_; i++)
			{
#ifndef PACKED
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
