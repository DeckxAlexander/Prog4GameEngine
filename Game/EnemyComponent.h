#pragma once
#include "ObjectComponent.h"



namespace dae
{
	class State;
	class EnemyComponent final : public ObjectComponent
	{
	private:
		std::vector<GameObject*> m_Players{};
		std::unique_ptr<State> m_State{};
	public:
		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override {}
		void InitializePlayers();
		
		void SetState(std::unique_ptr<State> state);
		void SearchPlayer();
		bool CanSeePlayer(GameObject* obj);
		void GiveUpChase();


		EnemyComponent(GameObject* pOwner);
		~EnemyComponent() = default;
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;
	};
}