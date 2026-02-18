#include "ObjectComponent.h"
#include "ResourceManager.h"

void dae::ObjectComponent::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::ObjectComponent::SetScale(float x, float y)
{
	m_transform.SetScale(x, y, 1.0f);
}


std::string dae::ObjectComponent::GetName() const
{
	return m_ComponentName;
}

dae::ObjectComponent::ObjectComponent(const std::string& componentName) : m_ComponentName{ componentName }
{
}
