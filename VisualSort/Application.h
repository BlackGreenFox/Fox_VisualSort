#pragma once

#include <SDL.h>
#include <memory>
#include <random>
#include <ranges>
#include <algorithm>

#include "Core/Window.h"
#include "Core/Style.h"
 
#include "Core/Sort/BubbleSort.h"

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


		void InitSort();
		void DrawArray(const std::vector<int>& array, SDL_Renderer* renderer, int red_index, int blue_index);

 
		SDL_Texture* LoadTexture(const std::string& path, SDL_Renderer* renderer);
		// Sorting related variables
 
		int current_index = 1;
		std::vector<float> search_times;

		void TogglePauseSorting();
		void SetSortingDelay(int delay);  

	private:
		static Application* instance;
		
		int m_exit_status{0};
		bool m_running{ true };

		std::unique_ptr<Window> m_window{nullptr};

		bool m_show_some_panel{ true };


		SDL_Texture* left_image = nullptr;
 

		std::vector<int> values;
		std::unique_ptr<SortBase> sorter;
		bool is_sorting_paused;
		int delay;  // Add this line

		// Add buffer for plotting
		static const int plot_buffer_size = 100;
		std::vector<float> plot_values;
		int plot_values_offset;
	};

} 