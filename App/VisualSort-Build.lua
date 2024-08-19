project "VisualSort"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { 
    "Source/**.h", 
    "Source/**.cpp",
    "Source/imgui/**.h", 
    "Include/imgui/**.cpp"
    }

   includedirs
   {   
      "Source",
      "Source/**",
      "Include", 
      "Assets",   
      "Resources",
	  "%{IncludeDir.GLFW}",
	  "%{IncludeDir.Glad}",
	  "%{IncludeDir.ImGui}",
	  "%{IncludeDir.SDL2}",
   }

 

	links 
	{ 
		"GLFW",
		"ImGui",
        "SDL2",
        "SDL2main",
        "SDL2",
	}

   defines
   {
       "_CRT_SECURE_NO_WARNINGS"
   }


   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   
   filter "system:windows"
       systemversion "latest"
       defines { 
         "WINDOWS",
         "GLCORE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
       }
    
   filter "configurations:Debug"
       defines { 
         "DEBUG",
         "GLCORE_DEBUG"
       }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"
 