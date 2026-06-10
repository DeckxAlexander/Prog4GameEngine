#include "ScoreDisplayComponent.h"
#include "GameManager.h"
#include "GameObject.h"
#include "TextComponent.h"


void dae::ScoreDisplayComponent::Start()
{
	GameManager::GetInstance().GetSubject()->AddObserver(this);
	int score = GameManager::GetInstance().GetScore();


	GetOwner()->GetComponentByType<TextComponent>()->SetText(std::to_string(score));

}

dae::ScoreDisplayComponent::ScoreDisplayComponent() : ObjectComponent(nullptr)
{

}

dae::ScoreDisplayComponent::~ScoreDisplayComponent()
{
	if (!m_SubjectDestroyed) GameManager::GetInstance().GetSubject()->RemoveObserver(this);
}

void dae::ScoreDisplayComponent::OnNotify(const Event& event)
{
	if (event.event == EventType::ScoreChanged) 
	{
		int score = GameManager::GetInstance().GetScore();
		GetOwner()->GetComponentByType<TextComponent>()->SetText(std::to_string(score));
	}
}

void dae::ScoreDisplayComponent::OnSubjectDestroyed(Subject * )
{
	m_SubjectDestroyed = true;
}
