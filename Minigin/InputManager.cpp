#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include <iostream>



bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}
		if (e.type == SDL_EVENT_KEY_DOWN) 
		{
			SDL_Scancode key = e.key.scancode;

			for (auto& binding : m_KeyBindings) 
			{
				if (binding.state == KeyState::Down && binding.key == key) 
				{
					binding.command->Execute(binding.value.get());
				}
			}
		}
		if (e.type == SDL_EVENT_KEY_UP)
		{
			SDL_Scancode key = e.key.scancode;

			for (auto& binding : m_KeyBindings)
			{
				if (binding.state == KeyState::Up && binding.key == key)
				{
					binding.command->Execute(binding.value.get());
				}
			}
		}
		ImGui_ImplSDL3_ProcessEvent(&e);


	}

	//Keys Held
	const bool* keyboardState = SDL_GetKeyboardState(nullptr);

	for (auto& binding : m_KeyBindings)
	{
		if (binding.state == KeyState::Pressed && keyboardState[binding.key])
		{

			binding.command->Execute(binding.value.get());
			
		}
	}
	for (auto& controller : m_Controllers)
	{
		controller->ProcessInput();
	}

	return true;
}

void dae::InputManager::BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
{
	m_KeyBindings.push_back({ key, state, std::move(command), std::move(value) });
}


void dae::InputManager::UnbindCommand(SDL_Scancode key, KeyState state)
{
	m_KeyBindings.erase(
		std::remove_if(m_KeyBindings.begin(), m_KeyBindings.end(),
			[key, state](const KeyBinding& b)
			{
				return b.key == key && b.state == state;
			}),
		m_KeyBindings.end());
}

void dae::InputManager::AddController(std::unique_ptr<Controller> controller)
{
	m_Controllers.push_back(std::move(controller));
}




