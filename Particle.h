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
	     Particle(const Particle& other) noexcept
		 { 
			std::cout << "COPY " << '\n';
		 }

		Particle& operator=(const Particle& t)
		{
			std::cout << "COPY ASSIGN " << '\n';
			return *this;
		}

	 //inline constexpr Vector2(Vector2<T>&& other) noexcept
	 //{
	 //    x = std::move(other.x);
	 //    y = std::move(other.y);
	 //}

	 //Vector2 operator=Vector2(Vector2<T>&& other) noexcept
	 //{
	 //    x = std::move(other.x);
	 //    y = std::move(other.y);
	 //}
		Math::Vec2 position = Math::Vec2(1.f, 1.f);
		Math::Vec2 velocity = Math::Vec2(1.f, 1.f);
		Math::Vec2 acceleration = Math::Vec2(0.f, 0.f);

		Color color = Color(1.f,1.f,1.f);
	
		float lifeTime = 1.0f;
		float lifeTimeRemains = 1.0f;
		bool isAlive = false;
	};
}
