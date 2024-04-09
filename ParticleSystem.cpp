#include "ParticleSystem.h"
#include "ParticleRenderer.h"
#include "ParticleEmitter.h"

namespace LestaTest
{
	ParticleSystem::ParticleSystem() noexcept
	{
		renderer_ = ParticleRenderer::create();
	}

	void ParticleSystem::update(float dt)
	{
		for (const ParticleEmitterPtr& emitter : emitters_)
		{
			if (!emitter->isAlive())
				continue;
			emitter->update(dt);
		}
	}

	void ParticleSystem::render() const
	{
		for (const ParticleEmitterPtr& emitter : emitters_)
		{
			if (emitter->isAlive())
				emitter->render(renderer_);
		}
	}

	void ParticleSystem::emit() const 
	{
		for (const ParticleEmitterPtr& emitter : emitters_)
		{
			emitter->emit();
		}
	}

	void ParticleSystem::setPosition(const Math::Vec2& position) 
	{
		for (ParticleEmitterPtr& emitter : emitters_)
		{
			emitter->setPosition(position);
		}
	}

	void ParticleSystem::addEmitter(const ParticleEmitterPtr emitter) noexcept
	{
		emitters_.emplace_back(emitter);
	}

	void ParticleSystem::setRenderer(const IParticleRendererPtr renderer)
	{
		renderer_ = renderer;
	}
}
