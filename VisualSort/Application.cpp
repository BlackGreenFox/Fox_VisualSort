#pragma once
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

#include "Application.h"
 

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
 
 
            UpdateGUI();
            Update();
        }
 
        return m_exit_status;
    }

    void Application::Update() {
      /*  SDL_SetRenderDrawColor(
            m_window->get_native_renderer(),
            100, 100, 100, 255
        );

        SDL_RenderClear(m_window->get_native_renderer());*/
        // Render data through the SDL renderer
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_window->get_native_renderer());
        SDL_RenderPresent(m_window->get_native_renderer());
    }

    void Application::UpdateGUI() {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();


        // Render "some panel".
        if (m_show_some_panel) {
            ImGui::Begin("Some panel", &m_show_some_panel);
            ImGui::Text("Hello World");
            ImGui::End();
        }

        ImGui::Render();
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