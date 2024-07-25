#pragma once
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "imgui_impl_opengl3.h"
#include <imgui.h>

#include <SDL.h>
#include <SDL_image.h>

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

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
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
        IMG_Quit(); 
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
 
        left_image = LoadTexture("Assets/left_image.png", m_window->get_native_renderer());

       
 
 

        // Initialize sorting algorithm
        delay = 300;  // Set default delay to 100 ms
        sorter = std::make_unique<BubbleSort>();
        values.resize(100);
        for (int i = 0; i < 100; ++i) {
            values[i] = i + 1;  
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(values.begin(), values.end(), g);

        sorter->Init(values);
        is_sorting_paused = false;
        plot_values = std::vector<float>(plot_buffer_size, 0.0f);
        plot_values_offset = 0;



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

            Update();

            // Then, update the GUI
            UpdateGUI();

            ImGui::Render();

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
        SDL_GetWindowSize(m_window->get_native_window(), &window_width, &window_height);

        int margin = 10;
        int square1_width = window_width * 0.3;
        int square2_width = window_width * 0.7;
        int square_height = window_height;

        // Draw the first rectangle (red)
        SDL_Rect square1 = { 0, 0, square1_width, square_height };
        SDL_SetRenderDrawColor(m_window->get_native_renderer(), 255, 0, 0, 255);
        SDL_RenderFillRect(m_window->get_native_renderer(), &square1);

        // Desired texture size
        const int target_texture_width = 300;
        const int target_texture_height = 300;

        // Calculate the position to center the texture within the red rectangle
        int x = (square1_width - target_texture_width) / 2;
        int y = (square_height - target_texture_height) / 2;

        SDL_Rect dstrect = { x, y, target_texture_width, target_texture_height };
        SDL_RenderCopy(m_window->get_native_renderer(), left_image, nullptr, &dstrect);

        // Draw the second rectangle (sort zone)
        SDL_Rect square2 = { square1_width, 0, square2_width, square_height };
        SDL_SetRenderDrawColor(m_window->get_native_renderer(), 233, 100, 100, 255);
        SDL_RenderFillRect(m_window->get_native_renderer(), &square2);

        // Update sorting and draw lines
        if (!is_sorting_paused) {
            Uint32 current_time = SDL_GetTicks();
            if (current_time - sorter->GetLastStep() >= delay) {
                sorter->Step();
                plot_values[plot_values_offset] = sorter->GetComparisonState();
                plot_values_offset = (plot_values_offset + 1) % plot_buffer_size;
            }
        }
        sorter->Update(m_window->get_native_renderer(), square1_width + margin, margin, square2_width - 2 * margin, square_height - 2 * margin);
    }

    void Application::UpdateGUI() {
        // Render "some panel".
        if (m_show_some_panel) {
            int window_width{ 0 };
            int window_height{ 0 };
            SDL_GetWindowSize(m_window->get_native_window(), &window_width, &window_height);

            int panel_width = static_cast<int>(window_width * 0.3);
            int panel_height = window_height;

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(panel_width, panel_height));
            ImGui::Begin("Some panel", &m_show_some_panel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

            ImGui::Text("Hello World");

            // Plot the comparison state
            ImGui::PlotLines("Sorting State", plot_values.data(), plot_buffer_size, plot_values_offset, nullptr, 0.0f, values.size(), ImVec2(300.0f, 80.0f));
            static std::vector<float> big_o_plot;
            if (big_o_plot.size() != values.size()) {
                big_o_plot.resize(values.size());
                for (int i = 0; i < values.size(); ++i) {
                    big_o_plot[i] = static_cast<float>(i * i); // O(n^2) for Bubble Sort
                }
            }
            ImGui::PlotLines("Big O Notation (O(n^2))", big_o_plot.data(), big_o_plot.size(), 0, nullptr, 0.0f, big_o_plot.back(), ImVec2(300.0f, 300.0f));

            // Add a button to toggle sorting pause
            if (ImGui::Button("Pause/Resume Sorting")) {
                TogglePauseSorting();
            }

            ImGui::End();
        }
    }

    void Application::TogglePauseSorting() {
        if (sorter) {
            sorter->Pause();
        }
        is_sorting_paused = !is_sorting_paused;
    }

    void Application::SetSortingDelay(int new_delay) {
        delay = new_delay;
        if (sorter) {
            sorter->SetDelay(delay);
        }
    }

    void Application::Shutdown() {
        m_running = false;
    }

        
    SDL_Texture* Application::LoadTexture(const std::string& path, SDL_Renderer* renderer) {
        SDL_Texture* newTexture = nullptr;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == nullptr) {
            return NULL;
        }
        else {
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface);
        }
        return newTexture;
    }
}   