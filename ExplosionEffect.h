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
			explosionParticleSystem_ = makePtr<ParticleSystem>();

			const size_t emitRate = 500;
			const size_t maxEffectsCount = 2048;

			explosionEmitter_ = makePtr<ParticleEmitter>(Math::Vec2{ 0,0 }, Math::Vec2{ 1,1 },
												Random::getColor(),
												maxEffectsCount	* emitRate, emitRate,
												Math::Vec2(3.5f,7.f));

			explosionEmitter_->setRandomVelocity({ -100,-100 }, { 100,100 });			
			explosionEmitter_->addParticleExpireCallback("onExplode", std::bind(&ExplosionEffect::onParticleExpired, this, std::placeholders::_1));
			explosionParticleSystem_->addEmitter(explosionEmitter_);
		}
		void setPosition(const Math::Vec2& pos)
		{
			explosionParticleSystem_->setPosition(pos);
		}
		void explosion()
		{
			explosionParticleSystem_->emit();
		}
		void update(float dt)
		{
			explosionParticleSystem_->update(dt);
		}

		void render()
		{
			explosionParticleSystem_->render();
		}
		void onParticleExpired(const Particle& particle)
		{
			const float explosionChance = 0.5f;
			if (explosionChance > Random::getFloat())
			{
				explosionEmitter_->setPosition(particle.position);
				//explosionEmitter_->setColor(particle.color);
				explosionParticleSystem_->emit();
			}			
		}
	private:
		ParticleEmitterPtr explosionEmitter_;
		ParticleSystemPtr explosionParticleSystem_;
	};


}
