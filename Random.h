#pragma once
#include <random>
#include "Color.h"
#include "Vec2.h"

namespace LestaTest
{
	class Random
	{
	public:
		static void init()
		{
			randomEngine_.seed(randomDevice_());		
		}

		static float getFloat()
		{
			randomEngine_.seed(randomDevice_());
			return static_cast<float>(distribution_(randomEngine_)) / static_cast<float>((std::numeric_limits<size_t>::max)());
		}
		
		static float getFloat(float min,float max)
		{
			static thread_local std::default_random_engine defaultRandomEngine_;
			std::uniform_real_distribution<> distribution(min,max);
			randomEngine_.seed(randomDevice_());
			return static_cast<float>(distribution(randomEngine_));
		}

		static Math::Vec2 getVec2(Math::Vec2 min, Math::Vec2 max)
		{
			return Math::Vec2(getFloat(min.x, max.x), getFloat(min.y, max.y));
		}

		static Color getColor()
		{
			return Color(getFloat(),getFloat(),getFloat());
		}
	private:
		static thread_local std::random_device randomDevice_; 
		static thread_local std::mt19937 randomEngine_;
		static thread_local std::uniform_int_distribution<std::mt19937::result_type> distribution_;
	};
}
