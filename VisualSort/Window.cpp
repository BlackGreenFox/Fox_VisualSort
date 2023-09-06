#include "Window.hpp"

namespace App {

    Window::Window(const Settings& settings) {
        auto window_flags{
          static_cast<SDL_WindowFlags>(
            SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
          )
        };
        constexpr int window_center_flag{ SDL_WINDOWPOS_CENTERED };

        m_window = SDL_CreateWindow(
            settings.title.c_str(),
            window_center_flag,
            window_center_flag,
            settings.width,
            settings.height,
            window_flags
        );

        auto renderer_flags{
          static_cast<SDL_RendererFlags>(
            SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
          )
        };
        m_renderer = SDL_CreateRenderer(
            m_window, -1, renderer_flags
        );

        if (m_renderer == nullptr) {
            return;
        }
    }

    Window::~Window() {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    SDL_Window* Window::get_native_window() const {
        return m_window;
    }

    SDL_Renderer* Window::get_native_renderer() const {
        return m_renderer;
    }

}  // namespace App