#pragma once
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "imgui_impl_opengl3.h"
#include <imgui.h>

#include "Application.h"
 
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR)))) 

namespace FoxSort {
    Application* Application::instance = nullptr;

    Application* Application::GetInstance() {
        if (instance == nullptr) {
            instance = new Application();
        }
        return instance;
    }

    Application::Application() {
        unsigned int init_flags{
        SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER
        };
        if (SDL_Init(init_flags) != 0) {
            m_exit_status = 1;
        }

        // Create new window with the title "Application".
        m_window = std::make_unique<Window>(
            Window::Settings{"Application"}
        );
    }

    Application::~Application() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_Quit();
    }

    int Application::Run() {
        if (m_exit_status == 1) {
            return m_exit_status;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io{ ImGui::GetIO() };


        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;                  // Enable docking
 

        ImGui_ImplSDL2_InitForSDLRenderer(
            m_window->get_native_window(),
            m_window->get_native_renderer()
        );
        ImGui_ImplSDLRenderer2_Init(
            m_window->get_native_renderer()
        );

        Style::SetImGuiStyle();
 
        m_running = true;
 

        while (m_running) {
            SDL_Event event{};
            while (SDL_PollEvent(&event) == 1) {
                ImGui_ImplSDL2_ProcessEvent(&event);

                if (event.type == SDL_QUIT) {
                    Shutdown();
                }
            }

            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();

            ImGui::NewFrame();
 

            UpdateGUI();

            ImGui::Render();

            SDL_SetRenderDrawColor(m_window->get_native_renderer(), 233, 100, 100, 255);
            SDL_RenderClear(m_window->get_native_renderer());


            Update();


             ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_window->get_native_renderer());
            SDL_RenderPresent(
                m_window->get_native_renderer()
            );
        }
 
        return m_exit_status;
    }

    void Application::Update() {

        int window_width{ 0 };
        int window_height{ 0 };
        SDL_GetWindowSize(
            m_window->get_native_window(),
            &window_width, &window_height
        );

        int square1_width = window_width * 0.3;
        int square2_width = window_width * 0.7;
        int square_height = window_height;

 

        // Малювання першого квадрату (червоний)
        SDL_Rect square1 = { 0, 0, square1_width, square_height };
        SDL_SetRenderDrawColor(m_window->get_native_renderer(), 255, 0, 0, 255);
        SDL_RenderFillRect(m_window->get_native_renderer(), &square1);

        //// Малювання другого квадрату (синій)
        //SDL_Rect square2 = { square1_width, 0, square2_width, square_height };
        //SDL_SetRenderDrawColor(m_window->get_native_renderer(), 0, 0, 255, 255);
        //SDL_RenderFillRect(m_window->get_native_renderer(), &square2);
    }

    void Application::UpdateGUI() {
        // Render "some panel".
        if (m_show_some_panel) {
            int window_width{ 0 };
            int window_height{ 0 };
            SDL_GetWindowSize(
                m_window->get_native_window(),
                &window_width, &window_height
            );

            int panel_width = static_cast<int>(window_width * 0.3);
            int panel_height = window_height;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(panel_width, panel_height));
            ImGui::Begin("Some panel", &m_show_some_panel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            ImGui::Text("Hello World");
            static float values[90] = {};
            static int values_offset = 0;
            static double refresh_time = 0.0;
            if ( refresh_time == 0.0)
                refresh_time = ImGui::GetTime();
            while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
            {
                static float phase = 0.0f;
                values[values_offset] = cosf(phase);
                values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
                phase += 0.10f * values_offset;
                refresh_time += 1.0f / 60.0f;
            }
            {
                float average = 0.0f;
                for (int n = 0; n < IM_ARRAYSIZE(values); n++)
                    average += values[n];
                average /= (float)IM_ARRAYSIZE(values);
                char overlay[32];
                sprintf(overlay, "avg %f", average);
                ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
            }


            ImGui::End();
        }
    }

    void Application::Draw(std::vector<int>& v, SDL_Renderer* renderer, unsigned int red, unsigned int blue)
    {
        int index = 0;
        for (int i : v) {
            if (index == red)
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else if (index == blue)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

            SDL_RenderDrawLine(renderer, index, 99, index, i);

            index += 1;
        }
    }

    void Application::Shutdown() {
        m_running = false;
    }
}   