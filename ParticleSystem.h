#pragma once
#include "Particle.h"
#include "ParticleRenderer.h"
#include <vector>
#include "Profiler.h"
#include "Random.h"
#include "ParticleEmitter.h"
#include "memory"
#include "JobSystem.h"
#include "PtrTypedefs.h"

#define EMITTER_POOL3

namespace LestaTest
{
	class ParticleSystem;
	using ParticleSystemPtr = Ptr<ParticleSystem>;

	class ParticleSystem
	{
	public:

		ParticleSystem();
		~ParticleSystem() = default;
		void update(float dt)
		{
			//PROFILE_FUNCTION();	
			for (ParticleEmitter& emitter : emitters_)
			{
				if (!emitter.isAlive())
					continue;
				emitter.update(dt);
			}		
		}
		void render()  
		{
			//PROFILE_FUNCTION();
			for (ParticleEmitter& emitter : emitters_)
			{
				if (emitter.isAlive())
				{
					//emitter.cull(renderer_);
					emitter.render(renderer_);
				}
					
			}
		}
#ifndef EMITTER_POOL
		void emit()
		{
			for (ParticleEmitter& emitter : emitters_)
			{
				emitter.emit();
			}
		}
#else
		void emit()
		{
			for (auto& emitter : emitterPool_)
			{
				if (!emitter.isAlive())
					return;
				ParticleEmitter& emitter = emitterPool_[poolIndex_];
				emitter.emit();

				if (poolIndex_ == 0)
				{
					poolIndex_ = emitterPool_.size() - 1;
				}
				else
				{
					poolIndex_--;
					poolIndex_ = poolIndex_ % emitterPool_.size();
				}
			}
		}
#endif
		void setPosition(const Math::Vec2 position) 
		{ 
			for (ParticleEmitter& emitter : emitters_)
			{
				emitter.setPosition(position);
			}
		}

		void addEmitter(const ParticleEmitter& emitter)
		{
			emitters_.emplace_back(emitter);
		}

		inline void setRenderer(const IParticleRendererPtr renderer)
		{
			renderer_ = renderer;
		}

	private:

		IParticleRendererPtr renderer_;
		std::vector<ParticleEmitter> emitters_;
		std::vector<ParticleEmitter> emitterPool_;
		size_t poolIndex_ = maxEmitters_ - 1;
		//static constexpr size_t effectsCount = 20480;
		//static constexpr size_t particlesPerEffect = 640;
		static constexpr size_t maxEmitters_ = 2048;

	};	
}

