#pragma once
#include "Vec2.h"
#include "Color.h"
#include "PtrTypedefs.h"

namespace test
{
	const float SCREEN_WIDTH = 1024;
	const float SCREEN_HEIGHT = 768;
}
namespace platform
{
	void drawPoint(float x, float y, float r, float g, float b, float a);
};

namespace LestaTest
{
	class IParticleRenderer;
	using IParticleRendererPtr = Ptr<IParticleRenderer>;

	class IParticleRenderer
	{
	public:
		virtual ~IParticleRenderer() = default;
		virtual void render(const Math::Vec2& pos, const Color& color) const  = 0;
	};

	class ParticleRenderer;
	using ParticleRendererPtr = Ptr<ParticleRenderer>;

	class ParticleRenderer : public IParticleRenderer
	{
	public:
		static ParticleRendererPtr create();

		ParticleRenderer() = default;

		virtual ~ParticleRenderer() = default;

		inline void render(const Math::Vec2& pos, const Color& color) const override final;

		static bool isInFrustum(const Math::Vec2& pos);

	private:
		struct Frustum
		{
			float left = 0;
			float right = 0;
			float top = 0;
			float bottom = 0;
		};

		static Frustum frustum_;
	};
}
