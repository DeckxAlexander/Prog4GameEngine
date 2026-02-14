#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

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
	
	if (m_texture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
	
	for (const auto& component : m_Components)
	{
		component.get()->Render();
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
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