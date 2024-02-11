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
IncludeDir["Glad"] = "ConstellationCore/vendor/Glad/include"
IncludeDir["glm"] = "ConstellationCore/vendor/glm"
IncludeDir["ImGui"] = "ConstellationCore/vendor/imgui"

group "Dependencies"
	include "ConstellationCore/vendor/GLFW"
	include "ConstellationCore/vendor/Glad"
	include "ConstellationCore/vendor/imgui"
group ""

project "ConstellationCore"
	location "ConstellationCore"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"

	defines {
		"CSTELL_PLATFORM_WINDOWS",
		"CSTELL_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
	}
	
	postbuildcommands {
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
	staticruntime "off"

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
		cppdialect "C++20"
		systemversion "latest"

	defines {
		"CSTELL_PLATFORM_WINDOWS"
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
