#include "ObjectComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"




dae::GameObject* dae::ObjectComponent::GetOwner() const
{
	return m_pOwner;
}


dae::ObjectComponent::ObjectComponent(GameObject* pOwner) : m_pOwner{pOwner}
{
}
