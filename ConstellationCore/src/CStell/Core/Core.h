#pragma once

#include <memory>
#include <filesystem>

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
	#if defined(CSTELL_PLATFORM_WINDOWS)
		#define CSTELL_DEBUGBREAK() __debugbreak()
	#endif
#endif

#define CSTELL_EXPAND_MACRO(x) x
#define CSTELL_STRINGIFY_MACRO(x) #x

#ifdef  CSTELL_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define CSTELL_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { CSTELL##type##ERROR(msg, __VA_ARGS__); CSTELL_DEBUGBREAK(); } }
	#define CSTELL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CSTELL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define CSTELL_INTERNAL_ASSERT_NO_MSG(type, check) CSTELL_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", CSTELL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define CSTELL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define CSTELL_INTERNAL_ASSERT_GET_MACRO(...) CSTELL_EXPAND_MACRO( CSTELL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CSTELL_INTERNAL_ASSERT_WITH_MSG, CSTELL_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define CSTELL_ASSERT(...) CSTELL_EXPAND_MACRO( CSTELL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define CSTELL_CORE_ASSERT(...) CSTELL_EXPAND_MACRO( CSTELL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )

#else
	#define	CSTELL_ASSERT(...)
	#define	CSTELL_CORE_ASSERT(...)
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