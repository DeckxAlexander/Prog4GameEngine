#include "Commands.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "PlaceBombComponent.h"
namespace dae
{
	class ToggleMuteCommand final : public Command
	{
	public:
		void Execute(CommandValue* value) override;
	private:
		bool m_Toggle{ false };
	};

	class MoveAround final : public GameObjectCommand
	{
	public:
		MoveAround(GameObject* gameobject) : GameObjectCommand(gameobject)
		{
		}

		void Execute([[maybe_unused]] CommandValue* value) 
		{
			auto comp = m_GameObject->GetComponentByType<MovementComponent>();
			if (value != nullptr)comp->AddVelocity(value->Vec2D.x, value->Vec2D.y);
		}
	};


	class PlaceBomb final : public GameObjectCommand
	{
	public:
		PlaceBomb(GameObject* gameobject) : GameObjectCommand(gameobject)
		{
		}

		void Execute([[maybe_unused]] CommandValue* value)
		{
			if (auto comp = m_GameObject->GetComponentByType<PlaceBombComponent>()) comp->PlaceBomb();
		}
	};

	class DetonateBomb final : public GameObjectCommand
	{
	public:
		DetonateBomb(GameObject* gameobject) : GameObjectCommand(gameobject)
		{
		}

		void Execute([[maybe_unused]] CommandValue* value)
		{
			if (auto comp = m_GameObject->GetComponentByType<PlaceBombComponent>()) comp->ForceDetonate();
		}
	};
}