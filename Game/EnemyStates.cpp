#include "EnemyStates.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "MovementComponent.h"


void dae::WanderState::Start(EnemyComponent* ownerObject)
{
	auto compptr = ownerObject->GetOwner()->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(true);
}

void dae::WanderState::Update(EnemyComponent* ownerObject)
{
	ownerObject->SearchPlayer();
}





void dae::WanderState::End(EnemyComponent* ownerObject)
{
	auto compptr = ownerObject->GetOwner()->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}

void dae::ChaseState::Start(EnemyComponent* ownerObject)
{
	auto compptr = ownerObject->GetOwner()->GetComponentByType<ChaseMovementComponent>();
	if (compptr != nullptr) {
		compptr->SetEnabled(true);
		compptr->SetTarget(m_Target);
	}

}

void dae::ChaseState::Update(EnemyComponent* ownerObject)
{
	if (m_Target == nullptr) return;
	bool CanSee = ownerObject->CanSeePlayer(m_Target);
	if (CanSee) 
	{
		if (!m_HasSeen) {
			auto compptr = ownerObject->GetOwner()->GetComponentByType<ChaseMovementComponent>();
			if (compptr != nullptr) compptr->SetTarget(m_Target);
		}
		m_LastSeenTimer = 0.f;
	}
	else if (!CanSee && !m_HasSeen) 
	{
		m_LastSeenTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_LastSeenTimer >= m_GiveUpTime)
		{
			ownerObject->SetState(std::make_unique<WanderState>());
			std::cout << "GiveUp";
			return;
		}
	}

	m_HasSeen = CanSee;


}

void dae::ChaseState::End(EnemyComponent* ownerObject)
{
	auto compptr = ownerObject->GetOwner()->GetComponentByType<ChaseMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}
