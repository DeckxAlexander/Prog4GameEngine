#include "PowerUps.h"
#include "PlaceBombComponent.h"
#include "GameObject.h"


void dae::ExtraBombPowerUpComponent::ActivatePowerUp(GameObject* player)
{
	PowerUpComponent::ActivatePowerUp(player);
	player->GetComponentByType<PlaceBombComponent>()->AddCanPlace();
	player->GetComponentByType<PlaceBombComponent>()->AddMaxCanPlace();

}

dae::ExtraBombPowerUpComponent::ExtraBombPowerUpComponent() : PowerUpComponent()
{
}

void dae::FlamesPowerUpComponent::ActivatePowerUp(GameObject* player)
{

	PowerUpComponent::ActivatePowerUp(player);
	player->GetComponentByType<PlaceBombComponent>()->AddSize(1);

}

dae::FlamesPowerUpComponent::FlamesPowerUpComponent() : PowerUpComponent()
{
	
}

void dae::DetonatorPowerUpComponent::ActivatePowerUp(GameObject* player)
{
	PowerUpComponent::ActivatePowerUp(player);
	player->GetComponentByType<PlaceBombComponent>()->SetUsingDetonator(true);
}

dae::DetonatorPowerUpComponent::DetonatorPowerUpComponent() : PowerUpComponent()
{
}
