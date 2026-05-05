#include "EnemyStates.h"
#include "EnemyComponent.h"
#include "GameObject.h"
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
	if (compptr != nullptr) compptr->SetEnabled(true);

	compptr->SetTarget(m_Target);

}

void dae::ChaseState::End(EnemyComponent* ownerObject)
{
	auto compptr = ownerObject->GetOwner()->GetComponentByType<ChaseMovementComponent>();
	if (compptr != nullptr) compptr->SetEnabled(false);
}
