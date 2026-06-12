#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include <sstream>


dae::TextComponent::TextComponent(const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color)
	: ObjectComponent(), m_needsUpdate(true), m_text(text), m_color(color), m_font(std::move(font))
{ }

void dae::TextComponent::Update()
{
	if (m_needsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);

		if (auto renderComp = m_pOwner->GetComponentByType<RenderComponent>()) 
		{
			renderComp->SetTexture(std::make_shared<Texture2D>(texture));
		}
		m_needsUpdate = false;
	}


}

std::vector<std::string> dae::TextComponent::SplitLines(const std::string& text)
{
	std::vector<std::string> lines;
	std::stringstream ss(text);
	std::string line;
	while (std::getline(ss, line))
		lines.push_back(line);
	return lines;
}

void dae::TextComponent::Render() const
{

}

void dae::TextComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}



void dae::TextComponent::SetColor(const SDL_Color& color)
{ 
	m_color = color; 
	m_needsUpdate = true; 
}



