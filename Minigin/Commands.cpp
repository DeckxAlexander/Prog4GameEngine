#include "Commands.h"
#include "GameObject.h"
#include "MovementComponent.h"



void dae::MoveAround::Execute(CommandValue* value)
{
	auto comp = m_GameObject->GetComponentByType<MovementComponent>();
	if (value != nullptr)comp->AddVelocity(value->Vec2D.x, value->Vec2D.y);
}
