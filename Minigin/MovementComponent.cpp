#include "MovementComponent.h"
#include "GameObject.h"
#include "TimeManager.h"


dae::MovementComponent::MovementComponent(GameObject* pOwner) : ObjectComponent(pOwner), m_Angle{}, m_Speed{ 10.f }, m_Radius{ 50.f }, m_Center{}
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


