#include "EnemyComponent.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include <vector>


void dae::EnemyComponent::InitializePlayers() 
{
	auto& scene = dae::SceneManager::GetInstance().GetScene(0);
	m_Players.clear();
	m_Players = scene.GetAllObjectsByComponent<PlayerComponent>();
}

void dae::EnemyComponent::Update()
{
	auto enemyCollider = GetOwner()->GetComponentByType<CollisionComponent>();
	for(auto player : m_Players)
	{
		auto playerCollider = player->GetComponentByType<CollisionComponent>();
		if (playerCollider == nullptr) continue;

		auto a = enemyCollider->GetCollisionRect();
		auto b = playerCollider->GetCollisionRect();
		if (CollisionComponent::CheckCollision(a, b)) 
		{ 
			m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), player), m_Players.end());
			player->MarkForDelete(); 
			break;
		}

	}
	
}