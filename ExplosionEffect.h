#pragma once
#include "ParticleSystem.h"
#include "Particle.h"
#include <memory>

namespace LestaTest
{
	class ExplosionEffect
	{
	public:
		ExplosionEffect()
		{
			//for (int i = 0; i < 2048;i++)
			{
				const size_t emitRate = 64;
				const size_t maxEffectsCount = 2048;
				explosionEmitter_ = ParticleEmitter(Math::Vec2{ 0,0 }, Math::Vec2{ 1,1 },
													Color(Random::getFloat(), 
													Random::getFloat(), 
													Random::getFloat()),
													maxEffectsCount	* emitRate, emitRate,
													Math::Vec2(0.5f,1));

				explosionEmitter_.setRandomVelocity({ -100,-100 }, { 100,100 });

				explosionEmitter_.addParticleExpireCallback("onExplode", std::bind(&ExplosionEffect::onExplode, this, std::placeholders::_1));
				//emitter.onExpireCallback_.addSubscriber("chainedExplosion", std::bind(&ExplosionEffect::onExplode, this, std::placeholders::_1));
				explosionParticleSystem_.addEmitter(explosionEmitter_);
				//explosionParticleSystem_.addModifier(gravityModifier);
			}
		
		/*	ParticleEmitter emitter(initialVelocity,particleCount);
			emitter.setRandomVelocity(from,to);

			explosionParticleSystem_.addModifier(gravity);
			explosionParticleSystem_.setDuration(5);
			explosionParticleSystem_.addEmitter(emitter);*/
		}
		void setPosition(const Math::Vec2& pos)
		{
			explosionParticleSystem_.setPosition(pos);
		}
		void explosion()
		{
			explosionParticleSystem_.emit();
		}
		void update(float dt)
		{
			explosionParticleSystem_.update(dt);
		}

		void render()
		{
			explosionParticleSystem_.render();
		}
		void onExplode(const Particle& particle)
		{
			if (Random::getFloat() > 0.5f)
			{
				explosionParticleSystem_.setPosition(particle.position);
				explosionParticleSystem_.emit();
			}			
		}
	private:
		ParticleEmitter explosionEmitter_;
		ParticleSystem explosionParticleSystem_;
	};


}
