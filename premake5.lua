workspace "ConstellationEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root directory
IncludeDir = {}
IncludeDir["glfw"] = "ConstellationCore/vendor/glfw/include"
IncludeDir["Glad"] = "ConstellationCore/vendor/Glad/include"
IncludeDir["glm"] = "ConstellationCore/vendor/glm"
IncludeDir["ImGui"] = "ConstellationCore/vendor/imgui"

group "Dependencies"
	include "ConstellationCore/vendor/glfw"
	include "ConstellationCore/vendor/Glad"
	include "ConstellationCore/vendor/imgui"
group ""

project "ConstellationCore"
	location "ConstellationCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .."/%{prj.name}")
	objdir ("bin-int/" .. outputdir .."/%{prj.name}")
	
	pchheader "CStellpch.h"
	pchsource "%{prj.name}/src/CStellpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"glfw",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"


	-- Warning	C4996	'stdext::checked_array_iterator<T *>::_Verify_offset': warning STL4043: stdext::checked_array_iterator,
	-- stdext::unchecked_array_iterator,
	--  and related factory functions are non-Standard extensions and will be removed in the future.
	--  std::span (since C++20) and gsl::span can be used instead. You can define 
	-- _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING or _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS to suppress this warning.
	-- 		with
	-- 		[
	-- 			T=int
	-- 		]	ConstellationCore	C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\xutility	1255
	


	defines {
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
		"_CRT_SECURE_NO_WARNINGS",
		"CSTELL_PLATFORM_WINDOWS",
		"CSTELL_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
	}
	
	filter "configurations:Debug"
		defines "CSTELL_DEBUG"
		runtime "debug"
		symbols "On"

	filter "configurations:Release"
		defines "CSTELL_RELEASE"
		runtime "release"
		optimize "On"

	filter "configurations:Dist"
		defines "CSTELL_DIST"
		runtime "release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .."/%{prj.name}")
	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"ConstellationCore/vendor/spdlog/include",
		"ConstellationCore/src",
		"%{IncludeDir.glm}"
	}

	links {
		"ConstellationCore"
	}

	filter "system:windows"
		systemversion "latest"

	defines {
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
		"CSTELL_PLATFORM_WINDOWS"
	}
	
	filter "configurations:Debug"
		defines "CSTELL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CSTELL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CSTELL_DIST"
		runtime "Release"
		optimize "on"
