#include <iostream>
#include <random>
#include <ranges>
#include <algorithm>
#include <SDL.h>

// GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// Dear ImGui
#include "imgui.h"
#include <imgui_impl_glfw.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h" 
#include "Application.h"

int main(int argc, char* argv[]) {
    FoxSort::Application app{};
    return app.Run();
}
 