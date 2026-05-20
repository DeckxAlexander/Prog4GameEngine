#include "PowerUpComponent.h"

namespace dae
{
	class ExtraBombPowerUpComponent final : public PowerUpComponent
	{

	public:


		virtual void ActivatePowerUp(GameObject* player) override;

		ExtraBombPowerUpComponent(GameObject* pOwner);
		~ExtraBombPowerUpComponent() = default;
		ExtraBombPowerUpComponent(const ExtraBombPowerUpComponent& other) = delete;
		ExtraBombPowerUpComponent(ExtraBombPowerUpComponent&& other) = delete;
		ExtraBombPowerUpComponent& operator=(const ExtraBombPowerUpComponent& other) = delete;
		ExtraBombPowerUpComponent& operator=(ExtraBombPowerUpComponent&& other) = delete;
	};

	class FlamesPowerUpComponent final : public PowerUpComponent
	{

	public:


		virtual void ActivatePowerUp(GameObject* player) override;

		FlamesPowerUpComponent(GameObject* pOwner);
		~FlamesPowerUpComponent() = default;
		FlamesPowerUpComponent(const FlamesPowerUpComponent& other) = delete;
		FlamesPowerUpComponent(FlamesPowerUpComponent&& other) = delete;
		FlamesPowerUpComponent& operator=(const FlamesPowerUpComponent& other) = delete;
		FlamesPowerUpComponent& operator=(FlamesPowerUpComponent&& other) = delete;
	};
}