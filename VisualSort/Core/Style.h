#pragma once
#include "imgui.h"

#include "imgui_impl_sdl2.h"

namespace FoxSort {
	class Style
	{
	public:
        static float MainColor[4];

        static float WindowBg[4];
        static float TitleBg[4];
        static float TitleBgActive[4];
        static float FrameBg[4];
        static float FrameBgHovered[4];
        static float FrameBgActive[4];
        static float Button[4];
        static float ButtonHovered[4];
        static float ButtonActive[4];
        static float Text[4];
        static float Header[4];
        static float HeaderHovered[4];
        static float HeaderActive[4];
        static float SliderGrab[4];
        static float SliderGrabActive[4];
        static float TextSelectedBg[4];

        static float SortLine[4];
        static float SortCurrentLine[4];
        static float SortGradienStartLine[4];
        static float SortGradienEndLine[4];
        static float SortComplete[4];

        static bool open_panel;
        static bool Gradient;

        static void Initialize();   
		static void ShowStylePanel();
		static void SetImGuiStyle();
	};
}
