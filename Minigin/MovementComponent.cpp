#include "MovementComponent.h"
#include "GameObject.h"
#include "TimeManager.h"


dae::MovementComponent::MovementComponent(GameObject* pOwner) : MovementComponent(pOwner, 10.f, 50.f)
{
}

dae::MovementComponent::MovementComponent(GameObject* pOwner, float speed, float radius) : ObjectComponent(pOwner), m_Angle{}, m_Speed{ speed }, m_Radius{ radius }, m_Center{}
{
}

void dae::MovementComponent::Update()
{

	m_Angle += m_Speed * TimeManager::GetInstance().GetDeltaTime() ;


	glm::vec3 position{};


	position.x = m_Center.x + std::cos(m_Angle) * m_Radius;
	position.y = m_Center.y + std::sin(m_Angle) * m_Radius;

	m_pOwner->SetPosition(position.x, position.y);
}

void dae::MovementComponent::Render() const
{
	//Empty
}


