#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"


void dae::RenderComponent::Update()
{
	//Nothing (YET)
}

void dae::RenderComponent::Render() const
{
	if (m_texture == nullptr) return;
	const auto& pos = m_pOwner->GetWorldPosition();
	const auto& scale = m_pOwner->GetScale();

	if (m_RenderOnScreen) Renderer::GetInstance().RenderTextureOnScreen(*m_texture, pos.x, pos.y, scale.x, scale.y);
	else Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, scale.x, scale.y );
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	 m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<dae::Texture2D> texture) 
{
	m_texture = texture;
}

dae::RenderComponent::RenderComponent() : ObjectComponent()
{

}

dae::RenderComponent::RenderComponent(const std::string& filename) : ObjectComponent()
{
	SetTexture(filename);
}
