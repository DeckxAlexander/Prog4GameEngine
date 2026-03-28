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
	public:
		virtual void Update() override;
		virtual void Render() const override;

		void SetTileScale(float x, float y);

		glm::vec2 GetTileScale() const
		{
			return m_TileScale;
		}

		GridComponent(GameObject* pOwner, int colums, int rows);
		~GridComponent() = default;
		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;
	};
}