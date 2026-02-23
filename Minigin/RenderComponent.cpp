#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"


void dae::RenderComponent::Update()
{
	//Nothing (YET)
}

void dae::RenderComponent::Render() const
{
	if (m_texture == nullptr) return;
	const auto& pos = m_transform.GetPosition();
	const auto& scale = m_transform.GetScale();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, scale.x, scale.y );
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	 m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<dae::Texture2D> texture) 
{
	m_texture = texture;
}

dae::RenderComponent::RenderComponent(GameObject* pOwner) : ObjectComponent(pOwner)
{

}

dae::RenderComponent::RenderComponent(GameObject* pOwner,const std::string& filename) : ObjectComponent(pOwner)
{
	SetTexture(filename);
}
