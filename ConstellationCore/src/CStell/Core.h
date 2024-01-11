#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS
	#ifdef CSTELL_BUILD_DLL
		#define CSTELL_API __declspec(dllexport)
	#else
		#define CSTELL_API __declspec(dllimport)
	#endif
#else
	#error Constellation Engine only supports Windows!
#endif

#ifdef  CSTELL_ENABLE_ASSERTS
	#define	CSTELL_ASSERT(x, ...) { if(!(x)) {CSTELL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define	CSTELL_CORE_ASSERT(x, ...) { if(!(x)) {CSTELL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define	CSTELL_ASSERT(x, ...)
	#define	CSTELL_CORE_ASSERT(x, ...)
#endif //  CSTELL_ENABLE_ASSERTS

#define BIT(x) (1 << x)