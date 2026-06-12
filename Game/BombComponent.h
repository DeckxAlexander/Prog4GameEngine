#pragma once
#include "ObjectComponent.h"

namespace dae
{
	class PlaceBombComponent;
	class GridTransform;
	class GridComponent;
	class BombComponent final : public ObjectComponent
	{
	private:
		float m_TimePassed{};
		float m_DetonationTime;
		int m_Size;
		

		bool m_IsDetonating{ false };
		bool m_IsExploding{ false };
		PlaceBombComponent* m_Placer;

		glm::ivec4 BreakSoftBlocks(GridComponent* grid, glm::ivec2 gpos);
		void KillSurrounding(GridTransform* gridTransform, glm::ivec4 dirSizes);
		void SpawnParticles(glm::ivec4 positions, glm::vec2 size);

	public:
		virtual void Update() override;
		virtual void Render() const override;
		void StartDetonate(bool value = true) {
			m_IsDetonating = value;
		}


		


		void Explode();

		BombComponent(float detonationTime, PlaceBombComponent* placer, int size = 1) : ObjectComponent(), m_DetonationTime{detonationTime}, m_Placer{ placer }, m_Size{size}
		{
		}
		~BombComponent() {}
		BombComponent(const BombComponent& other) = delete;
		BombComponent(BombComponent&& other) = delete;
		BombComponent& operator=(const BombComponent& other) = delete;
		BombComponent& operator=(BombComponent&& other) = delete;
	};
}