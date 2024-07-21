#pragma once

#include <SDL.h>
#include <memory>
#include <random>
#include <ranges>
#include <algorithm>

#include "Core/Window.h"
#include "Core/Style.h"

namespace FoxSort {

	class Application {
	public:
		Application();
		~Application();
		static Application* GetInstance();

		int Run();
		void Update();
		void UpdateGUI();
		void Shutdown();
		void Draw(std::vector<int>& v, SDL_Renderer* renderer, unsigned int red, unsigned int blue);

	private:
		static Application* instance;
		
		int m_exit_status{0};
		bool m_running{ true };

		std::unique_ptr<Window> m_window{nullptr};

		bool m_show_some_panel{ true };
	};

} 