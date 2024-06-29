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
IncludeDir["glfw"] = "ConstellationCore/vendor/glfw/include"
IncludeDir["Glad"] = "ConstellationCore/vendor/Glad/include"
IncludeDir["glm"] = "ConstellationCore/vendor/glm"
IncludeDir["ImGui"] = "ConstellationCore/vendor/imgui"
IncludeDir["stb_image"] = "ConstellationCore/vendor/stb_image"
IncludeDir["entt"] = "ConstellationCore/vendor/entt/include"
IncludeDir["yaml_cpp"] = "ConstellationCore/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "ConstellationCore/vendor/ImGuizmo"
IncludeDir["assimp"] = "ConstellationCore/vendor/assimp/include"

group "Dependencies"
	include "ConstellationCore/vendor/glfw"
	include "ConstellationCore/vendor/Glad"
	include "ConstellationCore/vendor/imgui"
	include "ConstellationCore/vendor/ImGuizmo"
	include "ConstellationCore/vendor/yaml-cpp"
	include "ConstellationCore/vendor/assimp"
group ""

project "ConstellationCore"
	location "ConstellationCore"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .."/%{prj.name}")
	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

	defines {
		"CSTELL_PLATFORM_WINDOWS",
		"CSTELL_DYNAMIC_LINK",
		"CSTELL_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
	}
	
	pchheader "CStellpch.h"
	pchsource "%{prj.name}/src/CStellpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		-- "%{prj.name}/vendor/spdlog/include/spdlog/spdlog.h",
		-- "%{prj.name}/vendor/spdlog/include/spdlog/spdlog-inl.h",
		-- "%{prj.name}/vendor/spdlog/include/spdlog/fmt/ostr.h",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		-- "%{prj.name}/vendor/glm/glm/**.hpp",
		-- "%{prj.name}/vendor/glm/glm/**.inl",
		-- "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		-- "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
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

	links
	{
		"glfw",
		"Glad",
		"imgui",
		"ImGuizmo",
		"yaml-cpp",
		"assimp",
		"Gdi32.lib",
        "User32.lib",
        "Shell32.lib",
		"Comdlg32.lib",
		"opengl32.lib"
	}

	postbuildcommands{
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/CStellObservatory")
    }

	filter "files:ConstellationCore/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }
	
	filter "system:windows"
		systemversion "latest"
	
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


-- project "Sandbox"
-- 	location "Sandbox"
-- 	kind "ConsoleApp"
-- 	language "C++"
-- 	cppdialect "C++20"
-- 	staticruntime "off"

-- 	targetdir ("bin/" .. outputdir .."/%{prj.name}")
-- 	objdir ("bin-int/" .. outputdir .."/%{prj.name}")

-- 	files {
-- 		"%{prj.name}/src/**.h",
-- 		"%{prj.name}/src/**.cpp"
-- 	}

-- 	includedirs {
-- 		"ConstellationCore/vendor/spdlog/include",
-- 		"ConstellationCore/src",
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

project "CStellObservatory"
	location "CStellObservatory"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
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