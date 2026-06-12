#include "PowerUpComponent.h"

namespace dae
{
	class ExtraBombPowerUpComponent final : public PowerUpComponent
	{

	public:


		virtual void ActivatePowerUp(GameObject* player) override;

		ExtraBombPowerUpComponent();
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

		FlamesPowerUpComponent();
		~FlamesPowerUpComponent() = default;
		FlamesPowerUpComponent(const FlamesPowerUpComponent& other) = delete;
		FlamesPowerUpComponent(FlamesPowerUpComponent&& other) = delete;
		FlamesPowerUpComponent& operator=(const FlamesPowerUpComponent& other) = delete;
		FlamesPowerUpComponent& operator=(FlamesPowerUpComponent&& other) = delete;
	};

	class DetonatorPowerUpComponent final : public PowerUpComponent
	{

	public:


		virtual void ActivatePowerUp(GameObject* player) override;

		DetonatorPowerUpComponent();
		~DetonatorPowerUpComponent() = default;
		DetonatorPowerUpComponent(const DetonatorPowerUpComponent& other) = delete;
		DetonatorPowerUpComponent(DetonatorPowerUpComponent&& other) = delete;
		DetonatorPowerUpComponent& operator=(const DetonatorPowerUpComponent& other) = delete;
		DetonatorPowerUpComponent& operator=(DetonatorPowerUpComponent&& other) = delete;
	};
}