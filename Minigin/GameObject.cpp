#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject() : m_transform{ std::make_unique<Transform>(this)}, m_Components{}, m_pParent{nullptr}, m_Children{}
{
}


dae::GameObject::GameObject(bool isGridObject, GridComponent* grid) : m_transform{}, m_Components{}, m_pParent{nullptr}, m_Children{}
{
	if (isGridObject) 
	{
		m_transform = std::make_unique<GridTransform>(this, grid);
	}
	else 
	{
		m_transform = std::make_unique<Transform>(this);
	}
}

void dae::GameObject::MarkForDelete()
{
	m_MarkedForDelete = true; 
	for(auto child : m_Children) //Detach Children when marked for delete
	{
		child->SetParent(nullptr);
	}

}

void dae::GameObject::SetParent(GameObject* gameObject, bool keepWorldPosition)
{
	if (gameObject == this ||
		std::find(m_Children.begin(), m_Children.end(), gameObject) != m_Children.end()) return;


	if (m_pParent != nullptr) //Remove child from previous parent
	{
		m_pParent->m_Children.erase(
			std::remove(
				m_pParent->m_Children.begin(),
				m_pParent->m_Children.end(),
				gameObject
			),
			m_pParent->m_Children.end()
		);
	}


	if (gameObject != nullptr)
	{ 
		gameObject->m_Children.push_back(this);
		if (keepWorldPosition) 
		{
			m_transform.get()->SetLocalPosition(m_transform.get()->GetWorldPosition() - gameObject->GetWorldPosition());
		}
		m_transform.get()->SetPositionDirty();
	}
	else 
	{
		m_transform.get()->SetLocalPosition(m_transform.get()->GetWorldPosition());
	}
	m_pParent = gameObject;


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
	m_transform.get()->SetPosition({ x, y, 0.0f });


}

void dae::GameObject::SetScale(float x, float y)
{
	m_transform.get()->SetScale(x, y, 1.0f);

}


void dae::GameObject::AddComponent(std::unique_ptr<ObjectComponent> component)
{
	component.get()->m_pOwner = this;
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



