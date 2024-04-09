#include "ParticleRenderer.h"

namespace LestaTest
{
	ParticleRenderer::Frustum ParticleRenderer::frustum_ = { 0.f, platform::SCREEN_WIDTH, platform::SCREEN_HEIGHT, 0.f };

	ParticleRendererPtr ParticleRenderer::create()
	{
		return ParticleRendererPtr(std::make_shared<ParticleRenderer>());
	}

	void ParticleRenderer::render(const Math::Vec2& pos, const Color& color) const
	{
		platform::drawPoint(pos.x, platform::SCREEN_HEIGHT - pos.y, color.r, color.g, color.b, color.a);
	}

	bool ParticleRenderer::isInFrustum(const Math::Vec2& pos)
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
}
