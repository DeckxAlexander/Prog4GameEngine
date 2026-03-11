#pragma once
#include "Singleton.h"
#include <vector>
#include <SDL3/SDL.h>
#include <cstdint>
#include "Controller.h"
#include "Commands.h"

//###################
//TODO
//###################
//- Create Controller Class and seperate Input
//- Add SDL_Controller for Emscripten
//- Allow for multiple controllers!


namespace dae
{


	struct KeyBinding 
	{
		SDL_Scancode key;
		KeyState state;
		std::unique_ptr<Command> command;
		std::unique_ptr<CommandValue> value;

	};



	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput();
		void BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value);
		void UnbindCommand(SDL_Scancode key, KeyState state);

		void AddController(std::unique_ptr<Controller> controller);
		const std::vector<std::unique_ptr<Controller>>& GetControllers() 
		{
			return m_Controllers;
		}

		

	private:



		std::vector<KeyBinding> m_KeyBindings{};
		std::vector<std::unique_ptr<Controller>> m_Controllers{};

	};
	

}
