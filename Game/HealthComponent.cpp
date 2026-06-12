#include "HealthComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

dae::HealthComponent::HealthComponent() : ObjectComponent()
{}

void dae::HealthComponent::ProcessDeath()
{
	if (GetOwner()->IsMarkedForDelete()) return;
	auto playerComp = GetOwner()->GetComponentByType<PlayerComponent>();
	if (playerComp) playerComp->PlayerDeath();
	else 
	{
		auto enemyComp = GetOwner()->GetComponentByType<EnemyComponent>();
		if (enemyComp) enemyComp->EnemyDeath();
	
	}

}