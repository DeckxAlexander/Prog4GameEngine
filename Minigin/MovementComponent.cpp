#include "MovementComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include <iostream>


dae::MovementComponent::MovementComponent(GameObject* pOwner, float speed) : ObjectComponent(pOwner), m_Speed{speed}
{
}



void dae::MovementComponent::Update()
{


	float deltaT{ TimeManager::GetInstance().GetDeltaTime() };

	glm::vec3 position{ m_pOwner->GetWorldPosition()};

	position.x += m_Velocity.x * m_Speed * deltaT;
	position.y += m_Velocity.y * m_Speed * deltaT;


	m_Velocity = glm::vec3{ 0.f,0.f,0.f };


	m_pOwner->SetPosition(position.x, position.y);
}

void dae::MovementComponent::Render() const
{
	//Empty
}


