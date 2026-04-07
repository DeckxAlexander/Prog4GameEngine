#include "ObjectComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"




dae::GameObject* dae::ObjectComponent::GetOwner()
{
	return m_pOwner;
}


dae::ObjectComponent::ObjectComponent(GameObject* pOwner) : m_pOwner{pOwner}
{
}
