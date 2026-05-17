#include "HealthComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"

void dae::HealthComponent::ProcessDeath() 
{
	auto playerComp = GetOwner()->GetComponentByType<PlayerComponent>();
	if (playerComp) playerComp->PlayerDeath();

	GetOwner()->MarkForDelete();



}