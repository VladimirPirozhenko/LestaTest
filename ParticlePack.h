#pragma once
#include <vector>
#include "Vec2.h"
#include "Color.h"

namespace LestaTest
{
	struct  ParticlePack
	{
		inline size_t size() const noexcept
		{
			return position.size();
		};

		void resize(const size_t newSize) noexcept
		{
			position.resize(newSize);
			velocity.resize(newSize);
			lifeTime.resize(newSize);
			lifeTimeRemains.resize(newSize);
			color.resize(newSize);
			isAlive.resize(newSize);
		};

		void ParticlePack::swap(size_t lhs, size_t rhs)
		{
			std::swap(position[lhs], position[rhs]);
			std::swap(velocity[lhs], velocity[rhs]);
			std::swap(lifeTime[lhs], lifeTime[rhs]);
			std::swap(lifeTimeRemains[lhs], lifeTimeRemains[rhs]);
			std::swap(color[lhs], color[rhs]);
			std::swap(isAlive[lhs], isAlive[rhs]);
		};

		std::vector<Math::Vec2> position;
		std::vector<Math::Vec2> velocity;
		std::vector<float> lifeTime;
		std::vector<float> lifeTimeRemains;
		std::vector<Color> color;
		std::vector<bool> isAlive;
	};
};
