#include "ObjectComponent.h"
#include "ResourceManager.h"

void dae::ObjectComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::ObjectComponent::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}