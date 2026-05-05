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


		std::vector<GameObject*>& GetGridPtrs() { return m_GridPointers; }
		std::vector<GridValue>& GetGridLayout() { return m_GridLayout; }

		glm::vec2 GetTileScale() const
		{
			return m_TileScale;
		}


		int GridToIndex(glm::ivec2 pos) { return GridToIndex(pos.x, pos.y); }
		int GridToIndex(int x, int y)
		{
			return y * m_Colums + x;
		}

		glm::ivec2 WorldPosToTile(const glm::vec3& worldPos) const;

		void SetSoftBlocksAmount(int amount) { m_SoftBlocksAmount = amount; }

		int GetColums() { return m_Colums; }

		GridComponent(GameObject* pOwner, int colums, int rows);
		~GridComponent() {  };
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;
	private:
		//Grid
		int m_Colums;
		int m_Rows;
		glm::vec2 m_TileScale{ 1.f,1.f };

		bool m_ShowDebug{ false };


		//GridObjects

		int m_SoftBlocksAmount{ 100 };
		std::vector<GridValue> m_GridLayout{};
		std::vector<GameObject*> m_GridPointers{};

	};


	class GridLocator final
	{
	private:
		static GridComponent* m_GridInstance;
	public:
		static GridComponent* GetGrid() { return m_GridInstance; }
		static void SetGrid(GridComponent*  ss) { m_GridInstance = ss; }
	};
}