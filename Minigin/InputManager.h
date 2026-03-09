#pragma once
#include "Singleton.h"
#include "Commands.h"
#include <vector>
#include <SDL3/SDL.h>

namespace dae
{
	enum class KeyState {
		Up, Down, Pressed
	};

	struct KeyBinding 
	{
		SDL_Scancode key;
		KeyState state;
		std::unique_ptr<Command> command;

	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		void BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command);
		void UnbindCommand(SDL_Scancode key, KeyState state);

	private:

		std::vector<KeyBinding> m_KeyBindings{};
	};
	

}
