#pragma once
#include "ObjectComponent.h"

namespace dae
{
	class GridComponent;
	class PlaceBombComponent final : public ObjectComponent
	{
	private:
			GridComponent* m_pGrid{ nullptr };
			bool m_CanPlace{ true };

	public:
		virtual void Update() override {}
		virtual void Render() const override {}


		void PlaceBomb();
		void UnlockCanPlace(bool value = true) { m_CanPlace = value; }



		PlaceBombComponent(GameObject* pOwner, GridComponent* pGrid) : ObjectComponent(pOwner), m_pGrid{pGrid}
		{}
		~PlaceBombComponent() = default;
		PlaceBombComponent(const PlaceBombComponent& other) = delete;
		PlaceBombComponent(PlaceBombComponent&& other) = delete;
		PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
		PlaceBombComponent& operator=(PlaceBombComponent&& other) = delete;
	};
}