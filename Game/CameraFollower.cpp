#include "CameraFollower.h"
#include "SceneManager.h"
#include "CollisionComponent.h"
#include "PlayerComponent.h"
#include "Renderer.h"

void dae::CameraFollowerComponent::InitializePlayer()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	auto players = scene.GetAllObjectsByComponent<PlayerComponent>();
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

	if (pos.x < camera.x + marginX && camera.x > 0)
		camera.x = pos.x - marginX;

	if (pos.x > camera.x + visibleWidth - (marginX*2.f) && camera.x < 480)
		camera.x = pos.x - (visibleWidth - (marginX * 2.f));

	if (pos.y < camera.y + marginY && camera.y > 0)
		camera.y = pos.y - marginY;

	if (pos.y > camera.y + visibleHeight - marginY && camera.y < 100)
		camera.y = pos.y - (visibleHeight - marginY);
}


dae::CameraFollowerComponent::CameraFollowerComponent(GameObject* pOwner) : ObjectComponent(pOwner)
{
}

dae::CameraFollowerComponent::~CameraFollowerComponent()
{
	
	
		auto playerComp = m_Player->GetComponentByType<PlayerComponent>();
		if (playerComp != nullptr) playerComp->GetSubject()->RemoveObserver(this);
	
}

void dae::CameraFollowerComponent::OnNotify(const Event& event)
{
	if (event.event == EventType::PlayerDead)m_Player = nullptr;
}

void dae::CameraFollowerComponent::OnSubjectDestroyed(Subject * subject)
{
	subject->RemoveObserver(this);
}
