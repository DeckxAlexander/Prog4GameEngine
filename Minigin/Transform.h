#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class Transform
	{
	public:
		Transform(GameObject* owner) : m_pOwner{ owner }, m_scale{ 1.f,1.f,1.f }, m_LocalPosition{ 0.f,0.f,0.f }, m_WorldPosition{ 0.f,0.f,0.f }, m_PositionIsDirty{ true }
		{
		}

		virtual ~Transform() = default;

		void SetScale(float x, float y, float z = 0);
		void SetScale(const glm::vec3& scale);
		const glm::vec3& GetScale() const { return m_scale; }
		void SetLocalPosition(const glm::vec3& position);
		void SetPosition(const glm::vec3& position) { SetLocalPosition(position); }; //SetPosition still used as SetLocalPosition
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetPosition() { return GetWorldPosition(); } //GetPosition still used as GetWorldPosition
		virtual void UpdateWorldPosition();
		void SetPositionDirty();

		void SetOwner(GameObject* owner)
		{
			m_pOwner = owner;
		}

	protected:
		GameObject* m_pOwner;

		glm::vec3 m_scale;
		glm::vec3 m_LocalPosition;
		glm::vec3 m_WorldPosition;
		bool m_PositionIsDirty;

	};




}
