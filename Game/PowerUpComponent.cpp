#include "PowerUpComponent.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include "PlayerComponent.h"
#include <iostream>

void dae::PowerUpComponent::Start()
{
	InitializePlayers();
	m_Collider = GetOwner()->GetComponentByType<CollisionComponent>();

	

}

void dae::PowerUpComponent::Update()
{
	if (m_IsActive) return;

	for (auto player : m_PlayersColliders)
	{ 

		auto a = m_Collider->GetCollisionRect();
		auto b = player->GetCollisionRect();
		if (CollisionComponent::CheckCollision(a, b))
		{
			std::cout << "Activated";
			ActivatePowerUp(player->GetOwner());
			break;
		}

	}
}

void dae::PowerUpComponent::ActivatePowerUp(GameObject*)
{
	m_IsActive = true;
	GetOwner()->MarkForDelete();

}

dae::PowerUpComponent::PowerUpComponent(GameObject* pOwner) : ObjectComponent(pOwner)
{
}

dae::PowerUpComponent::~PowerUpComponent()
{

}



void dae::PowerUpComponent::OnNotify(const Event& event)
{
	if (event.event == EventType::PlayerDead)
	{
		m_PlayersColliders.erase(std::remove(m_PlayersColliders.begin(), m_PlayersColliders.end(), event.sender->GetOwner()->GetComponentByType<CollisionComponent>()), m_PlayersColliders.end());

	}
}

void dae::PowerUpComponent::OnSubjectDestroyed(Subject* subject)
{
	subject->RemoveObserver(this);

}



void dae::PowerUpComponent::InitializePlayers()
{
	auto& scene = dae::SceneManager::GetInstance().GetScene(0);
	m_PlayersColliders.clear();
	auto players = scene.GetAllObjectsByComponent<PlayerComponent>();
	for (auto player : players) 
	{ 
		player->GetComponentByType<PlayerComponent>()->GetSubject()->AddObserver(this);
		auto collider = player->GetComponentByType<CollisionComponent>();
		if (collider != nullptr) m_PlayersColliders.push_back(collider);
	}
}
