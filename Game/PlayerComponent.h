#pragma once
#include "ObjectComponent.h"




namespace dae 
{
	class Subject;
	class GameManager;
	class PlayerComponent final : public ObjectComponent
	{
	private:
		friend class GameManager;
		int m_PlayerIndex;
		std::unique_ptr<Subject> m_Subject;
		int m_PlayerLives{ 3 };
		void Respawn();

	public:
		virtual void Start() override;
		virtual void Update() override {}
		virtual void Render() const override {}
		PlayerComponent(GameObject* pOwner, int index);
		virtual ~PlayerComponent() = default;
		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;

		int GetIndex() { return m_PlayerIndex; }
		

		void PlayerDeath();
		Subject* GetSubject() const { return m_Subject.get(); }




	};
}