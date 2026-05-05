#pragma once
#include "ObjectComponent.h"
#include <glm/glm.hpp>
//#define PI 3.14159265358979323846


namespace dae
{
	class CollisionComponent;
	class MovementComponent : public ObjectComponent
	{
	protected:
		glm::vec3 m_Velocity{};
		float m_Speed{};
		CollisionComponent* m_Collider{};
	public:
		virtual void Update() override;
		virtual void Render() const override;
		void AddVelocity(float x, float y) 
		{
			m_Velocity += glm::vec3{ x,y,0 };
			float len = glm::length(m_Velocity);

			if (len > 0) m_Velocity /= len;
		};
		virtual void HitCollider() {}
		void Start() override;



		MovementComponent(GameObject* pOwner, float speed);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;
	};

	class WanderMovementComponent final : public MovementComponent
	{
	private:
		glm::vec3 m_DesiredVelocity{};
		GridComponent* m_pGrid;
		glm::vec3 FindNewDirection();

	public:
		virtual void Update() override;
		virtual void HitCollider() override
		{
			m_DesiredVelocity = FindNewDirection();
			float len = glm::length(m_DesiredVelocity);
			if (len > 0) m_DesiredVelocity /= len;
		}

		void SetVelocity(float x, float y)
		{
			m_DesiredVelocity = glm::vec3{ x,y,0 };
			float len = glm::length(m_DesiredVelocity);
			if (len > 0) m_DesiredVelocity /= len;
		};

		WanderMovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid = nullptr);
		~WanderMovementComponent() = default;
		WanderMovementComponent(const WanderMovementComponent& other) = delete;
		WanderMovementComponent(WanderMovementComponent&& other) = delete;
		WanderMovementComponent& operator=(const WanderMovementComponent& other) = delete;
		WanderMovementComponent& operator=(WanderMovementComponent&& other) = delete;
	};


	class ChaseMovementComponent final : public MovementComponent 
	{
	private:
		glm::vec3 m_DesiredVelocity{};
		GridComponent* m_pGrid;
		GameObject* m_Target{};
		glm::vec3 FindDirection();

		virtual void Recalculate()
		{
			if (m_Target == nullptr) return;
			SetVelocity(FindDirection());

		}

		float m_RecalculateTimer{};
		float m_RecalcMaxTime{ 1.f };
	public:

		void SetTarget(GameObject* target);



		virtual void Update() override;
		void SetVelocity(glm::vec3 velocity)
		{
			m_DesiredVelocity = velocity;
			float len = glm::length(m_DesiredVelocity);
			if (len > 0) m_DesiredVelocity /= len;
		};



		ChaseMovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid);
		~ChaseMovementComponent() = default;
		ChaseMovementComponent(const ChaseMovementComponent& other) = delete;
		ChaseMovementComponent(ChaseMovementComponent&& other) = delete;
		ChaseMovementComponent& operator=(const ChaseMovementComponent& other) = delete;
		ChaseMovementComponent& operator=(ChaseMovementComponent&& other) = delete;


	};
}