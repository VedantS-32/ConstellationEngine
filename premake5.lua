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
IncludeDir["GLFW"] = "ConstellationCore/vendor/GLFW/include"

include "ConstellationCore/vendor/GLFW"

project "ConstellationCore"
	location "ConstellationCore"
	kind "SharedLib"
	language "C++"

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
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines {
		"CSTELL_PLATFORM_WINDOWS",
		"CSTELL_BUILD_DLL"
	}
	
	postbuildcommands {
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}
	
	filter "configurations:Debug"
		defines "CSTELL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CSTELL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "CSTELL_DIST"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .."/%{prj.name}")
	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"ConstellationCore/vendor/spdlog/include",
		"ConstellationCore/src"
	}

	links {
		"ConstellationCore"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines {
		"CSTELL_PLATFORM_WINDOWS"
	}
	
	filter "configurations:Debug"
		defines "CSTELL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CSTELL_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "CSTELL_DIST"
		optimize "On"
