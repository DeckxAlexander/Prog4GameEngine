#pragma once
#include <glm/glm.hpp>
#include "ObjectComponent.h"


namespace dae
{

	class GridComponent final : public ObjectComponent
	{
	private:
		int m_Colums;
		int m_Rows;

		glm::vec2 m_TileScale{1.f,1.f };

		bool m_ShowDebug{ true };

		enum class GridValue {
			empty, hard, soft
		};


		std::vector<GridValue> m_GridLayout{};
		std::vector<GameObject*> m_GridPointers{};


	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetTileScale(float x, float y);
		void SpawnGrid();

		glm::vec2 GetTileScale() const
		{
			return m_TileScale;
		}

		int GridToIndex(int x, int y)
		{
			return y * m_Colums + x;
		}

		GridComponent(GameObject* pOwner, int colums, int rows);
		~GridComponent() = default;
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;
	};
}