#pragma once
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "imgui_impl_opengl3.h"
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

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

        if (TTF_Init() == -1) {
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
        TTF_Quit();
    }


    std::string convertToASCII(const unsigned char* pixels, int width, int height) {
        std::string asciiArt;
        const char* asciiChars = " .:-=+*#%@";
        for (int y = 0; y < height; y += 2) {
            for (int x = 0; x < width; x++) {
                int offset = (y * width + x) * 4;
                int r = pixels[offset];
                int g = pixels[offset + 1];
                int b = pixels[offset + 2];
                float brightness = 0.3f * r + 0.59f * g + 0.11f * b;
                int index = static_cast<int>((brightness / 255.0f) * (strlen(asciiChars) - 1));
                asciiArt += asciiChars[index];
            }
            asciiArt += '\n';
        }
        return asciiArt;
    }

    std::vector<std::string> loadGIFFrames(const char* filepath, int& frameWidth, int& frameHeight) {
        // Load the GIF file into memory
        FILE* file = fopen(filepath, "rb");
        if (!file) {
            std::cerr << "Failed to open GIF file" << std::endl;
            return {};
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        std::vector<unsigned char> file_data(file_size);
        fread(file_data.data(), 1, file_size, file);
        fclose(file);

        // Decode the GIF
        int width, height, frames;
        unsigned char* data;
        int* delays;

        data = stbi_load_gif_from_memory(file_data.data(), file_size, &delays, &width, &height, &frames, 0, 0);

        if (!data) {
            std::cerr << "Failed to load GIF" << std::endl;
            return {};
        }

        std::vector<std::string> asciiFrames;
        frameWidth = width;
        frameHeight = height;

        // Convert each frame to ASCII
        for (int i = 0; i < frames; ++i) {
            int frame_offset = i * width * height * 4;
            asciiFrames.push_back(convertToASCII(data + frame_offset, width, height));
        }

        // Free the image memory
        stbi_image_free(data);

        return asciiFrames;
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

        Style::Initialize();
        Style::SetImGuiStyle();
 
        m_running = true;
 
        left_image = LoadTexture("Assets/left_image.png", m_window->get_native_renderer());
        font = TTF_OpenFont("Assets/8_bit_Arcade_In.ttf", 32);
      
 

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

        sorter->Init(values, delay);
        is_sorting_paused = true;
        plot_values = std::vector<float>(plot_buffer_size, 0.0f);
        plot_values_offset = 0;



        int frameWidth = 0;
        int frameHeight = 0;
        asciiFrames = loadGIFFrames("Assets/gifka.gif", frameWidth, frameHeight);

        while (m_running) {
            SDL_Event event{};
            while (SDL_PollEvent(&event) == 1) {
                OnEvent(event);
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
        SDL_RenderClear(m_window->get_native_renderer());
       
        int window_width{ 0 };
        int window_height{ 0 };
        SDL_GetWindowSize(m_window->get_native_window(), &window_width, &window_height);
    
        int margin = 10;
        int square1_width = window_width * 0.3;
        int square2_width = window_width * 0.7;
        int square_height = window_height;

        // Draw the first rectangle (red)
        SDL_Rect square1 = { 0, 0, square1_width, square_height };
        SDL_SetRenderDrawColor(m_window->get_native_renderer(), 133, 131, 132, 255);
        SDL_RenderFillRect(m_window->get_native_renderer(), &square1);

        // Desired texture size
        const int target_texture_width = 300;
        const int target_texture_height = 300;

        // Calculate the position to center the texture within the red rectangle
        int x = (square1_width - target_texture_width) / 2;
        int y = (square_height - target_texture_height) / 2 - 100;

        std::string text = "> Oh, you find me.";
        int text_width = target_texture_width; // Stretch to the same width as the image
        int text_height = 128; // Example height, adjust as necessary
        int text_x = (square1_width) / 2;
        int text_y = y + target_texture_height + 10; // Position just below the image with some padding

        RenderText(m_window->get_native_renderer(), text, 32, x, y + target_texture_height + 10, square1_width);

        SDL_Rect dstrect = { x, y, target_texture_width, target_texture_height };
        SDL_RenderCopy(m_window->get_native_renderer(), left_image, nullptr, &dstrect);

        // Draw the second rectangle (sort zone)
        SDL_Rect square2 = { square1_width, 0, square2_width, square_height };
        SDL_SetRenderDrawColor(m_window->get_native_renderer(), 37, 38, 41, 255);
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
        bool* p_open = NULL;

        int window_width{ 0 };
        int window_height{ 0 };

        SDL_GetWindowSize(m_window->get_native_window(), &window_width, &window_height);

        int panel_width = static_cast<int>(window_width * 0.3);
        int panel_height = window_height;

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(panel_width, panel_height));

        char buf[128];
        sprintf(buf, "Sorter Control Panel | %cw%c###MainPanel", "O-TO"[(int)(ImGui::GetTime() / 5.75f) & 3], "O-TO"[(int)(ImGui::GetTime() / 5.75f) & 3]);
        ImGui::Begin(buf, p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
 
        ImGui::SeparatorText("ABOUT THIS ALGORITHM:");
        ImGui::TextWrapped(sorter->GetDescription().c_str());

       
        ImGui::SeparatorText("PLOT GRAPH:");
        ImGui::PlotLines("##SortingState", plot_values.data(), plot_buffer_size, plot_values_offset, nullptr, 0.0f, *std::max_element(plot_values.begin(), plot_values.end()), ImVec2(static_cast<float>(panel_width - 20), 80.0f));

        ImGui::SeparatorText("BIG 'O':");
        std::vector<float> big_o_plot = sorter->GetBigOPlotData();
        ImGui::PlotLines("##BigO", big_o_plot.data(), big_o_plot.size(), 0, nullptr, 0.0f, big_o_plot.back(), ImVec2(static_cast<float>(panel_width - 20), 300.0f));


 
        ImGui::SeparatorText("CONTROL SECTION:");
        ImGui::BeginGroup();

        const char* items[] = { "Bubble Sort", "Insertion Sort", "Selection Sort", "Quick Sort", "Heap Sort", "Merge Sort" };
        static int item_current = 0;

        ImGui::SetNextItemWidth(static_cast<float>(panel_width * 0.65f));
        if (ImGui::Combo("Algoritme", &item_current, items, IM_ARRAYSIZE(items)))
        {
            switch (item_current) {
            case 0:
                sorter = std::make_unique<BubbleSort>();
                break;
            case 1:
                sorter = std::make_unique<InsertionSort>();
                break;
            case 2:
                
                break;
            default:
                break;
            }
            std::random_device rd;
            std::mt19937 g(rd());
            is_sorting_paused = true;
            std::shuffle(values.begin(), values.end(), g);
            sorter->Init(values, delay);
        }

        static int new_sort_size = 100;

        ImGui::SetNextItemWidth(static_cast<float>(panel_width * 0.65f));
        if (ImGui::SliderInt("Size Sorting", &new_sort_size, 10, 200, "%d", ImGuiSliderFlags_AlwaysClamp))
        {
            values.resize(new_sort_size);
            for (int i = 0; i < new_sort_size; ++i) {
                values[i] = i + 1;
            }
            std::random_device rd;
            std::mt19937 g(rd());
            is_sorting_paused = true;
            std::shuffle(values.begin(), values.end(), g);
            sorter->Init(values, delay);
        }

        ImGui::SetNextItemWidth(static_cast<float>(panel_width * 0.65f));
        if (ImGui::SliderInt("Delay Sorting", &delay, 0, 500, "%d", ImGuiSliderFlags_AlwaysClamp))
        {
           sorter->SetDelay(delay);
        }

        if (ImGui::Button("Pause/Resume Sorting", ImVec2(static_cast<float>(panel_width - 20), 0))) {
            TogglePauseSorting();
        }

        ImGui::EndGroup();

        ImGui::End();






        // ---------------------------------------------------------------------------
        // Little Tip Overlay Panel
        static int location = 0;
        
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags ovelay_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if (m_show_tip_panel) {
            if (location >= 0)
            {
                const float PAD = 10.0f;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
                ImVec2 work_size = viewport->WorkSize;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + panel_width + PAD);
                window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
                window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
                window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(viewport->ID);
                ovelay_flags |= ImGuiWindowFlags_NoMove;
            }
            else if (location == -2)
            {
                // Center window
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ovelay_flags |= ImGuiWindowFlags_NoMove;
            }
            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background



            // Overlay Tips Panel
 
            ImGui::Begin("Tips Overlay", &m_show_tip_panel, ovelay_flags);
            ImGui::Text("Little Tips");
            ImGui::Text("(right-click to Close)\n" "F1 to open this panel\n" "SPACE - to pause/resume\n" "R - to reset");
            ImGui::Separator();
            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse Position: <invalid>");
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
                if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
                if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                if (ImGui::MenuItem("Close", NULL)) m_show_tip_panel = false;
                ImGui::EndPopup();
            }
            float first_panel_height = ImGui::GetWindowHeight();
            ImGui::End();
            
        }
        // ---------------------------------------------------------------------------
        


        // Little Fun Panel
        if (!is_sorting_paused){
        ImGuiWindowFlags ovelay_flags_fun = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;

       
        const float PAD = 10.0f;
        window_pos.x = (work_pos.x + panel_width + PAD);
        if (location == 0) {
            float first_panel_height = ImGui::GetWindowHeight();
            // If the first panel is on the left side, place the second panel below it
            window_pos.y = first_panel_height + PAD + 10;
        }
        else {
            window_pos.y = PAD;
        }
        window_pos_pivot.x = 0.0f;
        window_pos_pivot.y = 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        
 
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::Begin("Meme Window", p_open, ovelay_flags_fun);


        char animated_text[128];
        sprintf(animated_text, "Sorting... %c", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3]);
        ImGui::Text(animated_text);
 
       

        ImGui::End();
        }

        static int currentFrame = 0;
        static int frameWidth = 1000;
        static int frameHeight = 1000;
        static float frameDuration = 0.1f;
        static Uint32 lastFrameTime = 0;

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFrameTime >= frameDuration * 1000) {
            currentFrame = (currentFrame + 1) % asciiFrames.size();
            lastFrameTime = currentTime;
        }

        static float opacity = 1.5f;
        static float anim_t = 1.5f;

        if (opacity != 0){
       
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)));
        ImGui::SetNextWindowBgAlpha(opacity); // Set initial opacity for the window

        // Add window flags to disable resizing and moving
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar;

        // Create the window with the specified flags
        ImGui::Begin("ASCII Art", nullptr, window_flags);

        // Gradually decrease opacity each frame
        anim_t -= 0.002f;
        if (opacity > 0.0f) {
            opacity -= 0.002f; // Adjust the decrement value to control fade-out speed
            ImGui::SetNextWindowBgAlpha(opacity);
        }
        else {
            opacity = 0.0f; // Make sure opacity doesn't go below zero
        }



        ImGui::SetWindowFontScale(std::min(static_cast<float>(600) / frameWidth, static_cast<float>(600) / frameHeight));
        //ImGui::TextUnformatted(asciiFrames[currentFrame].c_str());
        ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, opacity); // White color with alpha based on text_opacity
        ImGui::TextColored(textColor, "%s", asciiFrames[currentFrame].c_str());

        ImGui::End();
        }


        Style::ShowStylePanel();
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

    void Application::OnEvent(const SDL_Event& event)
    {
        if (event.type == SDL_QUIT) {
            Shutdown();
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_F1:
                m_show_tip_panel = !m_show_tip_panel;
                break;
            case SDLK_F2:
                Style::open_panel = !Style::open_panel;
            default:
                break;
            }
        }
        ImGui_ImplSDL2_ProcessEvent(&event);
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

    void Application::RenderText(SDL_Renderer* renderer, const std::string& text, int size, int x, int y, int max_width) {
        int window_width{ 0 };
        int window_height{ 0 };
        SDL_GetWindowSize(m_window->get_native_window(), &window_width, &window_height);

        // Calculate scale based on window size
        float scale_width = static_cast<float>(window_width) / 1920.0f;  // Assume 1920 is the base width
        float scale_height = static_cast<float>(window_height) / 1080.0f;  // Assume 1080 is the base height
        float scale = std::min(scale_width, scale_height);

        // Adjust the font size based on the scale
        int font_size = static_cast<int>(size * scale);  // Adjust the base font size as necessary

        // Open the font with the new size
        TTF_Font* scaled_font = TTF_OpenFont("Assets/upheavtt.ttf", font_size);  // Adjust the font path as necessary
        if (!scaled_font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }

        // Measure the text width and adjust the font size if necessary
        int text_width;
        int text_height;
        TTF_SizeText(scaled_font, text.c_str(), &text_width, &text_height);
        while (text_width > max_width && font_size > 1) {
            font_size--;
            TTF_CloseFont(scaled_font);
            scaled_font = TTF_OpenFont("Assets/upheavtt.ttf", font_size);
            if (!scaled_font) {
                std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                return;
            }
            TTF_SizeText(scaled_font, text.c_str(), &text_width, &text_height);
        }

        // Center the text horizontally within the specified width
        int centered_x = x + (max_width - text_width) / 2;

        // Render the text to a surface
        SDL_Color color = { 255, 255, 255, 255 };  // White color
        SDL_Surface* surface = TTF_RenderText_Blended(scaled_font, text.c_str(), color);
        if (!surface) {
            std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
            TTF_CloseFont(scaled_font);
            return;
        }

        // Create a texture from the surface
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            TTF_CloseFont(scaled_font);
            return;
        }

        // Define the destination rectangle
        SDL_Rect dstrect = { centered_x, y, text_width, text_height };

        // Render the texture
        SDL_RenderCopy(renderer, texture, nullptr, &dstrect);

        // Clean up
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        TTF_CloseFont(scaled_font);
    }
}   