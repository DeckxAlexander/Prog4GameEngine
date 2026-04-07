#pragma once
#include <glm/glm.hpp>
#include "ObjectComponent.h"


namespace dae
{

	class GridComponent final : public ObjectComponent
	{
	private:
		//Grid
		int m_Colums;
		int m_Rows;
		glm::vec2 m_TileScale{1.f,1.f };

		bool m_ShowDebug{ true };


		//GridObjects
		enum class GridValue {
			empty, hard, soft, spawn
		};

		int m_SoftBlocksAmount{75};
		std::vector<GridValue> m_GridLayout{};
		std::vector<GameObject*> m_GridPointers{};




	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetTileScale(float x, float y);
		void SetupGrid();
		void SpawnGrid();


		glm::vec2 GetTileScale() const
		{
			return m_TileScale;
		}

		int GridToIndex(int x, int y)
		{
			return y * m_Colums + x;
		}

		void SetSoftBlocksAmount(int amount) { m_SoftBlocksAmount = amount; }

		GridComponent(GameObject* pOwner, int colums, int rows);
		~GridComponent() = default;
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;
	};
}