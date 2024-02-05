workspace "DevTools"
	architecture "x64"

	configurations 
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.architecture}";

	IncludeDir = {}
	IncludeDir["SDL2"] = "DevTools/vendor/SDL2/include";
	IncludeDir["SDL2_mixer"] = "DevTools/vendor/SDL2_mixer/include";
	IncludeDir["SDL2_ttf"] = "DevTools/vendor/SDL_ttf/include";
	IncludeDir["entt"] = "DevTools/vendor/entt/include";
	IncludeDir["glm"] = "DevTools/vendor/glm";

	LibraryDir = {}
	LibraryDir["SDL2"] = "vendor/SDL2/lib/x64/SDL2.lib";
	LibraryDir["SDL2main"] = "vendor/SDL2/lib/x64/SDL2main.lib";
	LibraryDir["SDL2image"] = "vendor/SDL2/lib/x64/SDL2_image.lib";
	LibraryDir["SDL2mixer"] = "vendor/SDL2_mixer/lib/x64/SDL2_mixer.lib";
	LibraryDir["SDL2ttf"] = "vendor/SDL2_ttf/lib/x64/SDL2_ttf.lib";

project "DevTools"
	location "DevTools"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.SDL2_mixer}",
		"%{IncludeDir.SDL2_ttf}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
	}

	libdirs
	{
		"%{prj.name}/vendor/SDL2/lib/x64",
		"%{prj.name}/vendor/SDL2_mixer/lib/x64",
		"%{prj.name}/vendor/SDL_ttf/lib/x64"
	}

	links
	{
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_mixer.lib",
		"SDL2_ttf.lib",
		"libfreetype-6.dll",
		"SDL2.dll",
		"SDL2_image.dll",
		"SDL2_mixer.dll",
		"SDL2_ttf.dll",
		"zlib1.dll"
	}

	filter "configurations:Debug"
		defines "GAME_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "GAME_RELEASE"
		runtime "Release"
		optimize "On"