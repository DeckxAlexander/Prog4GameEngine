#include "Renderer.h"
#include "Renderer.h"
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

	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);



	SceneManager::GetInstance().Render();


	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
	SDL_RenderPresent(m_renderer);

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

	dst.x = (x - m_Camera.x) * m_Camera.zoom;
	dst.y = (y - m_Camera.y) * m_Camera.zoom;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);

	dst.w *= m_Camera.zoom;
	dst.h *= m_Camera.zoom;

	dst.x -= dst.w / 2.f;
	dst.y -= dst.h / 2.f;

	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_FRect dst{};
	dst.x = (x - m_Camera.x) * m_Camera.zoom;
	dst.y = (y - m_Camera.y) * m_Camera.zoom;
	//dst.w = width;
	//dst.h = height;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	dst.w *= width * m_Camera.zoom;
	dst.h *= height * m_Camera.zoom;

	dst.x -= dst.w / 2.f;
	dst.y -= dst.h / 2.f;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTextureOnScreen(const Texture2D& texture, float x, float y) const
{
	SDL_FRect dst{};

	dst.x = x;
	dst.y = y;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);

	dst.x -= dst.w / 2.f;
	dst.y -= dst.h / 2.f;

	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);

}

void dae::Renderer::RenderTextureOnScreen(const Texture2D& texture, float x, float y, float width, float height) const
{
	SDL_FRect dst{};
	dst.x = x;
	dst.y = y;
	//dst.w = width;
	//dst.h = height;
	SDL_GetTextureSize(texture.GetSDLTexture(), &dst.w, &dst.h);
	dst.w *= width;
	dst.h *= height;

	dst.x -= dst.w / 2.f;
	dst.y -= dst.h / 2.f;
	SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderLine(float fromX, float fromY, float toX, float toY) const 
{
	SDL_SetRenderDrawColor(GetSDLRenderer(), 255, 255, 255, 255);
	//SDL_RenderLine(GetSDLRenderer(), fromX, fromY, toX, toY);
	SDL_RenderLine(GetSDLRenderer(), 
		(fromX-m_Camera.x) * m_Camera.zoom, 
		(fromY - m_Camera.y) * m_Camera.zoom, 
		(toX - m_Camera.x) * m_Camera.zoom, 
		(toY - m_Camera.y) * m_Camera.zoom);





}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
