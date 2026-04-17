#include "EnemyComponent.h"
#include "SceneManager.h"
#include "CollisionComponent.h"


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
		auto a = enemyCollider->GetCollisionRect();
		auto b = playerCollider->GetCollisionRect();
		if (CollisionComponent::CheckCollision(a, b)) 
		{ 
			player->MarkForDelete(); 
			break;
		}

	}
	
}