#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Singleton.h"
#include "Camera.h"

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
		Camera m_Camera{1280, 576};



	public:
		void Init(SDL_Window* window);
		void Render() const;

		Camera& GetCamera() { return m_Camera; }

		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderLine(float fromX, float fromY, float toX, float toY) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }



	};
}

