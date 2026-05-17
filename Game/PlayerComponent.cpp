#include "PlayerComponent.h"
#include "EventManagers.h"
#include "SceneManager.h"
#include "EnemyComponent.h"
#include <memory>


dae::PlayerComponent::PlayerComponent(GameObject* pOwner, int index) : ObjectComponent(pOwner), m_PlayerIndex{ index }, m_Subject{std::make_unique<Subject>()}
{
	
}

void dae::PlayerComponent::Start() 
{
	auto& scene = dae::SceneManager::GetInstance().GetScene(0);
	auto enemies = scene.GetAllObjectsByComponent<EnemyComponent>();

	for(auto enemy : enemies) 
	{
		auto enemyComp = enemy->GetComponentByType<EnemyComponent>();
		m_Subject->AddObserver(enemyComp);
	}
}




void dae::PlayerComponent::PlayerDeath()
{
	if (m_PlayerIndex == 0) m_Subject->Notify(Event{ EventType::PlayerDead, this });
	GetOwner()->MarkForDelete();


}