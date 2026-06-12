#include "PlaceBombComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "RenderComponent.h"
#include "BombComponent.h"
#include "CollisionComponent.h"
#include "GridComponent.h"
#include "GridTransform.h"
#include "SoundSystem.h"

void dae::PlaceBombComponent::PlaceBomb() 
{
	if (m_pGrid == nullptr || m_CanPlace == 0) return;
	int gridIndex = m_pGrid->GridToIndex(m_pGrid->WorldPosToTile(GetOwner()->GetWorldPosition()));
	if (m_pGrid->GetGridLayout()[gridIndex] == GridComponent::GridValue::bomb) return;
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();

	m_CanPlace--;
	bool willDetonate{ true };

	//Place Bomb
	auto bombGameObject = std::make_unique<dae::GameObject>(std::make_unique<GridTransform>(m_pGrid));
	auto bombRenderComponent = std::make_unique<dae::RenderComponent>( "Textures/Bomb.png");
	auto bombBombComponent = std::make_unique<dae::BombComponent>( 3.f, this, m_Size);
	auto bombCollisionComponent = std::make_unique<dae::CollisionComponent>(32.f,32.f,'b');
	auto bombEx = bombBombComponent.get();

	if (m_UsingDetonator)
	{
		if (m_CanPlace + 1 == m_CanPlaceMax)
		{
			m_OldestBomb = bombEx;
			willDetonate = false;
		}

	}

	bombGameObject.get()->AddComponent(std::move(bombRenderComponent));
	bombGameObject.get()->AddComponent(std::move(bombBombComponent));
	bombGameObject.get()->AddComponent(std::move(bombCollisionComponent));

	auto pos = GetOwner()->GetTransform()->GetWorldPosition();

	bombGameObject.get()->SetPosition(pos.x, pos.y);
	bombGameObject.get()->SetScale(1.5f, 1.5f);


	m_pGrid->GetGridLayout()[gridIndex] = GridComponent::GridValue::bomb;

	scene.Add(std::move(bombGameObject));


	if (willDetonate) bombEx->StartDetonate();

	SoundSystemLocator::get_sound_system().PlaySound(5);



}

void dae::PlaceBombComponent::ForceDetonate()
{
	if (m_OldestBomb == nullptr) return;
	m_OldestBomb->Explode();
	m_OldestBomb = nullptr;
}
