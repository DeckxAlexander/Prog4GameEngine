#include <stdexcept>
#include <imgui.h>
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

	//ImGui::ShowDemoWindow(); // For demonstration purposes, do not keep this in your engine
    RenderImGui();
    
	ImGui::Render();

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);

}

void dae::Renderer::RenderImGui() const
{


    ImGui::Begin("Cache Manager");

    // Samples input
    ImGui::PushItemWidth(120);
    ImGui::InputInt("##samples", &m_samples);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text("# samples");

    m_samples = std::max(1, m_samples);

    //Thrash Cache 
    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {

        m_Timings3D.clear();
        m_Timings3D = ThrashCacheManager::ThrashCache(m_samples);


    }
    if (!m_Timings3D.empty())
    {
        ImGui::PlotLines("##plot3D",m_Timings3D.data(),static_cast<int>(m_Timings3D.size()),0,nullptr,0.0f,
			*std::max_element(m_Timings3D.begin(), m_Timings3D.end()),ImVec2(0, 100)
        );
    }

	//Thrash Cache Alt
    if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
    {
		m_Timings3DAlt.clear();
		m_Timings3DAlt = ThrashCacheManager::ThrashCacheAlt(m_samples);
    }

    if (!m_Timings3DAlt.empty())
    {
        ImGui::PlotLines("##plot3DAlt",m_Timings3DAlt.data(), static_cast<int>(m_Timings3DAlt.size()),0,nullptr,0.0f,
            *std::max_element(m_Timings3DAlt.begin(), m_Timings3DAlt.end()),ImVec2(0, 100));
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
