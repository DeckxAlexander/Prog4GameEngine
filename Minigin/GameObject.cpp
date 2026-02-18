#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


bool dae::GameObject::HasComponentOfName(const std::string& name) const
{
	return GetComponentByName(name) != nullptr;
}

dae::GameObject::~GameObject() = default;



void dae::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component.get()->Update();
	}
}

dae::ObjectComponent* dae::GameObject::GetComponentByName(const std::string& name) const
{
	for (const auto& component : m_Components)
	{
		auto compPtr = component.get();
		if (compPtr->GetName() == name)
		{
			return compPtr;
		}
	}
	return nullptr;
}


void dae::GameObject::Render() const
{

	
	for (const auto& component : m_Components)
	{
		component.get()->Render();
	}
}


void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
	for (const auto& component : m_Components)
	{
		component.get()->SetPosition(x, y);
	}

}

void dae::GameObject::SetScale(float x, float y)
{
	m_transform.SetScale(x, y, 1.0f);
	for (const auto& component : m_Components)
	{
		component.get()->SetScale(x, y);
	}
}

void dae::GameObject::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	m_Components.emplace_back(std::move(component));
}


void dae::GameObject::RemoveComponent(const ObjectComponent& component) 
{
	m_Components.erase(
		std::remove_if(
			m_Components.begin(),
			m_Components.end(),
			[&component](const auto& ptr) { return ptr.get() == &component; }
		),
		m_Components.end()
	);

}



