#include "Controller.h"
#include <SDL3/SDL.h>
#ifdef WIN32
#include <Windows.h>
#include <Xinput.h>
#else

#endif


struct dae::Controller::Impl
{

#ifdef WIN32
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};
#else
	SDL_Gamepad* sdlController{ };
	uint32_t previousButtons{ 0 };
	uint32_t currentButtons{ 0 };
#endif
};

dae::Controller::Controller(int controllerIndex) : m_ControllerIndex(controllerIndex), m_Impl{ std::make_unique<Impl>() }
{

#ifndef WIN32
	m_Impl->sdlController = SDL_OpenGamepad(*SDL_GetGamepads(&controllerIndex));

#endif
	

	

}

dae::Controller::~Controller()
{
#ifndef WIN32
	if (m_Impl->sdlController != nullptr)
	{
		SDL_CloseGamepad(m_Impl->sdlController);
	}
#endif
}

void dae::Controller::ProcessInput()
{
#ifdef WIN32
	CopyMemory(&m_Impl->previousState, &m_Impl->currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_Impl->currentState, sizeof(XINPUT_STATE));
	DWORD dwResult = XInputGetState(m_ControllerIndex, &m_Impl->currentState);
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
		float normRX = rightX / 32767.0f;
		float normRY = rightY / 32767.0f;

		for (auto& binding : m_ControllerAxisBindings)
		{
			if (binding.isLeft)
			{
				if (leftX == 0 && leftY == 0) continue;

				binding.value.get()->Vec2D.x = normLX;
				binding.value.get()->Vec2D.y = -normLY;
				binding.command->Execute(binding.value.get());

			}
			else 
			{
				if (rightX == 0 && rightY == 0) continue;

				binding.value.get()->Vec2D.x = normRX;
				binding.value.get()->Vec2D.y = -normRY;
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
#else
	if (m_Impl->sdlController == nullptr) return;
	m_Impl->previousButtons = m_Impl->currentButtons;
	m_Impl->currentButtons = 0;

	for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
	{
		if (SDL_GetGamepadButton(m_Impl->sdlController, static_cast<SDL_GamepadButton>(i)))
		{
			m_Impl->currentButtons |= (1 << i);
		}
	}

	//Axis
	float leftX = static_cast<float>(SDL_GetGamepadAxis(m_Impl->sdlController, SDL_GAMEPAD_AXIS_LEFTX));
	float leftY = static_cast<float>(SDL_GetGamepadAxis(m_Impl->sdlController, SDL_GAMEPAD_AXIS_LEFTY));
	float rightX = static_cast<float>(SDL_GetGamepadAxis(m_Impl->sdlController, SDL_GAMEPAD_AXIS_RIGHTX));
	float rightY = static_cast<float>(SDL_GetGamepadAxis(m_Impl->sdlController, SDL_GAMEPAD_AXIS_RIGHTY));

	const float deadzone = 8000.f;

	if (std::abs(leftX) < deadzone) leftX = 0;
	if (std::abs(leftY) < deadzone) leftY = 0;
	if (std::abs(rightX) < deadzone) leftX = 0;
	if (std::abs(rightY) < deadzone) leftY = 0;

	float normLX = leftX / 32767.f;
	float normLY = leftY / 32767.f;

	float normRX = rightX / 32767.f;
	float normRY = rightY / 32767.f;

	for (auto& binding : m_ControllerAxisBindings)
	{
		if (binding.isLeft)
		{
			if (leftX == 0 && leftY == 0) continue;

			binding.value->Vec2D.x = normLX;
			binding.value->Vec2D.y = -normLY;

			binding.command->Execute(binding.value.get());
		}
		else
		{
			if (rightX == 0 && rightY == 0) continue;

			binding.value->Vec2D.x = normRX;
			binding.value->Vec2D.y = -normRY;

			binding.command->Execute(binding.value.get());
		}
	}

	uint32_t buttonChanges = m_Impl->currentButtons ^ m_Impl->previousButtons;
	uint32_t buttonsPressedThisFrame = buttonChanges & m_Impl->currentButtons;
	uint32_t buttonsReleasedThisFrame = buttonChanges & (~m_Impl->currentButtons);

	for (auto& binding : m_ControllerBindings)
	{
		uint32_t mask = (1 << binding.button);
		bool isPressed = (m_Impl->currentButtons & mask) != 0;

		switch (binding.state)
		{
		case KeyState::Down:
			if (buttonsPressedThisFrame & mask)
				binding.command->Execute(binding.value.get());
			break;

		case KeyState::Up:
			if (buttonsReleasedThisFrame & mask)
				binding.command->Execute(binding.value.get());
			break;

		case KeyState::Pressed:
			if (isPressed)
				binding.command->Execute(binding.value.get());
			break;
		}
	}
#endif
	
}

void dae::Controller::BindCommand(uint16_t button, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
{
	m_ControllerBindings.push_back({ button, state, std::move(command), std::move(value) });
}

void dae::Controller::BindAxis(std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value, bool isLeft)
{
	m_ControllerAxisBindings.push_back({ isLeft, std::move(command), std::move(value) });
}

void dae::Controller::UnbindCommand(uint16_t key, KeyState state)
{
	m_ControllerBindings.erase(
		std::remove_if(m_ControllerBindings.begin(), m_ControllerBindings.end(),
			[key, state](const ControllerBinding& b)
			{
				return b.button == key && b.state == state;
			}),
		m_ControllerBindings.end());


}
