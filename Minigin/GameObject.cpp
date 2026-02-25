#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject() : m_transform{ this }, m_Components{}, m_pParent{ nullptr }, m_Children{}
{
}

void dae::GameObject::SetParent(GameObject* gameObject, bool keepWorldPosition)
{
	if (gameObject->IsMarkedForDelete() || gameObject == this ||
		std::find(m_Children.begin(), m_Children.end(), gameObject) != m_Children.end()) return;


	if (m_pParent != nullptr) //Remove child from previous parent
	{
		m_pParent->m_Children.erase(
			std::remove(
				m_Children.begin(),
				m_Children.end(),
				gameObject
			),
			m_Children.end()
		);
	}
	m_pParent = gameObject;
	if (gameObject != nullptr)
	{ 
		m_pParent->m_Children.push_back(this); 
		if (keepWorldPosition) 
		{
			m_transform.SetLocalPosition(m_transform.GetWorldPosition() - gameObject->GetWorldPosition());
		}
		m_transform.SetPositionDirty();
	}
	else 
	{
		m_transform.SetLocalPosition(m_transform.GetWorldPosition());
	}



}



dae::GameObject::~GameObject() = default;



void dae::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component.get()->Update();
	}
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
	m_transform.SetPosition({ x, y, 0.0f });


}

void dae::GameObject::SetScale(float x, float y)
{
	m_transform.SetScale(x, y, 1.0f);

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



