#include "Style.h"
#include <iostream>

namespace FoxSort {
  
    void MultiplyFloatArray(float* result, const float* color, float scalar) {
        for (int i = 0; i < 4; ++i) {
            result[i] = color[i] * scalar;
        }
    }

    void SetColor(float* color, float r, float g, float b, float a) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }

    void SetColor(float* color, float r, float g, float b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    float Style::MainColor[4];
    float Style::WindowBg[4];
    float Style::TitleBg[4];
    float Style::TitleBgActive[4];
    float Style::FrameBg[4];
    float Style::FrameBgHovered[4];
    float Style::FrameBgActive[4];
    float Style::Button[4];
    float Style::ButtonHovered[4];
    float Style::ButtonActive[4];
    float Style::Text[4];
    float Style::Header[4];
    float Style::HeaderHovered[4];
    float Style::HeaderActive[4];
    float Style::SliderGrab[4];
    float Style::SliderGrabActive[4];
    float Style::TextSelectedBg[4];
    float Style::SortLine[4];
    float Style::SortCurrentLine[4];
    float Style::SortComplete[4];
    float Style::SortGradienStartLine[4];
    float Style::SortGradienEndLine[4];

    bool Style::Gradient;
    bool Style::open_panel{ false };

    void Style::Initialize() {
        SetColor(MainColor, 0.342857f, 0.6f, 0.24f, 1.0f);
  

        SetColor(WindowBg, 0.1f, 0.1f, 0.1f, 1.0f);
        SetColor(TitleBg, 0.2f, 0.2f, 0.2f, 1.0f);
        SetColor(TitleBgActive, 0.3f, 0.3f, 0.3f, 1.0f);
        SetColor(FrameBg, 0.2f, 0.2f, 0.2f, 1.0f);

        MultiplyFloatArray(FrameBgHovered, MainColor, 1.1f);
        MultiplyFloatArray(FrameBgActive, MainColor, 0.9f);
        SetColor(Button, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);
        MultiplyFloatArray(ButtonHovered, MainColor, 1.1f);
        MultiplyFloatArray(ButtonActive, MainColor, 0.9f);
        SetColor(Text, 1.0f, 1.0f, 1.0f, 1.0f);
        SetColor(Header, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);
        MultiplyFloatArray(HeaderHovered, MainColor, 1.1f);
        MultiplyFloatArray(HeaderActive, MainColor, 0.9f);
        SetColor(SliderGrab, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);
        MultiplyFloatArray(SliderGrabActive, MainColor, 0.9f);
        MultiplyFloatArray(TextSelectedBg, MainColor, 0.6f);

        SetColor(SortLine, 1.0f, 1.0f, 1.0f, 1.0f); // White
        SetColor(SortGradienStartLine, 1.0f, 1.0f, 1.0f, 1.0f); // White
        SetColor(SortGradienEndLine, 0.0f, 0.0f, 0.0f, 1.0f); // Black
        SetColor(SortCurrentLine, 99 / 255.0f, 220 / 255.0f, 66 / 255.0f, 1.0f); // Highlight the current sorting elements
        SetColor(SortComplete, 144 / 255.0f, 238 / 255.0f, 144 / 255.0f, 1.0f); // Light green when sorted

        Gradient = false;
    }

    void Style::SetImGuiStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        colors[ImGuiCol_WindowBg] = ImVec4(WindowBg[0], WindowBg[1], WindowBg[2], WindowBg[3]);
        colors[ImGuiCol_TitleBg] = ImVec4(TitleBg[0], TitleBg[1], TitleBg[2], TitleBg[3]);
        colors[ImGuiCol_TitleBgActive] = ImVec4(TitleBgActive[0], TitleBgActive[1], TitleBgActive[2], TitleBgActive[3]);
        colors[ImGuiCol_FrameBg] = ImVec4(FrameBg[0], FrameBg[1], FrameBg[2], FrameBg[3]);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(FrameBgHovered[0], FrameBgHovered[1], FrameBgHovered[2], FrameBgHovered[3]);
        colors[ImGuiCol_FrameBgActive] = ImVec4(FrameBgActive[0], FrameBgActive[1], FrameBgActive[2], FrameBgActive[3]);
        colors[ImGuiCol_Button] = ImVec4(Button[0], Button[1], Button[2], Button[3]);
        colors[ImGuiCol_ButtonHovered] = ImVec4(ButtonHovered[0], ButtonHovered[1], ButtonHovered[2], ButtonHovered[3]);
        colors[ImGuiCol_ButtonActive] = ImVec4(ButtonActive[0], ButtonActive[1], ButtonActive[2], ButtonActive[3]);
        colors[ImGuiCol_Text] = ImVec4(Text[0], Text[1], Text[2], Text[3]);
        colors[ImGuiCol_Header] = ImVec4(Header[0], Header[1], Header[2], Header[3]);
        colors[ImGuiCol_HeaderHovered] = ImVec4(HeaderHovered[0], HeaderHovered[1], HeaderHovered[2], HeaderHovered[3]);
        colors[ImGuiCol_HeaderActive] = ImVec4(HeaderActive[0], HeaderActive[1], HeaderActive[2], HeaderActive[3]);
        colors[ImGuiCol_SliderGrab] = ImVec4(SliderGrab[0], SliderGrab[1], SliderGrab[2], SliderGrab[3]);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(SliderGrabActive[0], SliderGrabActive[1], SliderGrabActive[2], SliderGrabActive[3]);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(TextSelectedBg[0], TextSelectedBg[1], TextSelectedBg[2], TextSelectedBg[3]);       
    }


    void Style::ShowStylePanel() {
        if (!open_panel) 
            return;





        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoDocking;
 
        
        ImGui::Begin("Style Control Panel", &open_panel, window_flags);
        ImGuiStyle& style = ImGui::GetStyle();
     
       

        // Style
        ImGui::SeparatorText("Sort Line Colors:");
        const char* items[] = { "Single Color", "Gradient" };
        static int item_current = Gradient ? 1 : 0;

        if (ImGui::Combo("Lines Style", &item_current, items, IM_ARRAYSIZE(items))) {
            Gradient = (item_current == 1);
        }

  
        if (Gradient)
        {
            ImGui::ColorEdit3("SortGradienStartLine", SortGradienStartLine);
            ImGui::ColorEdit3("SortGradienEndLine", SortGradienEndLine);
        }
        else
        {
            ImGui::ColorEdit3("SortLine", SortLine);
        }

        ImGui::ColorEdit3("SortCurrentLine", SortCurrentLine);
        ImGui::ColorEdit3("SortComplete", SortComplete);

        ImGui::SeparatorText("Main Color:");
        if (ImGui::ColorEdit4("Main Color", MainColor))
        {
            MultiplyFloatArray(FrameBgHovered, MainColor, 1.1f);
            MultiplyFloatArray(FrameBgActive, MainColor, 0.9f);
            SetColor(Button, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);
            MultiplyFloatArray(ButtonHovered, MainColor, 1.1f);
            MultiplyFloatArray(ButtonActive, MainColor, 0.9f);
            SetColor(Text, 1.0f, 1.0f, 1.0f, 1.0f);
            SetColor(Header, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);
            MultiplyFloatArray(HeaderHovered, MainColor, 1.1f);
            MultiplyFloatArray(HeaderActive, MainColor, 0.9f);
            SetColor(SliderGrab, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);
            MultiplyFloatArray(SliderGrabActive, MainColor, 0.9f);
            MultiplyFloatArray(TextSelectedBg, MainColor, 0.6f);
            SetColor(SortCurrentLine, MainColor[0], MainColor[1], MainColor[2], MainColor[3]);  
            MultiplyFloatArray(SortComplete, MainColor, 0.6f);

            SetImGuiStyle();
        }
        
        ImGui::SeparatorText("Other Colors:");
        if (ImGui::ColorEdit4("WindowBg", WindowBg)) SetImGuiStyle();
        if (ImGui::ColorEdit4("TitleBg", TitleBg)) SetImGuiStyle();
        if (ImGui::ColorEdit4("TitleBgActive", TitleBgActive)) SetImGuiStyle();
        if (ImGui::ColorEdit4("FrameBg", FrameBg)) SetImGuiStyle();
        if (ImGui::ColorEdit4("FrameBgHovered", FrameBgHovered)) SetImGuiStyle();
        if (ImGui::ColorEdit4("FrameBgActive", FrameBgActive)) SetImGuiStyle();
        if (ImGui::ColorEdit4("Button", Button)) SetImGuiStyle();
        if (ImGui::ColorEdit4("ButtonHovered", ButtonHovered)) SetImGuiStyle();
        if (ImGui::ColorEdit4("ButtonActive", ButtonActive)) SetImGuiStyle();
        if (ImGui::ColorEdit4("Text", Text)) SetImGuiStyle();
        if (ImGui::ColorEdit4("Header", Header)) SetImGuiStyle();
        if (ImGui::ColorEdit4("HeaderHovered", HeaderHovered)) SetImGuiStyle();
        if (ImGui::ColorEdit4("HeaderActive", HeaderActive)) SetImGuiStyle();
        if (ImGui::ColorEdit4("SliderGrab", SliderGrab)) SetImGuiStyle();
        if (ImGui::ColorEdit4("SliderGrabActive", SliderGrabActive)) SetImGuiStyle();
        if (ImGui::ColorEdit4("TextSelectedBg", TextSelectedBg)) SetImGuiStyle();
 

 



        ImGui::End();
        
    }
}
