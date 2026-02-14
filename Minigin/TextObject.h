#pragma once
#include <string>
#include <memory>
#include "ObjectComponent.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextObject : public ObjectComponent
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		//void SetTexture(const std::string& filename) = delete;
		void SetPosition(float x, float y) override;
		void SetColor(const SDL_Color& color);

		TextObject(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color = { 255, 255, 255, 255 });
		virtual ~TextObject() = default;
		TextObject(const TextObject& other) = delete;
		TextObject(TextObject&& other) = delete;
		TextObject& operator=(const TextObject& other) = delete;
		TextObject& operator=(TextObject&& other) = delete;
	protected:
		bool m_needsUpdate{};
		std::string m_text{};
		SDL_Color m_color{ 255, 255, 255, 255 };
		Transform m_transform{};
		std::shared_ptr<Font> m_font{};
		std::shared_ptr<Texture2D> m_textTexture{};
	};
}
