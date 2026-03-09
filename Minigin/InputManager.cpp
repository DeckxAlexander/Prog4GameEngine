#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"

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
					binding.command->Execute();
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
					binding.command->Execute();
				}
			}
		}
		//if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) 
		//{
		//	
		//}
		// etc...
		ImGui_ImplSDL3_ProcessEvent(&e);

	}

	//Keys Held
	const bool* keyboardState = SDL_GetKeyboardState(nullptr);

	for (auto& binding : m_KeyBindings)
	{
		if (binding.state == KeyState::Pressed && keyboardState[binding.key])
		{

			binding.command->Execute();
			
		}
	}



	return true;
}

void dae::InputManager::BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command)
{
	m_KeyBindings.push_back({ key, state, std::move(command) });
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
