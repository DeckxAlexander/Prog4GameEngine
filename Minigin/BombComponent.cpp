#include "BombComponent.h"
#include "TimeManager.h"
#include "GridComponent.h"
#include "GameObject.h"
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
	for (int xIndex{gridPos.x - 1}; xIndex <= gridPos.x + 1; xIndex++) 
	{
		for (int yIndex{ gridPos.y - 1 }; yIndex <= gridPos.y + 1; yIndex++) 
		{
			int gridIndex = grid->GridToIndex(xIndex, yIndex);
			if (tileLayout[gridIndex] == GridComponent::GridValue::soft) 
			{
				grid->GetGridPtrs()[gridIndex]->MarkForDelete();
			}

		}
	}

	std::cout << "Explode";

	GetOwner()->MarkForDelete();










}