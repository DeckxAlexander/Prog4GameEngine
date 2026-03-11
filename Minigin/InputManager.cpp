#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#endif



struct dae::InputManager::Impl
{
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};
};


dae::InputManager::InputManager() : m_Impl(std::make_unique<Impl>())
{
}

dae::InputManager::~InputManager()
{
}

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

	//Controller Support

	CopyMemory(&m_Impl->previousState, &m_Impl->currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_Impl->currentState, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(0, &m_Impl->currentState);
	if (dwResult == ERROR_SUCCESS) {

		float leftX = m_Impl->currentState.Gamepad.sThumbLX;
		float leftY = m_Impl->currentState.Gamepad.sThumbLY;
		float rightX = m_Impl->currentState.Gamepad.sThumbRX;
		float rightY = m_Impl->currentState.Gamepad.sThumbRY;
		if (abs(leftX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) leftX = 0;
		if (abs(leftY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) leftY = 0;

		if (abs(rightX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) rightX = 0;
		if (abs(rightY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) rightY = 0;

		float normLX = leftX / 32767.0f;
		float normLY = leftY / 32767.0f;
		//float normRX = rightX / 32767.0f;
		//float normRY = rightY / 32767.0f;

		for (auto& binding : m_ControllerAxisBindings)
		{
			if (binding.isLeft) 
			{
				if (leftX == 0 && leftY == 0) continue;

				binding.value.get()->Vec2D.x = normLX;
				binding.value.get()->Vec2D.y = -normLY;


				binding.command->Execute(binding.value.get());

			}
		}


		WORD buttonChanges = m_Impl->currentState.Gamepad.wButtons ^ m_Impl->previousState.Gamepad.wButtons;
		WORD buttonsPressedThisFrame = buttonChanges & m_Impl->currentState.Gamepad.wButtons;
		WORD buttonsReleasedThisFrame = buttonChanges & (~m_Impl->currentState.Gamepad.wButtons);

		for (auto& binding : m_ControllerBindings)
		{
			bool isPressed = (m_Impl->currentState.Gamepad.wButtons & binding.button) != 0;

			switch (binding.state)
			{
			case KeyState::Down:
				if ((buttonsPressedThisFrame & binding.button) != 0)
					binding.command->Execute(binding.value.get());
				break;

			case KeyState::Up:
				if ((buttonsReleasedThisFrame & binding.button) != 0)
					binding.command->Execute(binding.value.get());
				break;

			case KeyState::Pressed:
				if (isPressed)
					binding.command->Execute(binding.value.get());
				break;
			}
		}

	}

	return true;
}

void dae::InputManager::BindCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
{
	m_KeyBindings.push_back({ key, state, std::move(command), std::move(value) });
}

void dae::InputManager::BindCommand(uint16_t key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
{
	m_ControllerBindings.push_back({ key, state, std::move(command), std::move(value) });
}

void dae::InputManager::BindAxis(std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value, bool isLeft)
{
	m_ControllerAxisBindings.push_back({ isLeft, std::move(command), std::move(value) });
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

void dae::InputManager::UnbindCommand(uint16_t key, KeyState state)
{
	m_ControllerBindings.erase(
		std::remove_if(m_ControllerBindings.begin(), m_ControllerBindings.end(),
			[key, state](const ControllerBinding& b)
			{
				return b.button == key && b.state == state;
			}),
		m_ControllerBindings.end());
}
