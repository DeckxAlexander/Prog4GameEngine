#pragma once
#include <iostream>
#include <glm/glm.hpp>

namespace dae
{
	struct CommandValue
	{
		glm::vec2 Vec2D;
	};


	class GameObject;

	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(CommandValue* value) = 0;

	};


	class GameObjectCommand : public Command
	{
	protected:
		GameObject* m_GameObject;
	public:
		GameObjectCommand(GameObject* gameobject) : m_GameObject{ gameobject }
		{
			
		}
		virtual ~GameObjectCommand() = default;
		virtual void Execute(CommandValue* value) = 0;
		GameObject* GetGameObject() const { return m_GameObject; }
	};


	

	
}