#pragma once
#include "ObjectComponent.h"

namespace dae
{

	class BombComponent final : public ObjectComponent
	{
	private:
		float m_TimePassed{};
		float m_DetonationTime;
		bool m_IsDetonating{ false };
		void Explode();

	public:
		virtual void Update() override;
		virtual void Render() const override {};
		void StartDetonate(bool value = true) {
			m_IsDetonating = value;
		}


		BombComponent(GameObject* pOwner, float detonationTime) : ObjectComponent(pOwner), m_DetonationTime{detonationTime}
		{
		}
		~BombComponent() {}
		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;
	};
}