#include "Commands.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "PlaceBombComponent.h"
namespace dae
{
	class TestCommand final : public Command
	{
	public:
		void Execute([[maybe_unused]] CommandValue* value) override
		{
			if (value == nullptr) std::cout << "Test";
		}
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
}