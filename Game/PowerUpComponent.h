#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae
{
	class CollisionComponent;
	class PowerUpComponent final : public ObjectComponent, public IObserver
	{
	private:
		std::vector<CollisionComponent*> m_PlayersColliders{};
		CollisionComponent* m_Collider{ nullptr };
		void InitializePlayers();

	public:
		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override {}

		PowerUpComponent(GameObject* pOwner);
		~PowerUpComponent() = default;
		PowerUpComponent(const PowerUpComponent& other) = delete;
		PowerUpComponent(PowerUpComponent&& other) = delete;
		PowerUpComponent& operator=(const PowerUpComponent& other) = delete;
		PowerUpComponent& operator=(PowerUpComponent&& other) = delete;

		virtual void OnNotify(const Event& event);
	};
}