#include "BombComponent.h"
#include "TimeManager.h"
#include "GridComponent.h"
#include "GameObject.h"
#include "PlaceBombComponent.h"
#include "SceneManager.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "SDLSoundSystem.h"
#include "PlayerComponent.h"
#include "GridTransform.h"
#include "GameManager.h"
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


void dae::BombComponent::Render() const 
{

}

void dae::BombComponent::Explode() 
{
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
	std::cout << "Explode";
	if (m_Placer) m_Placer->AddCanPlace();
	dae::SoundSystemLocator::get_sound_system().PlaySound(0);
	GetOwner()->MarkForDelete();


}

void dae::BombComponent::KillSurrounding(dae::GridTransform* gridTransform, glm::ivec4 dirSizes)
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	auto KillableObjects = scene.GetAllObjectsByComponent<HealthComponent>();

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

			if (CollisionComponent::CheckCollision(colliderRectHor, rect) || CollisionComponent::CheckCollision(colliderRectVer, rect)) obj->GetComponentByType<HealthComponent>()->ProcessDeath();

		}
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
		}
	}

	return{leftSize,upSize,rightSize,downSize};

}