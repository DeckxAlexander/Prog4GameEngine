#include "GridComponent.h"
#include "Renderer.h"
#include "GameObject.h"


dae::GridComponent::GridComponent(GameObject* pOwner, int colums, int rows) : ObjectComponent(pOwner), m_Colums{colums}, m_Rows{rows}
{
    m_GridLayout.resize(m_Colums * m_Rows);
}

void dae::GridComponent::SpawnGrid() 
{

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