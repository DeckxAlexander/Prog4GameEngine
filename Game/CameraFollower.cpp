#include "CameraFollower.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include "PlayerComponent.h"
#include "Renderer.h"

void dae::CameraFollowerComponent::InitializePlayer()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	auto players = scene.GetAllObjectsByComponent<PlayerComponent>();
	if (players.empty()) return;
	m_Player = players.front();
	m_Player->GetComponentByType<PlayerComponent>()->GetSubject()->AddObserver(this);
}


void dae::CameraFollowerComponent::Start()
{
	InitializePlayer();
}

void dae::CameraFollowerComponent::Update()
{
	if (m_Player == nullptr) return;
	auto& camera = dae::Renderer::GetInstance().GetCamera();
	const float marginX = 128.f;
	const float marginY = 128.f;
	const auto pos = m_Player->GetWorldPosition();

	float visibleWidth = 1280.f / camera.zoom;
	float visibleHeight = 576.f / camera.zoom;

	if (pos.x < camera.x + marginX)
		camera.x = std::max(pos.x - marginX, 0.f);

	if (pos.x > camera.x + visibleWidth - (marginX*2.f))
		camera.x = std::min(pos.x, m_MapX - marginX) - (visibleWidth - (marginX * 2.f));

	if (pos.y < camera.y + marginY)
		camera.y = std::max(pos.y - marginY, 0.f);

	if (pos.y > camera.y + visibleHeight - marginY)
		camera.y = std::min(pos.y, m_MapY - marginY) - (visibleHeight - marginY);
}


dae::CameraFollowerComponent::CameraFollowerComponent(float mapWidth, float mapHeight) : ObjectComponent(nullptr), m_MapX{ mapWidth}, m_MapY{ mapHeight }
{

}

dae::CameraFollowerComponent::~CameraFollowerComponent()
{
	
	if (m_Player != nullptr) 
	{
		auto playerComp = m_Player->GetComponentByType<PlayerComponent>();
		if (playerComp != nullptr) playerComp->GetSubject()->RemoveObserver(this);
	}
	
}

void dae::CameraFollowerComponent::OnNotify(const Event& event)
{
	if (event.event == EventType::PlayerDead)m_Player = nullptr;
}

void dae::CameraFollowerComponent::OnSubjectDestroyed(Subject * subject)
{
	subject->RemoveObserver(this);
}
