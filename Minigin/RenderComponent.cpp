#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

void dae::RenderComponent::Update()
{
	//Nothing (YET)
}

void dae::RenderComponent::Render() const
{
	const auto& pos = m_transform.GetPosition();
	const auto& scale = m_transform.GetScale();
	Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y, scale.x, scale.y );
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

dae::RenderComponent::RenderComponent(const std::string& filename, const std::string& componentName) : ObjectComponent(componentName)
{
	SetTexture(filename);
}
