#include "ParticleEmitter.h"
#include "Random.h"
#include "Profiler.h"
#include "JobSystem.h"
#include "ParticleRenderer.h"

namespace LestaTest
{
	ParticleEmitter::ParticleEmitter(const Math::Vec2& spawnPosition,
		const Math::Vec2& initialVelocity,
		const Color& color,
		size_t particleBank,
		size_t emitRate,
		const Math::Vec2& lifeTimeMinMax)
	{
		particlePrefab_.position = spawnPosition;
		particlePrefab_.velocity = initialVelocity;
		particlePrefab_.color = color;

		minLifeTime = lifeTimeMinMax.x;
		maxLifeTime = lifeTimeMinMax.y;
		
		emitRate_ = emitRate;
		poolIndex_ = particleBank / emitRate_ - 1;

		particlePools_.resize(particleBank / emitRate_);
		for (auto& particlePool : particlePools_)
		{
			particlePool.resize(emitRate_);
		}
	}

	void ParticleEmitter::particleExpired(Particle& particle)
	{
		particle.isAlive = false;
		aliveParticleCount_--;
		onExpireCallback_(std::move(particle));
	}

	void ParticleEmitter::update(float dt)
	{
		//PROFILE_FUNCTION();
		for (auto& pool : particlePools_)
		{
			JobSystem::get().execute([&, this]
				{
					for (auto& particle : pool)
					{
						if (!particle.isAlive)
							continue;

						if (!ParticleRenderer::isInFrustum(particle.position))
						{
							particle.isAlive = false;
							aliveParticleCount_++;
							continue;
						}

						if (particle.lifeTimeRemains <= 0.f)
						{
							particleExpired(particle);
							continue;
						}
						particle.lifeTimeRemains -= dt;
						particle.position += (particle.velocity + Math::Vec2(std::powf(particle.acceleration.x, 2), std::powf(particle.acceleration.y, 2)) / 2) * dt;
					}
				});
		}
		JobSystem::get().wait();
		if (aliveParticleCount_ == 0)
			isAlive_ = false;
	}

	void ParticleEmitter::render(IParticleRendererPtr renderer)
	{
		for (auto& pool : particlePools_)
		{
			for (auto& particle : pool)
			{
				if (!particle.isAlive)
					continue;
				renderer->render(particle.position, particle.color);
			}
		}
	}

	void ParticleEmitter::emit()
	{
		isAlive_ = true;

		auto& currentPool = particlePools_[poolIndex_];
		for (auto& particle : currentPool)
		{
			if (!particle.isAlive)
			{
				particle.isAlive = true;
				aliveParticleCount_++;
			}

			particle.position = particlePrefab_.position;

			if (hasRandomVelocity_)
				particle.velocity = particlePrefab_.velocity + Random::getVec2(randomVelocityMin_, randomVelocityMax_);
			else
				particle.velocity = particlePrefab_.velocity;

			particle.lifeTime = Random::getFloat(minLifeTime, maxLifeTime);
			particle.lifeTimeRemains = particle.lifeTime;
			particle.color = particlePrefab_.color;
			const float gravity = 10.f;
			particle.acceleration = Math::Vec2(0.f, gravity);
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
	}

	void ParticleEmitter::setRandomVelocity(const Math::Vec2& min, const Math::Vec2& max)
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

	void ParticleEmitter::addParticleExpireCallback(const std::string& callbackTag, std::function<void(const Particle&)> callback)
	{
		onExpireCallback_.addSubscriber(callbackTag, std::move(callback));
	}

	void ParticleEmitter::removeParticleExpireCallback(const std::string& callbackTag)
	{
		onExpireCallback_.removeSubscriber(callbackTag);
	}

}
