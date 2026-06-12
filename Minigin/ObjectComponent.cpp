#include "ObjectComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"




dae::GameObject* dae::ObjectComponent::GetOwner() const
{
	return m_pOwner;
}


dae::ObjectComponent::ObjectComponent() : m_pOwner{nullptr}
{
}
