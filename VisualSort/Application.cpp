#pragma once
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui.h>

#include "Application.hpp"
 

namespace App {

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

        s_window = std::make_unique<Window>(
            Window::Settings{"Sort", 350, 350}
        );
    }

    Application::~Application() {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        SDL_Quit();
    }

    int App::Application::run() {
        if (m_exit_status == 1) {
            return m_exit_status;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io{ ImGui::GetIO() };

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
 

        ImGui_ImplSDL2_InitForSDLRenderer(
            m_window->get_native_window(),
            m_window->get_native_renderer()
        );
        ImGui_ImplSDLRenderer2_Init(
            m_window->get_native_renderer()
        );



        bool sort = true;
        m_running = true;

        while (m_running) {
            SDL_Event event{};
            while (SDL_PollEvent(&event) == 1) {
                ImGui_ImplSDL2_ProcessEvent(&event);

                if (event.type == SDL_QUIT) {
                    stop();
                }
            }

            ImGui_ImplSDLRenderer2_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();



            // Render "some panel".
            if (m_show_some_panel) {
                ImGui::Begin("Some panel", &m_show_some_panel);
                ImGui::Text("Hello World");
                ImGui::End();
            }

            if (sort) {
                ImGui::Begin("Sorting Panel", &sort);

                static std::vector<int> sortingArray; // Ваш массив для сортировки
                static int currentIndex = 0;
                static int nextIndex = 1;
                static bool swapped = false;

                // Инициализация массива случайными числами (выполните это только один раз)
                if (sortingArray.empty()) {
                    std::random_device rd;
                    std::uniform_int_distribution<int> d(1, 99);

                    for (int i = 0; i < 100; i++) {
                        sortingArray.push_back(d(rd));
                    }
                }

                // Визуализация сортировки
                if (currentIndex < sortingArray.size()) {
                    if (sortingArray[currentIndex] > sortingArray[nextIndex]) {
                        std::swap(sortingArray[currentIndex], sortingArray[nextIndex]);
                        swapped = true;
                    }

                    for (int i = 0; i < sortingArray.size(); i++) {
                        if (i == currentIndex || i == nextIndex) {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // Синий цвет для текущих элементов
                        }
                        else {
                            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 1.0f)); // Серый цвет для остальных элементов
                        }

                        ImGui::Button(std::to_string(sortingArray[i]).c_str(), ImVec2(30, sortingArray[i]));
                        if (i < sortingArray.size() - 1) {
                            ImGui::SameLine();
                        }

                        ImGui::PopStyleColor();
                    }

                    if (swapped) {
                        swapped = false;
                        nextIndex++;
                    }

                    if (nextIndex >= sortingArray.size() - currentIndex) {
                        currentIndex++;
                        nextIndex = 0;
                    }
                }
                else {
                    sort = false;
                }

              
                ImGui::End();
            }

            ImGui::Render();

            SDL_SetRenderDrawColor(
                m_window->get_native_renderer(),
                100, 100, 100, 255
            );
            SDL_RenderClear(m_window->get_native_renderer());
 
            ImGui_ImplSDLRenderer2_RenderDrawData(
                ImGui::GetDrawData()
            );
            SDL_RenderPresent(
                m_window->get_native_renderer()
            );
 
    
        /*    if (sort)
            {
                std::random_device rd;
                std::uniform_int_distribution<int> d(1, 99);
                std::vector<int> v;


                bool quit = false;
                SDL_Event sort_event;
                SDL_RenderSetScale(m_window->get_native_renderer(), 5, 5);

                for (int i = 0; i < 100; i++)
                {
                    v.push_back(d(rd));
                }
                for (size_t i = 0; i < v.size(); i++)
                {
                    for (size_t j = 0; j < v.size(); j++)
                    {
                        while (SDL_PollEvent(&sort_event))
                        {
                            if (sort_event.type == SDL_QUIT)
                            {
                                quit = true;
                                break;
                            }
                        }

                        if (quit)
                            break;

                        if (v[j] > v[i])
                        {
                            std::swap(v[j], v[i]);
                        }
                        SDL_SetRenderDrawColor(m_window->get_native_renderer(), 0, 0, 0, 255);
                        SDL_RenderClear(m_window->get_native_renderer());

                        Draw_State(v, m_window->get_native_renderer(), i, j);

                        SDL_RenderPresent(m_window->get_native_renderer());
                        SDL_Delay(2);
                    }
                }
                sort = false;
            }*/

        }

        return m_exit_status;
    }

    void App::Application::stop() {
        m_running = false;
    }

    void App::Application::Draw_State(std::vector<int>& v, SDL_Renderer* renderer, unsigned int red, unsigned int blue)
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

}  // namespace App