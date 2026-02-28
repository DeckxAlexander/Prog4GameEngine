#pragma once
#include "ObjectComponent.h"
#include <glm/glm.hpp>
//#define PI 3.14159265358979323846

namespace dae
{


	class MovementComponent final : public ObjectComponent
	{
	private:
		float m_Angle;
		float m_Speed;
		float m_Radius;

		

	public:
		virtual void Update() override;
		virtual void Render() const override;

		MovementComponent(GameObject* pOwner);
		MovementComponent(GameObject* pOwner, float speed, float radius);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;
	};
}