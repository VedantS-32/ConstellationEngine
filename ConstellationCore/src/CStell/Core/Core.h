#pragma once

#include <memory>

#ifdef CSTELL_PLATFORM_WINDOWS
#if CSTELL_DYNAMIC_LINK
	#ifdef CSTELL_BUILD_DLL
		#define CSTELL_API __declspec(dllexport)
	#else
		#define CSTELL_API __declspec(dllimport)
	#endif
#else
	#define CSTELL_API
#endif
#else
	#error Constellation Engine only supports Windows!
#endif

#ifdef CSTELL_DEBUG
	#define CSTELL_ENABLE_ASSERTS
#endif

#ifdef  CSTELL_ENABLE_ASSERTS
	#define	CSTELL_ASSERT(x, ...) { if(!(x)) {CSTELL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define	CSTELL_CORE_ASSERT(x, ...) { if(!(x)) {CSTELL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define	CSTELL_ASSERT(x, ...)
	#define	CSTELL_CORE_ASSERT(x, ...)
#endif //  CSTELL_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define CSTELL_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace CStell
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}