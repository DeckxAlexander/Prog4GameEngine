#pragma once
#include <memory>


namespace dae {
	class EnemyComponent;
	class WanderMovementComponent;
	class GameObject;
	class State
	{
	public:
		virtual void Start([[maybe_unused]] GameObject* ownerObject) = 0;
		virtual std::unique_ptr<State> Update() = 0;
		virtual void End() = 0;

		State() = default;
		virtual ~State() = default;
	};


	class EnemyState : public State
	{
	public:
		EnemyState() = default;
		virtual ~EnemyState() = default;
		virtual void Start([[maybe_unused]] GameObject* ownerObject) override;
		virtual  std::unique_ptr<State> Update() override = 0;
		virtual void End() override = 0;


	protected:
		EnemyComponent* m_EnemyComponent{};


	};


	class SearchWanderState final : public EnemyState
	{
	public:
		SearchWanderState() = default;

		virtual void Start([[maybe_unused]] GameObject* ownerObject) override;
		virtual std::unique_ptr<State> Update() override;
		virtual void End() override;


	};

	class IdleWanderState final : public EnemyState
	{
	public:
		IdleWanderState() = default;

		virtual void Start([[maybe_unused]] GameObject* ownerObject) override;
		virtual std::unique_ptr<State> Update() override { return nullptr; }
		virtual void End() override;


	};

	class PlayerControlledState final : public EnemyState
	{
	public:
		PlayerControlledState() = default;

		virtual void Start(GameObject* ownerObject) override;
		virtual std::unique_ptr<State> Update() override { return nullptr; }
		virtual void End() override;


	};

	class IdleState final : public EnemyState
	{
	public:
		IdleState() = default;

		virtual std::unique_ptr<State> Update() override {};
		virtual void End() override {};


	};

	class ChaseState final : public EnemyState
	{
	public:
		ChaseState(GameObject* target) : m_Target{target} {}

		virtual void Start([[maybe_unused]] GameObject* ownerObject) override;
		virtual std::unique_ptr<State> Update() override;
		virtual void End() override;

	private:
		GameObject* m_Target;
		bool m_HasSeen{true};
		float m_LastSeenTimer{};
		float m_GiveUpTime{ 2.f };
	};
}