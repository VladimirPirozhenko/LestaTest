#pragma once
#include "ParticleSystem.h"
#include <vector>


namespace LestaTest
{
	class ParticleSystemManager
	{
	public:
		static ParticleSystemManager& get()
		{
			static ParticleSystemManager instance;
			return instance;
		};
		ParticleSystemManager() = default;
		~ParticleSystemManager() = default;

		void update(float dt)
		{
			for (ParticleSystemPtr& particleSystem : particleSystems_)
			{
				particleSystem->update(dt);
			}
		};

		void render() const
		{
			for (const ParticleSystemPtr& particleSystem : particleSystems_)
			{
				particleSystem->render();
			}
		};

		void registerSystem(const ParticleSystemPtr particleSystem)
		{
			particleSystems_.emplace_back(particleSystem);
		}

		void unregisterSystem(const Ptr<ParticleSystem> particleSystem) 
		{
			auto it = std::find(particleSystems_.begin(), particleSystems_.end(), particleSystem);

			if (it != particleSystems_.end()) 
				particleSystems_.erase(it);
		}

		ParticleSystemManager(ParticleSystemManager const&) = delete;
		ParticleSystemManager(ParticleSystemManager&&) = delete;
		ParticleSystemManager& operator=(ParticleSystemManager const&) = delete;
		ParticleSystemManager& operator=(ParticleSystemManager&&) = delete;
	private:
		
		std::vector<Ptr<ParticleSystem>> particleSystems_;
	};
};


