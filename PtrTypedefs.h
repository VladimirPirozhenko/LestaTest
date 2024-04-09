#pragma once
#include <memory>

namespace LestaTest
{
	template<typename T>
	using Unique = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Unique<T> makeUnique(Args&& ... args)
	{
		return std::unique_ptr<T>(std::make_unique<T>(std::forward<Args>(args)...));//new T(std::forward<Args>(args)...));//
	}

	template<typename T>
	using Ptr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ptr<T> makePtr(Args&& ... args)
	{
		return std::shared_ptr<T>(std::make_shared<T>(std::forward<Args>(args)...));//);//new T(std::forward<Args>(args)...)
	}

	class ParticleEmitter;
	typedef Ptr<ParticleEmitter> ParticleEmitterPtr;

	class IParticleRenderer;
	typedef Ptr<IParticleRenderer> IParticleRendererPtr;

	class ParticleSystem;
	typedef Ptr<ParticleSystem> ParticleSystemPtr;
};
