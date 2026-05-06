#pragma once


namespace dae {
	class EnemyComponent;
	class WanderMovementComponent;
	class GameObject;
	class State
	{
	public:
		virtual void Start([[maybe_unused]] GameObject* ownerObject) = 0;
		virtual void Update() = 0;
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
		virtual void Update() override = 0;
		virtual void End() override = 0;


	protected:
		EnemyComponent* m_EnemyComponent{};


	};


	class WanderState final : public EnemyState
	{
	public:
		WanderState() = default;

		virtual void Start([[maybe_unused]] GameObject* ownerObject) override;
		virtual void Update() override;
		virtual void End() override;


	};

	class IdleState final : public EnemyState
	{
	public:
		IdleState() = default;

		virtual void Update() override {};
		virtual void End() override {};


	};

	class ChaseState final : public EnemyState
	{
	public:
		ChaseState(GameObject* target) : m_Target{target} {}

		virtual void Start([[maybe_unused]] GameObject* ownerObject) override;
		virtual void Update() override;
		virtual void End() override;

	private:
		GameObject* m_Target;
		bool m_HasSeen{true};
		float m_LastSeenTimer{};
		float m_GiveUpTime{ 2.f };
	};
}