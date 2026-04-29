#include "PlaceBombComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "RenderComponent.h"
#include "BombComponent.h"
#include "CollisionComponent.h"
#include "SDLSoundSystem.h"

void dae::PlaceBombComponent::PlaceBomb() 
{
	if (m_pGrid == nullptr || m_CanPlace == false) return;
	auto& scene = dae::SceneManager::GetInstance().GetScene(0);

	m_CanPlace = false;
	//Place Bomb
	auto bombGameObject = std::make_unique<dae::GameObject>(true, m_pGrid);
	auto bombRenderComponent = std::make_unique<dae::RenderComponent>(bombGameObject.get(), "Bomb.png");
	auto bombBombComponent = std::make_unique<dae::BombComponent>(bombGameObject.get(), 5.f, this);
	auto bombCollisionComponent = std::make_unique<dae::CollisionComponent>(bombGameObject.get(),32.f,32.f,'b');
	auto bombEx = bombBombComponent.get();
	bombGameObject.get()->AddComponent(std::move(bombRenderComponent));
	bombGameObject.get()->AddComponent(std::move(bombBombComponent));
	bombGameObject.get()->AddComponent(std::move(bombCollisionComponent));

	auto pos = GetOwner()->GetTransform()->GetWorldPosition();

	bombGameObject.get()->SetPosition(pos.x, pos.y);
	bombGameObject.get()->SetScale(1.5f, 1.5f);


	scene.Add(std::move(bombGameObject));
	bombEx->StartDetonate();


}