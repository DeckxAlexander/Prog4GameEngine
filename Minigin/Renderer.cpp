#include <stdexcept>
#include <imgui.h>
#include <imgui_plot.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include "Renderer.h"
#include "SceneManager.h"
#include "ThrashCacheManager.h"
#include "Texture2D.h"



void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
	m_renderer = SDL_CreateRenderer(window, nullptr);
#else
	m_renderer = SDL_CreateRenderer(window, nullptr);
#endif

	if (m_renderer == nullptr)
	{
		std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#if __EMSCRIPTEN__
	io.IniFilename = NULL;
#endif

	ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
	ImGui_ImplSDLRenderer3_Init(m_renderer);
}

void dae::Renderer::Render() const
{

	//IMGUI
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

    RenderImGuiGameObject(); 
    RenderImGuiInt();
    
	ImGui::Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);

}

void dae::Renderer::RenderImGuiGameObject() const
{
    ImGui::Begin("Exercise 2");

    // Samples input
    ImGui::PushItemWidth(120);
    ImGui::InputInt("##samples", &m_Samples);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");

    m_Samples = std::max(1, m_Samples);

    //Thrash Cache 
    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {

        m_Timings3D.clear();
        m_Timings3D = ThrashCacheManager::ThrashCache(m_Samples);


    }

	//Thrash Cache Alt
    if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
    {
		m_Timings3DAlt.clear();
		m_Timings3DAlt = ThrashCacheManager::ThrashCacheAlt(m_Samples);
    }


    //Plot
    if (!m_Timings3D.empty() || !m_Timings3DAlt.empty())
    {
        const float* y_data[2];
        std::vector<float> x_values;
        ImU32 colors[2];

        int lineCount = 0;
        int dataCount = 0;
        float maxY = 0.0f;

        //Get Y values
        if (!m_Timings3D.empty())
        {
            y_data[lineCount] = m_Timings3D.data();
            colors[lineCount] = ImColor(0, 255, 0);
            dataCount = static_cast<int>(m_Timings3D.size());
            maxY = *std::max_element(m_Timings3D.begin(), m_Timings3D.end());
            lineCount++;
        }

        if (!m_Timings3DAlt.empty())
        {
            y_data[lineCount] = m_Timings3DAlt.data();
            colors[lineCount] = ImColor(255, 0, 0);
            int altSize = static_cast<int>(m_Timings3DAlt.size());
            dataCount = std::max(dataCount, altSize);
            float altMax = *std::max_element(m_Timings3DAlt.begin(), m_Timings3DAlt.end());
            maxY = std::max(maxY, altMax);
            lineCount++;
        }

        //Get X values
        x_values.resize(dataCount);
        for (int i = 0; i < dataCount; ++i)
        {
            x_values[i] = std::pow(2.0f, static_cast<float>(i));
        }

        //Config
        float plotWidth = ImGui::GetContentRegionAvail().x;
        ImGui::PlotConfig conf;
        conf.values.xs = x_values.data();         
        conf.values.ys_list = y_data;
        conf.values.ys_count = lineCount;
        conf.values.count = dataCount;
        conf.values.colors = colors;

        conf.scale.min = 0.0f;
        conf.scale.max = maxY;

        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_y.show = true;

        conf.line_thickness = 2.0f;
        conf.frame_size = ImVec2(plotWidth, 150);

        ImGui::Plot("Performance", conf);

    }

    ImGui::End();
}

void dae::Renderer::RenderImGuiInt() const
{
    ImGui::Begin("Exercise 1");

    // Samples input
    ImGui::PushItemWidth(120);
    ImGui::InputInt("##samplesInt", &m_SamplesInt);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");

    m_SamplesInt = std::max(1, m_SamplesInt);

    //Thrash Cache 
    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {

        m_TimingsInt.clear();
        m_TimingsInt = ThrashCacheManager::ThrashCacheInt(m_SamplesInt);


    }
    if (!m_TimingsInt.empty())
    {
        const float* y_data[1]= { m_TimingsInt.data() };
        std::vector<float> x_values;
        ImU32 colors[1] = { ImColor(0, 255, 0) };
        int dataCount = static_cast<int>(m_TimingsInt.size());
        float maxY = *std::max_element(m_TimingsInt.begin(), m_TimingsInt.end());

        //Get X values
        x_values.resize(dataCount);
        for (int i = 0; i < dataCount; ++i)
        {
            x_values[i] = std::pow(2.0f, static_cast<float>(i));
        }

        //Config
        float plotWidth = ImGui::GetContentRegionAvail().x;
        ImGui::PlotConfig conf;
        conf.values.xs = x_values.data();
        conf.values.ys_list = y_data;
        conf.values.ys_count = 1;
        conf.values.count = dataCount;
        conf.values.colors = colors;
        conf.scale.min = 0.0f;
        conf.scale.max = maxY;

        conf.tooltip.show = true;
        conf.grid_x.show = true;
        conf.grid_y.show = true;

        conf.line_thickness = 2.0f;
        conf.frame_size = ImVec2(plotWidth, 150);

        ImGui::Plot("Performance", conf);

    }


    ImGui::End();
}


void dae::Renderer::Destroy()
{
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}



}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	//dst.w = width;
	//dst.h = height;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	dst.w *= width;
	dst.h *= height;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
