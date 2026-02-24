#include "ObjectComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"




dae::GameObject* dae::ObjectComponent::GetOwner()
{
	return nullptr;
}


dae::ObjectComponent::ObjectComponent(GameObject* pOwner) : m_pOwner{pOwner}
{
}
