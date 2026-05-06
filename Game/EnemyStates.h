#pragma once


namespace dae {
	class EnemyComponent;
	class WanderMovementComponent;
	class GameObject;
	class EnemyState
	{
	public:
		virtual void Start([[maybe_unused]] EnemyComponent* ownerObject) = 0;
		virtual void Update([[maybe_unused]] EnemyComponent* ownerObject) = 0;
		virtual void End([[maybe_unused]] EnemyComponent* ownerObject) = 0;

	};


	class WanderState : public EnemyState
	{
	public:
		WanderState() {}

		virtual void Start([[maybe_unused]] EnemyComponent* ownerObject) override;
		virtual void Update([[maybe_unused]] EnemyComponent* ownerObject) override;
		virtual void End([[maybe_unused]] EnemyComponent* ownerObject) override;


	};

	class IdleState : public EnemyState
	{
	public:
		IdleState() {}

		virtual void Start([[maybe_unused]] EnemyComponent* ownerObject) override {};
		virtual void Update([[maybe_unused]] EnemyComponent* ownerObject) override {};
		virtual void End([[maybe_unused]] EnemyComponent* ownerObject) override {};


	};

	class ChaseState : public EnemyState
	{
	public:
		ChaseState(GameObject* target) : m_Target{target} {}

		virtual void Start([[maybe_unused]] EnemyComponent* ownerObject) override;
		virtual void Update([[maybe_unused]] EnemyComponent* ownerObject) override;
		virtual void End([[maybe_unused]] EnemyComponent* ownerObject) override;

	private:
		GameObject* m_Target;
		bool m_HasSeen{true};
		float m_LastSeenTimer{};
		float m_GiveUpTime{ 2.f };
	};
}