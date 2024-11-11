#pragma once
#include <iostream>
#include <memory>
#include <random>
#include <ranges>
#include <algorithm>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "imgui_impl_opengl3.h"
#include <imgui.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Core/Window.h"
#include "Core/Style.h"
 
#include "Core/Sort/BubbleSort.h"
#include "Core/Sort/InsertionSort.h"


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
		void OnEvent(const SDL_Event& event);

		// Panels ImGUI
		void ShowSorterControlPanel();
		void ShowTipsOverlayPanel();
		void ShowSorterIndicatorPanel();
		void ShowLoaderScreenPanel();







		// Render Functions
		SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer);
		void RenderText(SDL_Renderer* renderer, const std::string& text, int size, int x, int y, int max_width);
		
		
		// Sorting related variables
 
		int current_index = 1;
		std::vector<float> search_times;

		void TogglePauseSorting();
		void SetSortingDelay(int delay);  


		void ShowStylePanel(ImGuiStyle* ref);

	private:
		static Application* instance;
		bool m_show_tip_panel{ true };



		bool m_show_some_panel{ NULL };
		int m_exit_status{0};
		bool m_running{ true };



		std::unique_ptr<Window> m_window{nullptr};

		

		std::vector<std::string> asciiFrames;
		SDL_Texture* left_image = nullptr;
		TTF_Font* font = nullptr;

		std::vector<int> values;
		std::vector<int> values2;

		std::unique_ptr<SortBase> sorter;
		std::unique_ptr<SortBase> sorter2;

		bool is_sorting_paused;
	    bool compare_sorting{ false };
		int delay;  // Add this line

		// Add buffer for plotting
		static const int plot_buffer_size = 100;
		std::vector<float> plot_values;
		int plot_values_offset;
	};

} 