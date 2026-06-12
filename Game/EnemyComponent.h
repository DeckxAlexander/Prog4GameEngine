#pragma once
#include "ObjectComponent.h"
#include "EventManagers.h"



namespace dae
{
	class State;
	class EnemyComponent final : public ObjectComponent, public IObserver
	{
	public:

		enum class IntelligenceType 
		{
			normal, smart, player
		};

		virtual void Start() override;
		virtual void Update() override;
		virtual void Render() const override {}
		void InitializePlayers();

		void SetState(std::unique_ptr<State> state);
		void SearchPlayer();
		bool CanSeePlayer(GameObject* obj);
		void GiveUpChase();

		void EnemyDeath();


		EnemyComponent( IntelligenceType intelligenceType, int scoreAmount = 100);
		~EnemyComponent();
		EnemyComponent(const EnemyComponent& other) = delete;
		EnemyComponent(EnemyComponent&& other) = delete;
		EnemyComponent& operator=(const EnemyComponent& other) = delete;
		EnemyComponent& operator=(EnemyComponent&& other) = delete;


		virtual void OnNotify(const Event& event);
		virtual void OnSubjectDestroyed(Subject* subject);
	private:
		std::vector<GameObject*> m_Players{};
		std::unique_ptr<State> m_State{};
		int m_ScoreAmount;

		IntelligenceType m_IntelligenceType;
	};
}