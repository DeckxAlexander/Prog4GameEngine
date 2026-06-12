#include "LivesDisplayComponent.h"
#include "SceneManager.h"
#include "PlayerComponent.h"
#include "TextComponent.h"
#include <string>

void dae::LivesDisplayComponent::Start()
{
	InitializePlayers();
	UpdateDisplay();
}

void dae::LivesDisplayComponent::InitializePlayers()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	m_Players.clear();
	m_Players = scene.GetAllObjectsByComponent<PlayerComponent>();
	for (auto player : m_Players)
	{
		player->GetComponentByType<PlayerComponent>()->GetSubject()->AddObserver(this);
	}

}

void dae::LivesDisplayComponent::UpdateDisplay()
{
	std::string text = "";

	for (auto player : m_Players)
	{
		auto playerComp = player->GetComponentByType<PlayerComponent>();
		text += "P" + std::to_string(playerComp->GetIndex()+1);
		text += ": " + std::to_string(playerComp->GetLives());
		text += " ";
	}

	GetOwner()->GetComponentByType<TextComponent>()->SetText(text);
}

dae::LivesDisplayComponent::LivesDisplayComponent() : ObjectComponent()
{}

dae::LivesDisplayComponent::~LivesDisplayComponent()
{

	for (auto player : m_Players)
	{
		auto playerComp = player->GetComponentByType<PlayerComponent>();
		if (playerComp) playerComp->GetSubject()->RemoveObserver(this);
	}
}

void dae::LivesDisplayComponent::OnNotify(const Event& event)
{
	if (event.event == EventType::LivesChanged)
	{
		UpdateDisplay();
	}
	if (event.event == EventType::PlayerDead)
	{
		m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), event.sender->GetOwner()), m_Players.end());
	}

}
