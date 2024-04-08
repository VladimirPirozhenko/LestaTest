#pragma once
#include "ParticleRenderer.h"

namespace LestaTest
{
	class ParticleCulling
	{
	public:
		ParticleCulling()
		{
			frustum_ = { 0.f, platform::SCREEN_WIDTH, platform::SCREEN_HEIGHT, 0.f };
		}

		bool isInFrustum(const Math::Vec2& pos) const
		{
			if (pos.x >= frustum_.left
				&& pos.x <= frustum_.right
				&& pos.y <= frustum_.top
				&& pos.y >= frustum_.bottom)
			{
				return true;
			}
			return false;
		}

	private:
		struct Frustum
		{
			float left = 0;
			float right = 0;
			float top = 0;
			float bottom = 0;
		};

		Frustum frustum_;
	};


}
