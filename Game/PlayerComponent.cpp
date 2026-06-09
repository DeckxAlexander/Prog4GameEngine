#include "PlayerComponent.h"
#include "EventManagers.h"
#include "SceneManager.h"
#include "EnemyComponent.h"
#include "GameManager.h"
#include <memory>


dae::PlayerComponent::PlayerComponent(GameObject* pOwner, int index) : ObjectComponent(pOwner), m_PlayerIndex{ index }, m_Subject{std::make_unique<Subject>()}
{
	
}

void dae::PlayerComponent::Respawn()
{
	GetOwner()->SetPosition(50.f, 50.f);
}

void dae::PlayerComponent::Start()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	auto enemies = scene.GetAllObjectsByComponent<EnemyComponent>();

	for(auto enemy : enemies) 
	{
		auto enemyComp = enemy->GetComponentByType<EnemyComponent>();
		m_Subject->AddObserver(enemyComp);
	}
}




void dae::PlayerComponent::PlayerDeath()
{
	m_PlayerLives--;

	if (m_PlayerLives <= 0) 
	{ 
		m_Subject->Notify(Event{ EventType::PlayerDead, this }); 
		GameManager::GetInstance().CheckPlayerDeath();
		GetOwner()->MarkForDelete();
	}
	else 
	{
		Respawn();
	}
	


}