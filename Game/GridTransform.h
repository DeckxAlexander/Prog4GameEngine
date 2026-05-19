#pragma once
#include "Transform.h"

namespace dae 
{
	class GridComponent;
	class GridTransform final : public Transform
	{
	public:
		GridTransform(GridComponent* grid) : Transform{ nullptr }, m_pGrid{ grid }
		{
		}

		~GridTransform() = default;

		static float SnapToGrid(float value, float tileSize)
		{
			return (floorf(value / tileSize) * tileSize) + tileSize * 0.5f;
		}



		void SetGridTile(int tileX, int tileY);

		glm::ivec2 GetGridTiles();

		virtual void UpdateWorldPosition() override;

		GridComponent* GetGrid() { return m_pGrid; }



	protected:
		GridComponent* m_pGrid;


	};
}