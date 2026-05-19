#include "ExtraBombPowerUpComponent.h"
#include "PlaceBombComponent.h"
#include "GameObject.h"


void dae::ExtraBombPowerUpComponent::ActivatePowerUp(GameObject* player)
{
	PowerUpComponent::ActivatePowerUp(player);
	player->GetComponentByType<PlaceBombComponent>()->AddCanPlace();

}

dae::ExtraBombPowerUpComponent::ExtraBombPowerUpComponent(GameObject* pOwner) : PowerUpComponent(pOwner)
{
}

