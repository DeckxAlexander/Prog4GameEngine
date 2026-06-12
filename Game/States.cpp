#include "States.h"
#include "EnemyComponent.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "MovementComponents.h"



void dae::IdleWanderState::Start(GameObject* ownerObject)
{
	EnemyState::Start(ownerObject);
	auto compptr = ownerObject->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(true);
}

void dae::IdleWanderState::End()
{
	auto compptr = m_EnemyComponent->GetOwner()->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}



void dae::SearchWanderState::Start(GameObject* ownerObject)
{
	EnemyState::Start(ownerObject);
	auto compptr = ownerObject->GetComponentByType<WanderMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(true);
}

std::unique_ptr<dae::State> dae::SearchWanderState::Update()
{
	m_EnemyComponent->SearchPlayer();
	return nullptr;
}


void dae::SearchWanderState::End()
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

std::unique_ptr<dae::State> dae::ChaseState::Update()
{
	if (m_Target == nullptr) return nullptr;
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
			m_EnemyComponent->GiveUpChase();
			std::cout << "GiveUp";
			return nullptr;
		}
	}

	m_HasSeen = CanSee;
	return nullptr;


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

void dae::PlayerControlledState::Start(GameObject* ownerObject)
{
	EnemyState::Start(ownerObject);
	auto compptr = ownerObject->GetComponentByType<MovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(true);
}

void dae::PlayerControlledState::End()
{
	//EnemyState::End();
	auto compptr = m_EnemyComponent->GetOwner()->GetComponentByType<MovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}
