#include "GridComponent.h"
#include "GridComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "GridTransform.h"
#include "RenderComponent.h"
#include "CollisionComponent.h"
#include "SceneManager.h"
#include <iostream>

dae::GridComponent* dae::GridLocator::m_GridInstance = nullptr;


glm::ivec2 dae::GridComponent::WorldPosToTile(const glm::vec3& worldPos) const
{

	return glm::ivec2(
		static_cast<int>(std::floor(worldPos.x / m_TileScale.x)),
		static_cast<int>(std::floor(worldPos.y / m_TileScale.y))
	);
}

dae::GridComponent::GridComponent(GameObject* pOwner, int colums, int rows) : ObjectComponent(pOwner), m_Colums{colums}, m_Rows{rows}
{
    m_GridLayout.resize(m_Colums * m_Rows);
    m_GridPointers.resize(m_Colums * m_Rows);
}


void dae::GridComponent::SetupGrid() 
{
	//Create Hard blocks

	m_GridLayout[GridToIndex(1, 1)] = GridValue::spawn;
	m_GridLayout[GridToIndex(1, 2)] = GridValue::spawn;
	m_GridLayout[GridToIndex(1, 3)] = GridValue::spawn;
	m_GridLayout[GridToIndex(2, 1)] = GridValue::spawn;
	m_GridLayout[GridToIndex(3, 1)] = GridValue::spawn;


	for (int indexX{}; indexX < m_Colums; indexX++)
	{
		for (int indexY{}; indexY < m_Rows; indexY++)
		{
			if ((indexX == 0 || indexX == m_Colums-1 || indexY == 0 || indexY == m_Rows-1) || (indexX % 2 == 0 && indexY % 2 == 0))
			{
				m_GridLayout[GridToIndex(indexX, indexY)] = GridValue::hard;
			}

		}

	}

	int softblocksSpawned{ 0 };

	while (softblocksSpawned < m_SoftBlocksAmount)
	{
		int index = rand() % (m_Colums * m_Rows);
		if (m_GridLayout[index] == GridValue::empty) 
		{
			m_GridLayout[index] = GridValue::soft;
			softblocksSpawned++;
		}
	}


	//Create softblocks

}

void dae::GridComponent::SpawnGrid() 
{
	auto& scene = dae::SceneManager::GetInstance().GetScene(0);


	for (int i{}; i < int(m_GridLayout.size()); i++)
	{
		auto val = m_GridLayout[i];
		if (val == GridValue::hard) 
		{
			auto tileGameObject = std::make_unique<dae::GameObject>(std::make_unique<GridTransform>( this));
			auto tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "HardStoneTile.png");
			auto tilecoll = std::make_unique<dae::CollisionComponent>(tileGameObject.get(), 32.f, 32.f, 'w');
			tilecoll.get()->AddBlockingTag('e');

			tileGameObject.get()->AddComponent(std::move(tileRenderComponent));
			tileGameObject.get()->AddComponent(std::move(tilecoll));
			tileGameObject->SetScale(2.f, 2.f);

			int x = i % m_Colums;
			int y = i / m_Colums;
			dynamic_cast<dae::GridTransform*>(tileGameObject->GetTransform())->SetGridTile(x, y);
			m_GridPointers[i] = (tileGameObject.get());

			scene.Add(std::move(tileGameObject));
		}

		if (val == GridValue::soft)
		{
			auto tileGameObject = std::make_unique<dae::GameObject>(std::make_unique<GridTransform>(this));
			auto tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "SoftStoneTile.png");
			auto tilecoll = std::make_unique<dae::CollisionComponent>(tileGameObject.get(), 32.f, 32.f, 'w');
			tilecoll.get()->AddBlockingTag('e');
			tileGameObject.get()->AddComponent(std::move(tileRenderComponent));
			tileGameObject.get()->AddComponent(std::move(tilecoll));
			tileGameObject->SetScale(2.f, 2.f);

			int x = i % m_Colums;
			int y = i / m_Colums;
			dynamic_cast<dae::GridTransform*>(tileGameObject->GetTransform())->SetGridTile(x, y);
			m_GridPointers[i] = (tileGameObject.get());

			scene.Add(std::move(tileGameObject));
		}
	}
}


void dae::GridComponent::Render() const
{
	if (!m_ShowDebug) return;

    const auto& pos = m_pOwner->GetWorldPosition();
    float width =  m_TileScale.x;
    float height = m_TileScale.y;

    for (int x = 0; x <= m_Colums; x++) {
        float xpos = pos.x + x * width;
        Renderer::GetInstance().RenderLine(xpos, pos.y, xpos, pos.y + height* m_Rows);
    }

    for (int y = 0; y <= m_Rows; y++) {
        float ypos = pos.y + y * height;
        Renderer::GetInstance().RenderLine(pos.x, ypos, pos.x +width * m_Colums, ypos);
    }

}

void dae::GridComponent::Update() 
{

}

void dae::GridComponent::SetTileScale(float x, float y) 
{
    m_TileScale.x = x;
    m_TileScale.y = y;
}