#include "Commands.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "ScoreManager.h"
#include "HealthComponent.h"



void dae::MoveAround::Execute([[maybe_unused]] CommandValue* value)
{
	auto comp = m_GameObject->GetComponentByType<MovementComponent>();
	if (value != nullptr)comp->AddVelocity(value->Vec2D.x, value->Vec2D.y);
}

void dae::Damage::Execute([[maybe_unused]] CommandValue* value)
{
	if (auto comp = m_GameObject->GetComponentByType<HealthComponent>()) comp->TakeDamage(1);
}

void dae::AddScore::Execute([[maybe_unused]] CommandValue* value)
{
	if (auto comp = m_GameObject->GetComponentByType<ScoreComponent>()) comp->AddScore(1);
}
