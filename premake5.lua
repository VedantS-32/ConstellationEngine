workspace "ConstellationEngine"
	architecture "x64"
	startproject "CStellObservatory"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root directory
IncludeDir = {}
IncludeDir["spdlog"] = "ConstellationCore/Vendor/spdlog/include"
IncludeDir["glfw"] = "ConstellationCore/Vendor/glfw/include"
IncludeDir["Glad"] = "ConstellationCore/Vendor/Glad/include"
IncludeDir["glm"] = "ConstellationCore/Vendor/glm"
IncludeDir["ImGui"] = "ConstellationCore/Vendor/imgui"
IncludeDir["stb_image"] = "ConstellationCore/Vendor/stb_image"
IncludeDir["entt"] = "ConstellationCore/Vendor/entt/include"
IncludeDir["yaml_cpp"] = "ConstellationCore/Vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "ConstellationCore/Vendor/ImGuizmo"
IncludeDir["assimp"] = "ConstellationCore/Vendor/assimp/include"

group "Dependencies"
	include "ConstellationCore/Vendor/glfw"
	include "ConstellationCore/Vendor/Glad"
	include "ConstellationCore/Vendor/imgui"
	include "ConstellationCore/Vendor/ImGuizmo"
	include "ConstellationCore/Vendor/yaml-cpp"
	include "ConstellationCore/Vendor/assimp"
group ""

project "ConstellationCore"
	location "ConstellationCore"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	flags { "MultiProcessorCompile" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "CStellpch.h"
	pchsource "%{prj.name}/Source/CStellpch.cpp"

	files {
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/stb_image/**.h",
		"%{prj.name}/Vendor/stb_image/**.cpp",
	}

	includedirs {
		"%{prj.name}/Source",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}"
	}

	links {
		"glfw",
		"Glad",
		"imgui",
		"ImGuizmo",
		"yaml-cpp",
		"assimp"
	}

	postbuildcommands {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/CStellObservatory")
    }

	filter "system:windows"
		systemversion "latest"
		defines {
			"CSTELL_PLATFORM_WINDOWS",
			"CSTELL_DYNAMIC_LINK",
			"CSTELL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		links {
			"Gdi32.lib",
			"User32.lib",
			"Shell32.lib",
			"Comdlg32.lib",
			"opengl32.lib"
		}
	
	filter "system:linux"
		defines {
			"CSTELL_PLATFORM_LINUX",
			"CSTELL_DYNAMIC_LINK",
			"CSTELL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "system:macosx"
		defines {
			"CSTELL_PLATFORM_MACOSX",
			"CSTELL_DYNAMIC_LINK",
			"CSTELL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "files:ConstellationCore/Vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "configurations:Debug"
		defines "CSTELL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CSTELL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CSTELL_DIST"
		runtime "Release"
		optimize "On"

project "CStellObservatory"
	location "CStellObservatory"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	flags { "MultiProcessorCompile" }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs {
		"ConstellationCore/Vendor/spdlog/include",
		"ConstellationCore/Source",
		"ConstellationCore",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links {
		"imgui",
		"ImGuizmo",
		"ConstellationCore"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"CSTELL_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		defines {
			"CSTELL_PLATFORM_LINUX"
		}

	filter "system:macosx"
		defines {
			"CSTELL_PLATFORM_MACOSX"
		}

	filter "configurations:Debug"
		defines "CSTELL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CSTELL_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CSTELL_DIST"
		runtime "Release"
		optimize "On"


-- project "Sandbox"
-- 	location "Sandbox"
-- 	kind "ConsoleApp"
-- 	language "C++"
-- 	cppdialect "C++20"
-- 	staticruntime "off"

-- 	targetdir ("bin/" .. outputdir .."/%{prj.name}")
-- 	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

-- 	files {
-- 		"%{prj.name}/Source/**.h",
-- 		"%{prj.name}/Source/**.cpp"
-- 	}

-- 	includedirs {
-- 		"ConstellationCore/Vendor/spdlog/include",
-- 		"ConstellationCore/Source",
-- 		"%{IncludeDir.glm}",
-- 		"%{IncludeDir.ImGui}",
-- 		"%{IncludeDir.entt}"
-- 	}

-- 	links {
-- 		"ConstellationCore"
-- 	}

-- 	filter "system:windows"
-- 		systemversion "latest"

-- 	defines {
-- 		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
-- 		"CSTELL_PLATFORM_WINDOWS"
-- 	}
	
-- 	filter "configurations:Debug"
-- 		defines "CSTELL_DEBUG"
-- 		runtime "Debug"
-- 		symbols "on"

-- 	filter "configurations:Release"
-- 		defines "CSTELL_RELEASE"
-- 		runtime "Release"
-- 		optimize "on"

-- 	filter "configurations:Dist"
-- 		defines "CSTELL_DIST"
-- 		runtime "Release"
-- 		optimize "on"