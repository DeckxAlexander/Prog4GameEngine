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
	BreakSoftBlocks(grid, gridPos);

	int gridIndex = grid->GridToIndex(gridPos);
	grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;

	KillSurrounding(gridTransform);
	std::cout << "Explode";
	if (m_Placer) m_Placer->UnlockCanPlace();
	dae::SoundSystemLocator::get_sound_system().PlaySound(0);
	GetOwner()->MarkForDelete();


}

void dae::BombComponent::KillSurrounding(dae::GridTransform* gridTransform) 
{
	auto& scene = dae::SceneManager::GetInstance().GetScene(0);
	auto KillableObjects = scene.GetAllObjectsByComponent<HealthComponent>();
	glm::vec4 colliderRectVer{
		gridTransform->GetPosition().x- gridTransform->GetGrid()->GetTileScale().x*0.5f,
		gridTransform->GetPosition().y - gridTransform->GetGrid()->GetTileScale().y*1.5f,
		gridTransform->GetGrid()->GetTileScale().x,
		3 * gridTransform->GetGrid()->GetTileScale().y
	};

	glm::vec4 colliderRectHor{
	gridTransform->GetPosition().x - gridTransform->GetGrid()->GetTileScale().x*1.5f,
	gridTransform->GetPosition().y - gridTransform->GetGrid()->GetTileScale().y*0.5f,
	3 * gridTransform->GetGrid()->GetTileScale().x,
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


void dae::BombComponent::BreakSoftBlocks(dae::GridComponent* grid, glm::ivec2 gpos) 
{
	auto tileLayout = grid->GetGridLayout();

	//Temporary
	int gridIndex = grid->GridToIndex(gpos.x, gpos.y - 1);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{
		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
		grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
	}

	gridIndex = grid->GridToIndex(gpos.x, gpos.y + 1);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{
		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
		grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
	}

	gridIndex = grid->GridToIndex(gpos.x - 1, gpos.y);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{
		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
		grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
	}

	gridIndex = grid->GridToIndex(gpos.x + 1, gpos.y);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{

		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
		grid->GetGridLayout()[gridIndex] = GridComponent::GridValue::empty;
	}
}