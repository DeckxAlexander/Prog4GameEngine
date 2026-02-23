#include "ObjectComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

void dae::ObjectComponent::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::ObjectComponent::SetScale(float x, float y)
{
	m_transform.SetScale(x, y, 1.0f);

}


dae::GameObject* dae::ObjectComponent::GetOwner()
{
	return nullptr;
}


dae::ObjectComponent::ObjectComponent(GameObject* pOwner) : m_pOwner{pOwner}
{
}
