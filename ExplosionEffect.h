#pragma once
#include "Vec2.h"
#include "Particle.h"
#include "PtrTypedefs.h"

namespace LestaTest
{
	class ParticleEmitter;
	class ParticleSystem;

	class ExplosionEffect
	{
	public:
		ExplosionEffect() noexcept;
		~ExplosionEffect() = default;
		void setPosition(const Math::Vec2& pos);
		void explode();
		void update(float dt);
		void render();

	private:
		void createExplosionsEmitter();
		void onParticleExpired(const Particle& particle);

		ParticleEmitterPtr explosionEmitter_;
		ParticleSystemPtr explosionParticleSystem_;
	};
}
