#pragma once

#ifdef CSTELL_PLATFORM_WINDOWS
	#ifdef CSTELL_BUILD_DLL
		#define CSTELL_API __declspec(dllexport)
	#else
		#define CSTELL_API __declspec(dllimport)
	#endif
#else
	#error Constellation only supports Windows!
#endif