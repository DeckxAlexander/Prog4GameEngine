#include "EnemyComponent.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include "States.h"
#include "GridComponent.h"
#include "MovementComponent.h"
#include "PlayerComponent.h"
#include "GameManager.h"
#include "HealthComponent.h"
#include <memory>
#include <iostream>
#include <vector>


void dae::EnemyComponent::InitializePlayers() 
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	m_Players.clear();
	m_Players = scene.GetAllObjectsByComponent<PlayerComponent>();
}



void dae::EnemyComponent::SetState(std::unique_ptr<State> state)
{
	if (m_State.get() != nullptr) m_State.get()->End(); //old
	m_State = std::move(state);
	if (m_State.get() != nullptr) m_State.get()->Start(GetOwner()); //new
}


bool dae::EnemyComponent::CanSeePlayer(GameObject* obj)
{
	auto grid = GridLocator::GetGrid();

	auto enemyGridPosition = grid->WorldPosToTile(GetOwner()->GetWorldPosition());



	auto playerGridPosition = grid->WorldPosToTile(obj->GetWorldPosition());
	if (grid->GetGridLayout()[grid->GridToIndex(playerGridPosition.x, playerGridPosition.y)] == GridComponent::GridValue::bomb) 
		return false; //If player is standing on a bomb he will not be count as seen!
	


	if (playerGridPosition == enemyGridPosition) //On same grid tile
	{
		return true;
	}

	//Check Vertical
	if (playerGridPosition.x == enemyGridPosition.x)
	{
		int step = (playerGridPosition.y > enemyGridPosition.y) ? 1 : -1;
		for (int y = enemyGridPosition.y + step; y != playerGridPosition.y; y += step)
		{
			if (grid->GetGridLayout()[grid->GridToIndex(enemyGridPosition.x, y)] == GridComponent::GridValue::hard ||
				grid->GetGridLayout()[grid->GridToIndex(enemyGridPosition.x, y)] == GridComponent::GridValue::soft || 
				grid->GetGridLayout()[grid->GridToIndex(enemyGridPosition.x, y)] == GridComponent::GridValue::bomb)
			{
				return false;
			}
		}
	}
	else if (playerGridPosition.y == enemyGridPosition.y)//Check Horizontal
	{
		int step = (playerGridPosition.x > enemyGridPosition.x) ? 1 : -1;
		for (int x = enemyGridPosition.x + step; x != playerGridPosition.x; x += step)
		{
			if (grid->GetGridLayout()[grid->GridToIndex(x, enemyGridPosition.y)] == GridComponent::GridValue::hard ||
				grid->GetGridLayout()[grid->GridToIndex(x, enemyGridPosition.y)] == GridComponent::GridValue::soft || 
				grid->GetGridLayout()[grid->GridToIndex(x, enemyGridPosition.y)] == GridComponent::GridValue::bomb)
			{
				return false;
			}
		}
	}
	else 
	{ 
		return false;
	}
	return true;

}

void dae::EnemyComponent::GiveUpChase() 
{

	SetState(std::make_unique<SearchWanderState>());
	

}

void dae::EnemyComponent::EnemyDeath()
{
	GameManager::GetInstance().AddScore(m_ScoreAmount);
	GetOwner()->MarkForDelete();
}


void dae::EnemyComponent::SearchPlayer()
{
	auto grid = GridLocator::GetGrid();

	auto enemyGridPosition = grid->WorldPosToTile(GetOwner()->GetWorldPosition());

	for (auto player : m_Players) 
	{

		bool found = CanSeePlayer(player);
		if (found == true) 
		{
			std::cout << "Seen";
			SetState(std::make_unique<ChaseState>(player));
			return;
		}

	}




}

dae::EnemyComponent::EnemyComponent( IntelligenceType intelligenceType, int scoreAmount) : ObjectComponent(), m_ScoreAmount{ scoreAmount }, m_IntelligenceType{ intelligenceType }
{
	InitializePlayers();
}

dae::EnemyComponent::~EnemyComponent()
{
	for (auto player : m_Players)
	{
		auto playerComp = player->GetComponentByType<PlayerComponent>();
		if (playerComp != nullptr) 
		{
			playerComp->GetSubject()->RemoveObserver(this);
		}

	}
}



void dae::EnemyComponent::Start()
{
	if (m_IntelligenceType == IntelligenceType::normal) m_State = std::make_unique<IdleWanderState>();
	if (m_IntelligenceType== IntelligenceType::smart) m_State = std::make_unique<SearchWanderState>();
	if (m_IntelligenceType== IntelligenceType::player) m_State = std::make_unique<PlayerControlledState>();
	m_State->Start(GetOwner());


}

void dae::EnemyComponent::Update()
{
	auto enemyCollider = GetOwner()->GetComponentByType<CollisionComponent>(); //TODO Should Change
	for(auto player : m_Players)
	{
		auto playerCollider = player->GetComponentByType<CollisionComponent>();
		if (playerCollider == nullptr) continue;

		auto a = enemyCollider->GetCollisionRect();
		auto b = playerCollider->GetCollisionRect();
		if (CollisionComponent::CheckCollision(a, b)) 
		{ 
			player->GetComponentByType<PlayerComponent>()->PlayerDeath();
			break;
		}

	}
	if (m_State.get() != nullptr) 
	{
		auto returnstate = m_State->Update(); 
		if (returnstate != nullptr) SetState(std::move(returnstate));
	}
	
	
}

void dae::EnemyComponent::OnNotify(const Event& event) 
{
	if (event.event == EventType::PlayerDead) 
	{
		m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), event.sender->GetOwner()), m_Players.end());
		
		auto possibleChaseState = dynamic_cast<ChaseState*>(m_State.get());
		if (possibleChaseState) 
		{
			SetState(std::make_unique<SearchWanderState>());
		}
	}

}



