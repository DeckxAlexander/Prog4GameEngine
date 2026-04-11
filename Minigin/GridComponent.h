#pragma once
#include <glm/glm.hpp>
#include "ObjectComponent.h"


namespace dae
{

	class GridComponent final : public ObjectComponent
	{
	public:
		enum class GridValue {
			empty, hard, soft, spawn
		};

		virtual void Update() override;
		virtual void Render() const override;

		void SetTileScale(float x, float y);
		void SetupGrid();
		void SpawnGrid();


		const std::vector<GameObject*>& GetGridPtrs() { return m_GridPointers; }
		const std::vector<GridValue>& GetGridLayout() { return m_GridLayout; }

		glm::vec2 GetTileScale() const
		{
			return m_TileScale;
		}

		int GridToIndex(int x, int y)
		{
			return y * m_Colums + x;
		}

		void SetSoftBlocksAmount(int amount) { m_SoftBlocksAmount = amount; }

		int GetColums() { return m_Colums; }

		GridComponent(GameObject* pOwner, int colums, int rows);
		~GridComponent() = default;
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;
	private:
		//Grid
		int m_Colums;
		int m_Rows;
		glm::vec2 m_TileScale{ 1.f,1.f };

		bool m_ShowDebug{ true };


		//GridObjects

		int m_SoftBlocksAmount{ 100 };
		std::vector<GridValue> m_GridLayout{};
		std::vector<GameObject*> m_GridPointers{};

	};
}