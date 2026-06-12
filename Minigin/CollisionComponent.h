#pragma once
#include "ObjectComponent.h"
#include <algorithm>
#include <glm/glm.hpp>

namespace dae
{


	class CollisionComponent final : public ObjectComponent
	{
	private:
		glm::vec4 m_CollisionRect{}; //Left, Top, Width, Height
		bool m_CollisionRectIsDirty{ true };
		char m_Tag;

		std::vector<char> m_BlockingTags{};

		void UpdateCollisionRect();

	public:
		virtual void Update() override {};
		virtual void Render() const override;

		char GetTag() const {return m_Tag;}
		void SetTag(char val) { m_Tag = val; }

		void AddBlockingTag(char val) { m_BlockingTags.emplace_back(val); }
		void RemoveBlockingTag(char val) 
		{ 
			m_BlockingTags.erase(std::remove(m_BlockingTags.begin(), m_BlockingTags.end(), val), m_BlockingTags.end());
		}
		glm::vec4 GetCollisionRect();
		void SetCollisionRectDirty() { m_CollisionRectIsDirty = true; }
		static bool CheckBlockingCollision(CollisionComponent* collisionA, CollisionComponent* collisionB);
		static bool CheckCollision(glm::vec4 collisionRectA, glm::vec4 collisionRectB);

		


		CollisionComponent(float width, float height, char tag);
		~CollisionComponent();
		CollisionComponent(const CollisionComponent& other) = delete;
		CollisionComponent(CollisionComponent&& other) = delete;
		CollisionComponent& operator=(const CollisionComponent& other) = delete;
		CollisionComponent& operator=(CollisionComponent&& other) = delete;
	};
}