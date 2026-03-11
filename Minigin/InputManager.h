#pragma once
#include "Singleton.h"
#include <vector>
#include <SDL3/SDL.h>
#include <cstdint>
#include "Commands.h"

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
		std::unique_ptr<CommandValue> value;

	};

	struct ControllerBinding
	{
		//WORD button;
		uint16_t button;
		KeyState state;
		std::unique_ptr<Command> command;
		std::unique_ptr<CommandValue> value;
	};

	struct ControllerAxisBinding
	{
		bool isLeft;
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
		//void BindCommand(WORD key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value);
		void BindCommand(uint16_t key, KeyState state, std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value);
		void BindAxis(std::unique_ptr<Command> command, std::unique_ptr<CommandValue> value, bool isLeft);
		void UnbindCommand(SDL_Scancode key, KeyState state);
		//void UnbindCommand(WORD key, KeyState state);
		void UnbindCommand(uint16_t key, KeyState state);

	private:


		struct Impl;             
		std::unique_ptr<Impl> m_Impl;

		//XINPUT_STATE m_PreviousControllerState;
		std::vector<KeyBinding> m_KeyBindings{};
		std::vector<ControllerBinding> m_ControllerBindings{};
		std::vector<ControllerAxisBinding> m_ControllerAxisBindings{};

		//XINPUT_STATE m_PreviousState{};
		//XINPUT_STATE m_CurrentState{};
	};
	

}
