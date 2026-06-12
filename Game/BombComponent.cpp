#include "BombComponent.h"
#include "TimeManager.h"
#include "GridComponent.h"
#include "GameObject.h"
#include "PlaceBombComponent.h"
#include "SceneManager.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "SoundSystem.h"
#include "PlayerComponent.h"
#include "GridTransform.h"
#include "GameManager.h"
#include "RenderComponent.h"
#include "LifetimeComponent.h"
#include <iostream>

void dae::BombComponent::Update()
{
	if(m_IsDetonating) 
	{
		if (m_TimePassed >= m_DetonationTime) 
		{
			Explode();
			return;
		}
		float deltaT{ TimeManager::GetInstance().GetDeltaTime() };
		m_TimePassed += deltaT;

	}
}



void dae::BombComponent::Explode() 
{
	if (m_IsExploding) return;
	m_IsExploding = true;
	m_IsDetonating = false;

	auto gridTransform = dynamic_cast<dae::GridTransform*>(GetOwner()->GetTransform());
	if (gridTransform == nullptr) return;

	auto gridPos = gridTransform->GetGridTiles();
	auto grid = gridTransform->GetGrid();


	//Affect Surrounding tiles
	auto sizes = BreakSoftBlocks(grid, gridPos);

	int gridIndex = grid->GridToIndex(gridPos);
	grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;

	KillSurrounding(gridTransform, sizes);
	SpawnParticles(sizes, grid->GetTileScale());
	std::cout << "Explode";
	if (m_Placer) m_Placer->AddCanPlace();
	dae::SoundSystemLocator::get_sound_system().PlaySound(0);
	GetOwner()->MarkForDelete();


}

dae::BombComponent::BombComponent(float detonationTime, PlaceBombComponent* placer, int size) : ObjectComponent(), m_DetonationTime{ detonationTime }, m_Placer{ placer }, m_Size{ size }
{}

void dae::BombComponent::KillSurrounding(dae::GridTransform* gridTransform, glm::ivec4 dirSizes)
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	auto KillableObjects = scene.GetAllObjectsByComponent<HealthComponent>();
	auto OtherBombs = scene.GetAllObjectsByComponent<BombComponent>();

	glm::vec4 colliderRectVer{
		gridTransform->GetPosition().x- gridTransform->GetGrid()->GetTileScale().x*0.5f,
		gridTransform->GetPosition().y - (gridTransform->GetGrid()->GetTileScale().y*0.5f) - (gridTransform->GetGrid()->GetTileScale().y * dirSizes.y),
		gridTransform->GetGrid()->GetTileScale().x,
		(dirSizes.y+1)* gridTransform->GetGrid()->GetTileScale().y + gridTransform->GetGrid()->GetTileScale().y* dirSizes.w
	};

	glm::vec4 colliderRectHor{
	gridTransform->GetPosition().x - (gridTransform->GetGrid()->GetTileScale().x * 0.5f) - (gridTransform->GetGrid()->GetTileScale().x * dirSizes.x),
	gridTransform->GetPosition().y - gridTransform->GetGrid()->GetTileScale().y*0.5f,
	(dirSizes.x + 1)* gridTransform->GetGrid()->GetTileScale().x + gridTransform->GetGrid()->GetTileScale().x *dirSizes.z,
	 gridTransform->GetGrid()->GetTileScale().y
	};

	for (auto obj : KillableObjects)
	{
		auto ColliderComp = obj->GetComponentByType<CollisionComponent>();
		if (ColliderComp != nullptr)
		{
			auto rect = ColliderComp->GetCollisionRect();

			if (CollisionComponent::CheckCollision(colliderRectHor, rect) || CollisionComponent::CheckCollision(colliderRectVer, rect)) 
			{ 
				obj->GetComponentByType<HealthComponent>()->ProcessDeath(); 
			}

		}
	}

	if (OtherBombs.size() > 1) 
	{
		for (auto obj : OtherBombs)
		{
			if (obj == GetOwner()) continue;
			auto ColliderComp = obj->GetComponentByType<CollisionComponent>();
			if (ColliderComp != nullptr)
			{
				auto rect = ColliderComp->GetCollisionRect();

				if (CollisionComponent::CheckCollision(colliderRectHor, rect) || CollisionComponent::CheckCollision(colliderRectVer, rect))
				{
					obj->GetComponentByType<BombComponent>()->Explode();
				}

			}
		}
	}

}

