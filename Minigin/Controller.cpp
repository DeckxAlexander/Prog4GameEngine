#include "Controller.h"
#ifdef WIN32
#include <Windows.h>
#include <Xinput.h>
#else
#include <SDL3/SDL.h>
#endif


struct dae::Controller::Impl
{
	int m_ControllerIndex;
	std::vector<ControllerBinding> m_ControllerBindings{};
	std::vector<ControllerAxisBinding> m_ControllerAxisBindings{};
#ifdef WIN32
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};
#else
	SDL_Gamepad* sdlController{ };
	uint32_t previousButtons{ 0 };
	uint32_t currentButtons{ 0 };
#endif

	Impl(int controllerIndex) : m_ControllerIndex{ controllerIndex }
	{
#ifndef WIN32
		sdlController = SDL_OpenGamepad(*SDL_GetGamepads(&m_ControllerIndex));

#endif
	}



	static int GetConnectedControllerCount()
	{
#ifdef WIN32
		int count = 0;

		for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i) // XUSER_MAX_COUNT == 4
		{
			XINPUT_STATE state{};
			if (XInputGetState(i, &state) == ERROR_SUCCESS)
			{
				++count;
			}
		}

		return count;
#else

		int count = 0;

		SDL_JoystickID* gamepads = SDL_GetGamepads(&count);

		if (gamepads)
		{
			SDL_free(gamepads);
		}

		return count;
#endif
	}


	void ProcessInput()
	{
#ifdef WIN32
		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
		DWORD dwResult = XInputGetState(m_ControllerIndex, &currentState);
		if (dwResult == ERROR_SUCCESS) {

			float leftX = currentState.Gamepad.sThumbLX;
			float leftY = currentState.Gamepad.sThumbLY;
			float rightX = currentState.Gamepad.sThumbRX;
			float rightY = currentState.Gamepad.sThumbRY;
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
			WORD buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
			WORD buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
			WORD buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

			for (auto& binding : m_ControllerBindings)
			{
				bool isPressed = (currentState.Gamepad.wButtons & binding.button) != 0;

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
		if (sdlController == nullptr)
		{
			sdlController = SDL_OpenGamepad(*SDL_GetGamepads(&m_ControllerIndex));
			return;
		}
		previousButtons = currentButtons;
		currentButtons = 0;

		for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
		{
			if (SDL_GetGamepadButton(sdlController, static_cast<SDL_GamepadButton>(i)))
			{
				currentButtons |= (1 << i);
			}
		}

		//Axis
		float leftX = static_cast<float>(SDL_GetGamepadAxis(sdlController, SDL_GAMEPAD_AXIS_LEFTX));
		float leftY = static_cast<float>(SDL_GetGamepadAxis(sdlController, SDL_GAMEPAD_AXIS_LEFTY));
		float rightX = static_cast<float>(SDL_GetGamepadAxis(sdlController, SDL_GAMEPAD_AXIS_RIGHTX));
		float rightY = static_cast<float>(SDL_GetGamepadAxis(sdlController, SDL_GAMEPAD_AXIS_RIGHTY));

		const float deadzone = 8000.f;

		if (std::abs(leftX) < deadzone) leftX = 0;
		if (std::abs(leftY) < deadzone) leftY = 0;
		if (std::abs(rightX) < deadzone) rightX = 0;
		if (std::abs(rightY) < deadzone) rightY = 0;

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
				binding.value->Vec2D.y = normLY;

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

		uint32_t buttonChanges =currentButtons ^ previousButtons;
		uint32_t buttonsPressedThisFrame = buttonChanges &currentButtons;
		uint32_t buttonsReleasedThisFrame = buttonChanges & (~currentButtons);

		for (auto& binding : m_ControllerBindings)
		{
			uint32_t mask = (1 << binding.button);
			bool isPressed = (currentButtons & mask) != 0;

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

	void BindCommand(uint16_t button, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
	{
		m_ControllerBindings.push_back({ button, state, std::move(command), std::move(value) });
	}

	void BindAxis(std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value, bool isLeft)
	{
		m_ControllerAxisBindings.push_back({ isLeft, std::move(command), std::move(value) });
	}

	void UnbindCommand(uint16_t key, KeyState state)
	{
		m_ControllerBindings.erase(
			std::remove_if(m_ControllerBindings.begin(), m_ControllerBindings.end(),
				[key, state](const ControllerBinding& b)
				{
					return b.button == key && b.state == state;
				}),
			m_ControllerBindings.end());


	}
	void UnbindAxis(bool isLeft)
	{
		m_ControllerAxisBindings.erase(
			std::remove_if(m_ControllerAxisBindings.begin(), m_ControllerAxisBindings.end(),
				[isLeft](const ControllerAxisBinding& b)
				{
					return b.isLeft == isLeft;
				}),
			m_ControllerAxisBindings.end());
	}

	void UnbindGameObject(GameObject* gameObject)
	{

		m_ControllerBindings.erase(
			std::remove_if(m_ControllerBindings.begin(), m_ControllerBindings.end(),
				[gameObject](const ControllerBinding& b)
				{
					auto ptr = dynamic_cast<GameObjectCommand*>(b.command.get());
					if (ptr == nullptr) return false;

					return ptr->GetGameObject() != nullptr && ptr->GetGameObject() == gameObject;
				}),
			m_ControllerBindings.end());

		m_ControllerAxisBindings.erase(
			std::remove_if(m_ControllerAxisBindings.begin(), m_ControllerAxisBindings.end(),
				[gameObject](const ControllerAxisBinding& b)
				{
					auto ptr = dynamic_cast<GameObjectCommand*>(b.command.get());
					if (ptr == nullptr) return false;

					return ptr->GetGameObject() != nullptr && ptr->GetGameObject() == gameObject;
				}),
			m_ControllerAxisBindings.end());
	}

};

dae::Controller::Controller(int controllerIndex) :  m_Impl{ std::make_unique<Impl>(controllerIndex) }
{
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
	m_Impl->ProcessInput();
	
}

void dae::Controller::BindCommand(uint16_t button, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value)
{
	m_Impl->BindCommand(button, state, std::move(command), std::move(value));
}

void dae::Controller::BindAxis(std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value, bool isLeft)
{
	m_Impl->BindAxis(std::move(command), std::move(value), isLeft );
}

void dae::Controller::UnbindCommand(uint16_t key, KeyState state)
{
	m_Impl->UnbindCommand(key, state);


}

void dae::Controller::UnbindAxis(bool isLeft)
{
	m_Impl->UnbindAxis(isLeft);
}

void dae::Controller::UnbindGameObject(GameObject* gameObject)
{

	m_Impl->UnbindGameObject(gameObject);
}

int dae::Controller::GetConnectedControllerCount()
{
	return Impl::GetConnectedControllerCount();
}
