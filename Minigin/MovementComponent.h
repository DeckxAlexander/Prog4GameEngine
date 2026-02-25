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


		glm::vec3 m_Center;
		

	public:
		virtual void Update() override;
		virtual void Render() const override;
		void SetCenterPosition(float x, float y) { m_Center = { x,y,0.f }; }
		void SetCenterPosition(const glm::vec3& position) { m_Center = position; }


		MovementComponent(GameObject* pOwner);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;
	};
}