#pragma once
#include <vector>
#include "Vec2.h"
#include "PtrTypedefs.h"

namespace LestaTest
{
	class ParticleSystem
	{
	public:
		ParticleSystem() noexcept;
		~ParticleSystem() = default;

		void update(float dt);
		void render() const;
		void emit() const;

		void setPosition(const Math::Vec2& position);

		void addEmitter(const ParticleEmitterPtr emitter) noexcept;

		void setRenderer(const IParticleRendererPtr renderer);

	private:

		IParticleRendererPtr renderer_;
		std::vector<ParticleEmitterPtr> emitters_;
	};	
}

