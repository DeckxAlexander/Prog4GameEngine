#pragma once
#include "ObjectComponent.h"
#include <glm/glm.hpp>
//#define PI 3.14159265358979323846

namespace dae
{
	class MovementComponent : public ObjectComponent
	{
	protected:
		glm::vec3 m_Velocity{};
		float m_Speed{};
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

		MovementComponent(GameObject* pOwner, float speed);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;
	};

	class AIMovementComponent final : public MovementComponent
	{
	private:
		glm::vec3 m_DesiredVelocity{};
		GridComponent* m_pGrid;

		glm::vec3 FindNewDirection();

	public:
		virtual void Update() override;
		virtual void HitCollider() 
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

		AIMovementComponent(GameObject* pOwner, float speed, GridComponent* pGrid = nullptr);
		~AIMovementComponent() = default;
		AIMovementComponent(const AIMovementComponent& other) = delete;
		AIMovementComponent(AIMovementComponent&& other) = delete;
		AIMovementComponent& operator=(const AIMovementComponent& other) = delete;
		AIMovementComponent& operator=(AIMovementComponent&& other) = delete;
	};
}