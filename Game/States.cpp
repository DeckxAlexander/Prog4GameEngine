#include "States.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "MovementComponent.h"


void dae::WanderState::Start(GameObject* ownerObject)
{
	EnemyState::Start(ownerObject);
	auto compptr = ownerObject->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(true);
}

void dae::WanderState::Update()
{
	m_EnemyComponent->SearchPlayer();
}





void dae::WanderState::End()
{
	auto compptr = m_EnemyComponent->GetOwner()->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}

void dae::ChaseState::Start(GameObject* ownerObject)
{
	EnemyState::Start(ownerObject);
	auto compptr = ownerObject->GetComponentByType<ChaseMovementComponent>();
	if (compptr != nullptr) {
		compptr->SetEnabled(true);
		compptr->SetTarget(m_Target);
	}

}

void dae::ChaseState::Update()
{
	if (m_Target == nullptr) return;
	bool CanSee = m_EnemyComponent->CanSeePlayer(m_Target);
	if (CanSee) 
	{
		if (!m_HasSeen) {
			auto compptr = m_EnemyComponent->GetOwner()->GetComponentByType<ChaseMovementComponent>();
			if (compptr != nullptr) compptr->SetTarget(m_Target);
		}
		m_LastSeenTimer = 0.f;
	}
	else if (!CanSee && !m_HasSeen) 
	{
		m_LastSeenTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_LastSeenTimer >= m_GiveUpTime)
		{
			m_EnemyComponent->SetState(std::make_unique<WanderState>());
			std::cout << "GiveUp";
			return;
		}
	}

	m_HasSeen = CanSee;


}

void dae::ChaseState::End()
{
	auto compptr = m_EnemyComponent->GetOwner()->GetComponentByType<ChaseMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}

void dae::EnemyState::Start(GameObject* ownerObject)
{
	m_EnemyComponent = ownerObject->GetComponentByType<EnemyComponent>();
}
