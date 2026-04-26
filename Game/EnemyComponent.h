#pragma once
#include "ObjectComponent.h"

namespace dae
{
	class EnemyComponent final : public ObjectComponent
	{
	private:
		std::vector<GameObject*> m_Players{};
	public:
		virtual void Update() override;
		virtual void Render() const override {}
		void InitializePlayers();
		
		EnemyComponent(GameObject* pOwner) : ObjectComponent(pOwner)
		{
			InitializePlayers();
		}
		~EnemyComponent() = default;
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;
	};
}