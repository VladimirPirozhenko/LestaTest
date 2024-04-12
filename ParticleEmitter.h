#pragma once
#include <functional>
#include <vector>
#include "Particle.h"
#include "Action.h"
#include "PtrTypedefs.h"

namespace LestaTest
{
	class ParticleEmitter
	{
	public:
	
		ParticleEmitter() = default;
		ParticleEmitter(const Math::Vec2& spawnPosition,const Math::Vec2& initialVelocity, const Color &color, 
						size_t particleBank,size_t emitRate,const Math::Vec2& lifetimeMinMax);

		void particleExpired(Particle& particle);
		void addParticleExpireCallback(const std::string& callbackTag, std::function<void(const Particle&)> callback);
		void removeParticleExpireCallback(const std::string& callbackTag);

		void update(float dt);

		void render(IParticleRendererPtr renderer);

		void emit();

		void setPosition(const Math::Vec2 position) { particlePrefab_.position = position; }
		Math::Vec2 getPosition() const { return particlePrefab_.position; }
		void setColor(const Color& color) { particlePrefab_.color = color; }
		Color getColor() const { return particlePrefab_.color; }
		bool isAlive() const { return isAlive_; }
		Math::Vec2 getInitialVelocity() const { return initialVelocity_; }
		void setRandomVelocity(const Math::Vec2& min, const Math::Vec2& max);

	private:

		bool isAlive_ = false;
		size_t emitRate_ = 0;
		std::atomic_size_t aliveParticleCount_ {0};
		Action<const Particle&> onExpireCallback_;

		Math::Vec2 initialVelocity_;
		bool hasRandomVelocity_ = false;
		Math::Vec2 randomVelocityMin_;
		Math::Vec2 randomVelocityMax_;

		float minLifeTime = 0;
		float maxLifeTime = 0;

		Particle particlePrefab_;
		size_t poolIndex_ = 0;	
		std::vector<std::vector<Particle>> particlePools_;
	};
}
