#include "ExplosionEffect.h"
#include <functional>

#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Random.h"
#include "Vec2.h"
#include "Color.h"

namespace LestaTest
{
	ExplosionEffect::ExplosionEffect() noexcept
	{
		explosionParticleSystem_ = makePtr<ParticleSystem>();
		createExplosionsEmitter();
		explosionParticleSystem_->addEmitter(explosionEmitter_);
	}

	void ExplosionEffect::createExplosionsEmitter()
	{
		const size_t emitRate = 64;
		const size_t maxEffectsCount = 2048;
		const Math::Vec2 initialPosition = Math::Vec2{ 0,0 };
		const Math::Vec2 initialVelocity = Math::Vec2{ 0,0 };
		constexpr size_t totalParticleCount = maxEffectsCount * emitRate;

		explosionEmitter_ = makePtr<ParticleEmitter>(initialPosition,
			initialVelocity,
			Random::getColor(),
			totalParticleCount, emitRate,
			Math::Vec2(3.5f, 7.f));

		const Math::Vec2 minVelocity = Math::Vec2{ -100,-100 };
		const Math::Vec2 maxVelocity = Math::Vec2{ 100, 100 };
		explosionEmitter_->setRandomVelocity(minVelocity, maxVelocity);
		explosionEmitter_->addParticleExpireCallback("onExplode", std::bind(&ExplosionEffect::onParticleExpired, this, std::placeholders::_1));
	}

	void ExplosionEffect::onParticleExpired(const Particle& particle)
	{
		const float explosionChance = 0.5f;
		if (explosionChance > Random::getFloat())
		{
			explosionEmitter_->setPosition(particle.position);
			explosionParticleSystem_->emit();
		}
	}

	void ExplosionEffect::setPosition(const Math::Vec2& pos)
	{
		explosionParticleSystem_->setPosition(pos);
	}

	void ExplosionEffect::explode()
	{
		explosionParticleSystem_->emit();
	}

	void ExplosionEffect::update(float dt)
	{
		explosionParticleSystem_->update(dt);
	}

	void ExplosionEffect::render()
	{
		explosionParticleSystem_->render();
	}
};
