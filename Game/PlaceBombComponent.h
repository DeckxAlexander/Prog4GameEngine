#pragma once
#include "ObjectComponent.h"

namespace dae
{
	class GridComponent;
	class BombComponent;
	class PlaceBombComponent final : public ObjectComponent
	{
	private:
			GridComponent* m_pGrid{ nullptr };
			int m_CanPlace{ 1 };
			int m_CanPlaceMax{ 1 };
			int m_Size{ 1 };
			bool m_UsingDetonator{ false };

			BombComponent* m_OldestBomb{};


	public:
		virtual void Update() override {}
		virtual void Render() const override {}

		void AddSize(int size) { m_Size += size; }
		void PlaceBomb();
		void AddCanPlace(int value = 1) {m_CanPlace += value; }
		void AddMaxCanPlace(int value = 1) { m_CanPlaceMax += value; }
		void SetUsingDetonator(bool value) { m_UsingDetonator = value; }

		void ForceDetonate();

		PlaceBombComponent(GridComponent* pGrid) : ObjectComponent(), m_pGrid{pGrid}
		{}
		~PlaceBombComponent() = default;
		PlaceBombComponent(const PlaceBombComponent& other) = delete;
		PlaceBombComponent(PlaceBombComponent&& other) = delete;
		PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
		PlaceBombComponent& operator=(PlaceBombComponent&& other) = delete;
	};
}