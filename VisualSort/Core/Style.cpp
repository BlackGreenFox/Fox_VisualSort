#include "Style.h"

namespace FoxSort {
    void Style::SetImGuiStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Change the background color of the window
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Dark gray

        // Change the color of the title bar
        colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Slightly lighter gray
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // Lighter gray when active

        // Change the color of the frame background (used for most widgets)
        colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Slightly lighter gray

        // Change the color of the button
        colors[ImGuiCol_Button] = ImVec4(0.2f, 0.3f, 0.4f, 1.0f); // Blue-ish
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.4f, 0.5f, 1.0f); // Lighter blue when hovered
        colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.2f, 0.3f, 1.0f); // Darker blue when active

        // Change the color of the text
        colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
    }
};
