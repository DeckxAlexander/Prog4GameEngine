#include "ExitComponent.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include "PlayerComponent.h"
#include "GameManager.h"

void dae::ExitComponent::Start()
{
	InitializePlayers();
	m_Collider = GetOwner()->GetComponentByType<CollisionComponent>();
}

void dae::ExitComponent::Update()
{
	for (auto player : m_PlayerColliders)
	{
		auto a = m_Collider->GetCollisionRect();
		auto b = player->GetCollisionRect();
		if (CollisionComponent::CheckCollision(a, b))
		{
			if (m_CheckActive) return;
			m_CheckActive = true;
			GameManager::GetInstance().CheckGameState();
			return;
		}
	}
	m_CheckActive = false;
}

void dae::ExitComponent::InitializePlayers()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	m_PlayerColliders.clear();
	auto players = scene.GetAllObjectsByComponent<PlayerComponent>();
	for (auto player : players)
	{
		player->GetComponentByType<PlayerComponent>()->GetSubject()->AddObserver(this);
		auto collider = player->GetComponentByType<CollisionComponent>();
		if (collider != nullptr) m_PlayerColliders.push_back(collider);
	}
}

dae::ExitComponent::ExitComponent() : ObjectComponent(nullptr)
{
}

dae::ExitComponent::~ExitComponent()
{
	for (auto player : m_PlayerColliders)
	{
		auto playerComp = player->GetOwner()->GetComponentByType<PlayerComponent>();
		if (playerComp != nullptr) playerComp->GetSubject()->RemoveObserver(this);
	}
}

void dae::ExitComponent::OnNotify(const Event & event)
{
	if (event.event == EventType::PlayerDead)
	{
		m_PlayerColliders.erase(std::remove(m_PlayerColliders.begin(), m_PlayerColliders.end(), event.sender->GetOwner()->GetComponentByType<CollisionComponent>()), m_PlayerColliders.end());

	}
}


