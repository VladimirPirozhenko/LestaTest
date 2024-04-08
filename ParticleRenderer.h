#pragma once
#include "Vec2.h"
#include "Color.h"
#include "PtrTypedefs.h"

namespace platform
{
	const float SCREEN_WIDTH = 1024;
	const float SCREEN_HEIGHT = 768;

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
		virtual bool isInFrustum(const Math::Vec2& pos) const = 0;
	};

	class ParticleRenderer;
	using ParticleRendererPtr = Ptr<ParticleRenderer>;

	class ParticleRenderer : public IParticleRenderer
	{
	public:
		static ParticleRendererPtr create()
		{
			return ParticleRendererPtr(std::make_shared<ParticleRenderer>());
		}

		ParticleRenderer()
		{
			frustum_ = { 0.f, platform::SCREEN_WIDTH, platform::SCREEN_HEIGHT, 0.f};
		}

		virtual ~ParticleRenderer() = default;

		void render(const Math::Vec2& pos,const Color& color) const override final
		{
			platform::drawPoint(pos.x, platform::SCREEN_HEIGHT - pos.y, color.r, color.g, color.b, color.a);
		};

		bool isInFrustum(const Math::Vec2& pos) const override final
		{
			if (pos.x  >= frustum_.left      
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
