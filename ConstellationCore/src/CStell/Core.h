#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS
	#ifdef CSTELL_BUILD_DLL
		#define CSTELL_API __declspec(dllexport)
	#else
		#define CSTELL_API __declspec(dllimport)
	#endif
#else
	#error CStell Engine only supports Windows!
#endif

#define BIT(x) (1 << x)