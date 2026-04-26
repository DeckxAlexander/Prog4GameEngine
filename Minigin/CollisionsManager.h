#pragma once
#include "Singleton.h"
#include <vector>

namespace dae
{
	class CollisionComponent;
	class CollisionsManager final : public Singleton<CollisionsManager>
	{
	public:
		const std::vector<CollisionComponent*>& GetColliders() const { return m_Collisions; }
		void AddCollision(CollisionComponent* coll) { m_Collisions.push_back(coll); }
		void RemoveCollision(CollisionComponent* coll) 
		{ 
			if (m_Collisions.size() > 0) m_Collisions.erase(std::remove(m_Collisions.begin(), m_Collisions.end(), coll), m_Collisions.end()); 
		}
	private:
		friend class Singleton<CollisionsManager>;
		CollisionsManager() = default;
		std::vector<CollisionComponent*> m_Collisions{};
		


	};
}