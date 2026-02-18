#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);

		void SetScale(float x, float y, float z=0);
		void SetScale(const glm::vec3& scale);
		const glm::vec3& GetScale() const { return m_scale; }

	private:
		glm::vec3 m_position;
		glm::vec3 m_scale{1.f,1.f,1.f}; //!Q is this good or should i rather make a whole constructor for this?

	};
}
