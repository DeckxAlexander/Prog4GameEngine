#include "PowerUpComponent.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include "PlayerComponent.h"
#include "SoundSystem.h"
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
	SoundSystemLocator::get_sound_system().PlaySound(4);
	GetOwner()->MarkForDelete();

}

dae::PowerUpComponent::PowerUpComponent() : ObjectComponent()
{
}

dae::PowerUpComponent::~PowerUpComponent()
{
	for (auto player : m_PlayersColliders) 
	{
		auto playerComp = player->GetOwner()->GetComponentByType<PlayerComponent>();
		if (playerComp != nullptr) playerComp->GetSubject()->RemoveObserver(this);
	}
}



void dae::PowerUpComponent::OnNotify(const Event& event)
{
	if (event.event == EventType::PlayerDead)
	{
		m_PlayersColliders.erase(std::remove(m_PlayersColliders.begin(), m_PlayersColliders.end(), event.sender->GetOwner()->GetComponentByType<CollisionComponent>()), m_PlayersColliders.end());

	}
}





void dae::PowerUpComponent::InitializePlayers()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	m_PlayersColliders.clear();
	auto players = scene.GetAllObjectsByComponent<PlayerComponent>();
	for (auto player : players) 
	{ 
		player->GetComponentByType<PlayerComponent>()->GetSubject()->AddObserver(this);
		auto collider = player->GetComponentByType<CollisionComponent>();
		if (collider != nullptr) m_PlayersColliders.push_back(collider);
	}
}
