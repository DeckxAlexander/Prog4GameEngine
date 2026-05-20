#pragma once
#include "ObjectComponent.h"

namespace dae
{
	class GridComponent;
	class PlaceBombComponent final : public ObjectComponent
	{
	private:
			GridComponent* m_pGrid{ nullptr };
			int m_CanPlace{ 1 };
			int m_Size{ 1 };




	public:
		virtual void Update() override {}
		virtual void Render() const override {}

		void AddSize(int size) { m_Size += size; }
		void PlaceBomb();
		void AddCanPlace(int value = 1) { m_CanPlace += value; }



		PlaceBombComponent(GameObject* pOwner, GridComponent* pGrid) : ObjectComponent(pOwner), m_pGrid{pGrid}
		{}
		~PlaceBombComponent() = default;
		PlaceBombComponent(const PlaceBombComponent& other) = delete;
		PlaceBombComponent(PlaceBombComponent&& other) = delete;
		PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
		PlaceBombComponent& operator=(PlaceBombComponent&& other) = delete;
	};
}