void dae::BombComponent::SpawnParticles(glm::ivec4 positions, glm::vec2 size)
{

	auto& scene = SceneManager::GetInstance().GetActiveScene();
	auto positionOwner = GetOwner()->GetWorldPosition();
	auto particleObject = std::make_unique<GameObject>();
	particleObject->AddComponent(std::make_unique<RenderComponent>( "Textures/BombParticles/ExplosionCenter.png"));
	particleObject->AddComponent(std::make_unique<LifetimeComponent>(0.5f));
	particleObject->SetPosition(positionOwner);
	particleObject->SetScale(2.0f, 2.0f);
	scene.Add(std::move(particleObject));

	//Left 
	for(int index{1}; index <= positions.x; index++) 
	{
		auto position = positionOwner;
		position.x -= size.x * index;
		particleObject = std::make_unique<GameObject>();
		std::string filename = "Textures/BombParticles/ExplosionLeft.png";
		if (index == positions.x) filename = "Textures/BombParticles/ExplosionLeftEnd.png";
		particleObject->AddComponent(std::make_unique<RenderComponent>( filename));
		particleObject->AddComponent(std::make_unique<LifetimeComponent>(0.5f));
		particleObject->SetPosition(position);
		particleObject->SetScale(2.0f, 2.0f);
		scene.Add(std::move(particleObject));
	}

	//Up
	for (int index{ 1 }; index <= positions.y; index++)
	{
		auto position = positionOwner;
		position.y -= size.y * index;
		particleObject = std::make_unique<GameObject>();
		std::string filename = "Textures/BombParticles/ExplosionTop.png";
		if (index == positions.y) filename = "Textures/BombParticles/ExplosionTopEnd.png";
		particleObject->AddComponent(std::make_unique<RenderComponent>( filename));
		particleObject->AddComponent(std::make_unique<LifetimeComponent>(0.5f));
		particleObject->SetPosition(position);
		particleObject->SetScale(2.0f, 2.0f);
		scene.Add(std::move(particleObject));
	}

	//Right
	for (int index{ 1 }; index <= positions.z; index++)
	{
		auto position = positionOwner;
		position.x += size.x * index;
		particleObject = std::make_unique<GameObject>();
		std::string filename = "Textures/BombParticles/ExplosionRight.png";
		if (index == positions.z) filename = "Textures/BombParticles/ExplosionRightEnd.png";
		particleObject->AddComponent(std::make_unique<RenderComponent>( filename));
		particleObject->AddComponent(std::make_unique<LifetimeComponent>(0.5f));
		particleObject->SetPosition(position);
		particleObject->SetScale(2.0f, 2.0f);
		scene.Add(std::move(particleObject));
	}

	//Down
	for (int index{ 1 }; index <= positions.w; index++)
	{
		auto position = positionOwner;
		position.y += size.y * index;
		particleObject = std::make_unique<GameObject>();
		std::string filename = "Textures/BombParticles/ExplosionBottom.png";
		if (index == positions.w) filename = "Textures/BombParticles/ExplosionBottomEnd.png";
		particleObject->AddComponent(std::make_unique<RenderComponent>( filename));
		particleObject->AddComponent(std::make_unique<LifetimeComponent>(0.5f));
		particleObject->SetPosition(position);
		particleObject->SetScale(2.0f, 2.0f);
		scene.Add(std::move(particleObject));
	}

}




glm::ivec4 dae::BombComponent::BreakSoftBlocks(dae::GridComponent* grid, glm::ivec2 gpos)
{
	auto tileLayout = grid->GetGridLayout();

	//Up 
	int upSize{};
	for (int index{1}; index <= m_Size; index++) 
	{
		int gridIndex = grid->GridToIndex(gpos.x, gpos.y - index);
		if (gridIndex >= tileLayout.size() || gridIndex < 0) continue;
		if (tileLayout[gridIndex] == GridComponent::GridValue::hard) break;
		upSize++;
		if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
		{
			grid->GetGridPtrs()[gridIndex]->MarkForDelete();
			grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
			break;

		}
	}

	//Down
	int downSize{};
	for (int index{ 1 }; index <= m_Size; index++)
	{
		int gridIndex = grid->GridToIndex(gpos.x, gpos.y + index);
		if (gridIndex >= tileLayout.size() || gridIndex < 0) continue;
		if (tileLayout[gridIndex] == GridComponent::GridValue::hard) break;
		downSize++;
		if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
		{
			grid->GetGridPtrs()[gridIndex]->MarkForDelete();
			grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
			break;
		}
	}

	//Right
	int rightSize{};
	for (int index{ 1 }; index <= m_Size; index++)
	{
		int gridIndex = grid->GridToIndex(gpos.x+index, gpos.y);
		if (gridIndex >= tileLayout.size() || gridIndex < 0) continue;
		if (tileLayout[gridIndex] == GridComponent::GridValue::hard) break;
		rightSize++;
		if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
		{
			grid->GetGridPtrs()[gridIndex]->MarkForDelete();
			grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
			break;
		}
	}

	//Right
	int leftSize{};
	for (int index{ 1 }; index <= m_Size; index++)
	{
		int gridIndex = grid->GridToIndex(gpos.x - index, gpos.y);
		if (gridIndex >= tileLayout.size() || gridIndex < 0) continue;
		if (tileLayout[gridIndex] == GridComponent::GridValue::hard) break;
		leftSize++;
		if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
		{
			grid->GetGridPtrs()[gridIndex]->MarkForDelete();
			grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
			break;
		}
	}

	return{leftSize,upSize,rightSize,downSize};

}