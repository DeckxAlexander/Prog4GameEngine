#include "LifetimeComponent.h"
#include "TimeManager.h"
#include "GameObject.h"

void dae::LifetimeComponent::Update()
{
	float deltaT = TimeManager::GetInstance().GetDeltaTime();
	m_AccumulatedTime += deltaT;

	if (m_AccumulatedTime >= m_LifeTime)
	{
		GetOwner()->MarkForDelete();
	}

}

dae::LifetimeComponent::LifetimeComponent(float lifetime) : ObjectComponent(nullptr), m_LifeTime{lifetime}
{
	

}