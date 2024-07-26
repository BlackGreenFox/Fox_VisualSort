#include "Style.h"

namespace FoxSort {

    // Helper function to multiply ImVec4 by a scalar
    ImVec4 MultiplyImVec4(const ImVec4& color, float scalar) {
        return ImVec4(color.x * scalar, color.y * scalar, color.z * scalar, color.w * scalar);
    }

    void Style::SetImGuiStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // New custom color #49c454
        ImVec4 custom_color = (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f); // RGB to ImVec4

        // Change the background color of the window
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Dark gray

        // Change the color of the title bar
        colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Slightly lighter gray
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // Lighter gray when active

        // Change the color of the frame background (used for most widgets)
        colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); // Slightly lighter gray
        colors[ImGuiCol_FrameBgHovered] = MultiplyImVec4(custom_color, 1.1f); // Slightly brighter on hover
        colors[ImGuiCol_FrameBgActive] = MultiplyImVec4(custom_color, 0.9f); // Slightly darker when active

        // Change the color of the button
        colors[ImGuiCol_Button] = custom_color; // Custom color
        colors[ImGuiCol_ButtonHovered] = MultiplyImVec4(custom_color, 1.1f); // Slightly brighter on hover
        colors[ImGuiCol_ButtonActive] = MultiplyImVec4(custom_color, 0.9f); // Slightly darker when active

        // Change the color of the text
        colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White

        // Change the color of the Combo box
        colors[ImGuiCol_Header] = custom_color;
        colors[ImGuiCol_HeaderHovered] = MultiplyImVec4(custom_color, 1.1f);
        colors[ImGuiCol_HeaderActive] = MultiplyImVec4(custom_color, 0.9f);

        // Change the color of the SliderInt
        colors[ImGuiCol_SliderGrab] = custom_color;
        colors[ImGuiCol_SliderGrabActive] = MultiplyImVec4(custom_color, 0.9f);

        // Change the color of the text field
        colors[ImGuiCol_TextSelectedBg] = MultiplyImVec4(custom_color, 0.6f);
    }
}
