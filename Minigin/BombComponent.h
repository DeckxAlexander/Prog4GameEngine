#pragma once
#include "ObjectComponent.h"

namespace dae
{
	class PlaceBombComponent;
	class BombComponent final : public ObjectComponent
	{
	private:
		float m_TimePassed{};
		float m_DetonationTime;
		bool m_IsDetonating{ false };
		PlaceBombComponent* m_Placer;
		void Explode();

	public:
		virtual void Update() override;
		virtual void Render() const override {};
		void StartDetonate(bool value = true) {
			m_IsDetonating = value;
		}


		BombComponent(GameObject* pOwner, float detonationTime, PlaceBombComponent* placer) : ObjectComponent(pOwner), m_DetonationTime{detonationTime}, m_Placer{ placer }
		{
		}
		~BombComponent() {}
		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;
	};
}