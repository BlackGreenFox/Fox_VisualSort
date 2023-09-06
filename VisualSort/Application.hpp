#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include "Window.hpp"
#include <random>
#include <ranges>
#include <algorithm>

namespace App {

	class Application {
	public:
		Application();
		~Application();

		int run();
		void stop();
		void Draw_State(std::vector<int>& v, SDL_Renderer* renderer, unsigned int red, unsigned int blue);

	private:
		int m_exit_status{0};
		bool m_running{ true };

		std::unique_ptr<Window> m_window{nullptr};
		std::unique_ptr<Window> s_window{nullptr};

		bool m_show_some_panel{ true };
	};

} 