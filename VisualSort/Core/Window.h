#pragma once

#include <string>
#include "SDL.h"

namespace FoxSort {

    class Window {
    public:
        struct Settings {
            std::string title;
            int width{ 1280 };
            int height{ 720 };
        };

        explicit Window(const Settings& settings);
        ~Window();

        [[nodiscard]] float get_scale() const;
        [[nodiscard]] SDL_Window* get_native_window() const;
        [[nodiscard]] SDL_Renderer* get_native_renderer() const;

    private:
        SDL_Window* m_window{ nullptr };
        SDL_Renderer* m_renderer{ nullptr };
    };

}  // namespace App