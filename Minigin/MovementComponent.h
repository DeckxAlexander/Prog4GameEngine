#pragma once
#include "ObjectComponent.h"
#include <glm/glm.hpp>
//#define PI 3.14159265358979323846

namespace dae
{


	class MovementComponent final : public ObjectComponent
	{
	private:
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

		MovementComponent(GameObject* pOwner, float speed);
		~MovementComponent() = default;
		MovementComponent(const MovementComponent& other) = delete;
		MovementComponent(MovementComponent&& other) = delete;
		MovementComponent& operator=(const MovementComponent& other) = delete;
		MovementComponent& operator=(MovementComponent&& other) = delete;
	};
}