#include "GridTransform.h"
#include "GameObject.h"
#include "CollisionComponent.h"
#include "GridComponent.h"

void dae::GridTransform::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		GameObject* pParent = m_pOwner->GetParent();

		if (pParent == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = pParent->GetWorldPosition() + m_LocalPosition;
		}

		m_WorldPosition.x = SnapToGrid(m_WorldPosition.x, m_pGrid->GetTileScale().x);
		m_WorldPosition.y = SnapToGrid(m_WorldPosition.y, m_pGrid->GetTileScale().y);

	}
	m_PositionIsDirty = false;

}


void dae::GridTransform::SetGridTile(int tileX, int tileY)
{
	glm::vec3 screenPos{};
	screenPos.x = tileX * m_pGrid->GetTileScale().x;
	screenPos.x += m_pGrid->GetTileScale().x * 0.5f;


	screenPos.y = tileY * m_pGrid->GetTileScale().y;
	screenPos.y += m_pGrid->GetTileScale().y * 0.5f;

	SetLocalPosition(screenPos);

}

glm::ivec2 dae::GridTransform::GetGridTiles()
{
	glm::ivec2 tile{};
	auto pos = GetWorldPosition();

	tile.x = static_cast<int>(pos.x / m_pGrid->GetTileScale().x);
	tile.y = static_cast<int>(pos.y / m_pGrid->GetTileScale().y);

	return tile;
}