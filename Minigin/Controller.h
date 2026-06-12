#pragma once
#include <vector>
#include <memory>
#include "Commands.h"

#ifdef WIN32
#define GAMEPAD_DPAD_UP          0x0001
#define GAMEPAD_DPAD_DOWN        0x0002
#define GAMEPAD_DPAD_LEFT        0x0004
#define GAMEPAD_DPAD_RIGHT       0x0008
#define GAMEPAD_BACK             0x0020
#define GAMEPAD_LEFT_THUMB       0x0040
#define GAMEPAD_RIGHT_THUMB      0x0080
#define GAMEPAD_LEFT_SHOULDER    0x0100
#define GAMEPAD_RIGHT_SHOULDER   0x0200
#define GAMEPAD_A                0x1000
#define GAMEPAD_B                0x2000
#define GAMEPAD_X                0x4000
#define GAMEPAD_Y                0x8000

#else
#define GAMEPAD_DPAD_UP          11
#define GAMEPAD_DPAD_DOWN        12
#define GAMEPAD_DPAD_LEFT        13
#define GAMEPAD_DPAD_RIGHT       14
#define GAMEPAD_LEFT_THUMB       7
#define GAMEPAD_RIGHT_THUMB      8
#define GAMEPAD_LEFT_SHOULDER    9
#define GAMEPAD_RIGHT_SHOULDER   10
#define GAMEPAD_A                0
#define GAMEPAD_B                1
#define GAMEPAD_X                2
#define GAMEPAD_Y                3
#endif

namespace dae
{
	enum class KeyState {
		Up, Down, Pressed
	};

	struct ControllerBinding
	{
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


class Controller final
{
public:
	Controller(int controllerIndex);
	~Controller();
	void ProcessInput();
	void BindCommand(uint16_t button, KeyState state,std::unique_ptr<Command> command,std::unique_ptr<CommandValue> value);
	void BindAxis(std::unique_ptr<Command> command,std::unique_ptr<CommandValue> value,bool isLeft);
	void UnbindCommand(uint16_t key, KeyState state);
	void UnbindAxis(bool isLeft);
	void UnbindGameObject(GameObject* gameObject);
	static int GetConnectedControllerCount();

private:

	struct Impl;
	std::unique_ptr<Impl> m_Impl;

};

}