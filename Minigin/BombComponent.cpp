#include "BombComponent.h"
#include "TimeManager.h"
#include "GridComponent.h"
#include "GameObject.h"
#include "PlaceBombComponent.h"
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
	m_IsDetonating = false;

	auto GridTransform = dynamic_cast<dae::GridTransform*>(GetOwner()->GetTransform());
	if (GridTransform == nullptr) return;

	auto gridPos = GridTransform->GetGridTiles();
	auto grid = GridTransform->GetGrid();


	//Affect Surrounding tiles
	auto tileLayout = grid->GetGridLayout();

	//Temporary
	int gridIndex = grid->GridToIndex(gridPos.x, gridPos.y-1);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft) 
	{
				grid->GetGridPtrs()[gridIndex]->MarkForDelete();
	}

	 gridIndex = grid->GridToIndex(gridPos.x, gridPos.y + 1);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{
		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
	}

	 gridIndex = grid->GridToIndex(gridPos.x - 1, gridPos.y);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{
		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
	}

	 gridIndex = grid->GridToIndex(gridPos.x + 1, gridPos.y);
	if (tileLayout[gridIndex] == GridComponent::GridValue::soft)
	{
		grid->GetGridPtrs()[gridIndex]->MarkForDelete();
	}

	std::cout << "Explode";

	if (m_Placer) m_Placer->UnlockCanPlace();

	GetOwner()->MarkForDelete();










}