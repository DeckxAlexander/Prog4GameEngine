#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"

namespace dae
{
	class CollisionComponent;
	class PowerUpComponent : public ObjectComponent, public IObserver
	{
	private:
		std::vector<CollisionComponent*> m_PlayersColliders{};
		CollisionComponent* m_Collider{ nullptr };
		bool m_IsActive{ false };
		void InitializePlayers();

	public:
		virtual void Start() override;
		virtual void Update() override;

		virtual void ActivatePowerUp(GameObject* player);

		PowerUpComponent();
		~PowerUpComponent() ;
		PowerUpComponent(const PowerUpComponent& other) = delete;
		PowerUpComponent(PowerUpComponent&& other) = delete;
		PowerUpComponent& operator=(const PowerUpComponent& other) = delete;
		PowerUpComponent& operator=(PowerUpComponent&& other) = delete;

		virtual void OnNotify(const Event& event) override;
		virtual void OnSubjectDestroyed(Subject*) override {}
	};


}