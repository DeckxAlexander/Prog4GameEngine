#pragma once
#include <string>
#include <memory>
#include "ObjectComponent.h"
#include "Transform.h"
#include <SDL3/SDL.h> 

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public ObjectComponent
	{
	public:
		void Update() override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		TextComponent( const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		std::vector<std::string> SplitLines(const std::string& text);
	protected:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font{};
	};
}
