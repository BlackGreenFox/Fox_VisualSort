workspace "FoxVisualSort"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "VisualSort"

   filter "system:windows"
   buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
 
IncludeDir = {}
IncludeDir["GLFW"] = "Include//GLFW/include"
IncludeDir["ImGui"] = "Include/imgui"
IncludeDir["Glad"] = "Include/Glad/include"
IncludeDir["SDL2"] = "Include/SDL2/include"

-- Projects
group "Dependencies"
	include "App/Include/GLFW"
	include "App/Include/imgui"
    include "App/Include/Glad"
	include "App/Include/SDL2/SDL2.lua"
	include "App/Include/SDL2/SDL2main.lua"
group ""

include "App/VisualSort-Build.lua"
