#pragma once
#include "ObjectComponent.h"



namespace dae 
{
	class Subject;
	class PlayerComponent final : public ObjectComponent
	{
	private:
		[[maybe_unused]] int m_PlayerIndex;
		std::unique_ptr<Subject> m_Subject;


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

		void PlayerDeath();
		Subject* GetSubject() const { return m_Subject.get(); }


	};
}