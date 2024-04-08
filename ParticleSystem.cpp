#include "ParticleSystem.h"
#include "ParticleSystemManager.h"

LestaTest::ParticleSystem::ParticleSystem()
{
	renderer_ = ParticleRenderer::create();
	emitterPool_.resize(maxEmitters_);
}

