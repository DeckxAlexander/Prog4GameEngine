#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};


		mutable int m_samples{100};
		mutable std::vector<float> m_Timings3D{};
		mutable std::vector<float> m_Timings3DAlt{};
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void RenderImGui() const;


		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }


	};
}

