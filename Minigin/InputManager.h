#pragma once
#include "Singleton.h"
#include <vector>
#include <SDL3/SDL.h>
#include <cstdint>
#include "Controller.h"
#include "Commands.h"



namespace dae
{


	struct KeyBinding 
	{
		SDL_Scancode key;
		KeyState state;
		std::unique_ptr<Command> command;
		std::unique_ptr<CommandValue> value;

	};

	struct MouseBinding
	{
		uint8_t button;
		KeyState state;
		std::unique_ptr<Command> command;
		std::unique_ptr<CommandValue> value;

	};


	class InputManager final : public Singleton<InputManager>
	{
	public:

		bool ProcessInput();
		void BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value);
		void BindMouseCommand(uint8_t button, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value);
		void UnbindCommand(SDL_Scancode key, KeyState state);
		void UnbindMouseCommand(uint8_t button, KeyState state);

		void UnbindGameObject(GameObject* go);
		void UnbindAll();

		void AddController(std::unique_ptr<Controller> controller);
		const std::vector<std::unique_ptr<Controller>>& GetControllers() 
		{
			return m_Controllers;
		}

		

	private:



		std::vector<KeyBinding> m_KeyBindings{};
		std::vector<MouseBinding> m_MouseBindings{};
		std::vector<std::unique_ptr<Controller>> m_Controllers{};

	};
	

}
