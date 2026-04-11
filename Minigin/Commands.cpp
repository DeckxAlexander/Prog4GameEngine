#include "Commands.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "PlaceBombComponent.h"



void dae::MoveAround::Execute([[maybe_unused]] CommandValue* value)
{
	auto comp = m_GameObject->GetComponentByType<MovementComponent>();
	if (value != nullptr)comp->AddVelocity(value->Vec2D.x, value->Vec2D.y);
}

void dae::PlaceBomb::Execute([[maybe_unused]] CommandValue* value)
{
	if (auto comp = m_GameObject->GetComponentByType<PlaceBombComponent>()) comp->PlaceBomb();
}

