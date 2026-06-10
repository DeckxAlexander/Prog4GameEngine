#include "InputManager.h"
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

		if (e.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			auto button = e.button.button;

			for (auto& binding : m_MouseBindings)
			{
				if (binding.state == KeyState::Up && binding.button == button)
				{
					binding.command->Execute(binding.value.get());
				}
			}

		}

		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			auto button = e.button.button;

			for (auto& binding : m_MouseBindings)
			{
				if (binding.state == KeyState::Down && binding.button == button)
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

	//Mouse 
	float mouseX, mouseY;
	uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY); 

	for (auto& binding : m_MouseBindings)
	{
		if (binding.state == KeyState::Pressed && (mouseState & binding.button))
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

void dae::InputManager::BindMouseCommand(uint8_t button, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
{
	m_MouseBindings.push_back({ button, state, std::move(command), std::move(value) });
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

void dae::InputManager::UnbindMouseCommand(uint8_t button, KeyState state)
{
	m_MouseBindings.erase(
		std::remove_if(m_MouseBindings.begin(), m_MouseBindings.end(),
			[button, state](const MouseBinding& b)
			{
				return b.button == button && b.state == state;
			}),
		m_MouseBindings.end());
}

void dae::InputManager::UnbindGameObject(GameObject* go)
{
	if (m_KeyBindings.empty()) return;
	m_KeyBindings.erase(
		std::remove_if(m_KeyBindings.begin(), m_KeyBindings.end(),
			[go](const KeyBinding& b)
			{
				auto ptr = dynamic_cast<GameObjectCommand*>(b.command.get());
				if (ptr == nullptr) return false;

				return ptr->GetGameObject() != nullptr && ptr->GetGameObject() == go;
			}),
		m_KeyBindings.end());

	m_MouseBindings.erase(
		std::remove_if(m_MouseBindings.begin(), m_MouseBindings.end(),
			[go](const MouseBinding& b)
			{
				auto ptr = dynamic_cast<GameObjectCommand*>(b.command.get());
				if (ptr == nullptr) return false;

				return ptr->GetGameObject() != nullptr && ptr->GetGameObject() == go;
			}),
		m_MouseBindings.end());
	for (auto& controller : m_Controllers)
	{
		controller->UnbindGameObject(go);
	}
}

void dae::InputManager::UnbindAll()
{
	m_KeyBindings.clear();
	m_MouseBindings.clear();
	m_Controllers.clear();
}

void dae::InputManager::AddController(std::unique_ptr<Controller> controller)
{
	m_Controllers.push_back(std::move(controller));
}




