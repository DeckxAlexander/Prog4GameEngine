#pragma once
#include "ObjectComponent.h"
#include <glm/glm.hpp>

namespace dae
{


	class CollisionComponent final : public ObjectComponent
	{
	private:
		glm::vec4 m_CollisionRect{}; //Left, Top, Width, Height
		bool m_CollisionRectIsDirty{ true };


		void UpdateCollisionRect();

	public:
		virtual void Update() override {};
		virtual void Render() const override;

		glm::vec4 GetCollisionRect();
		void SetCollisionRectDirty() { m_CollisionRectIsDirty = true; }
		static bool CheckCollision(CollisionComponent* collisionA, CollisionComponent* collisionB);
		static bool CheckCollision(glm::vec4 collisionRectA, glm::vec4 collisionRectB);


		CollisionComponent(GameObject* pOwner, float width, float height);
		~CollisionComponent();
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;
	};
}