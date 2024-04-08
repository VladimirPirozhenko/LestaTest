#pragma once
#include "Vec2.h"
#include "Color.h"

namespace LestaTest
{
	struct Particle
	{
		Particle()
		{
			//std::cout << "allocParticle";
		}
		~Particle()
		{
			//std::cout << "deleteParticle";
		}
		Math::Vec2 position = Math::Vec2(1.f, 1.f);
		Math::Vec2 velocity = Math::Vec2(1.f, 1.f);
		Math::Vec2 acceleration = Math::Vec2(0.f, 0.f);

		Color color = Color(1.f,1.f,1.f);
	
		float lifeTime = 1.0f;
		float lifeTimeRemains = 1.0f;
		bool isAlive = false;
	};
}
