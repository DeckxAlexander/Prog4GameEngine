#include "GameCommands.h"
#include "SoundSystem.h"
#include "GameManager.h"

void dae::ToggleMuteCommand::Execute(CommandValue*)
{
	m_Toggle = !m_Toggle;
	SoundSystemLocator::get_sound_system().Mute(m_Toggle);

}

void dae::NextLevelCommand::Execute(CommandValue* )
{
	GameManager::GetInstance().ProcessNextLevel();
}
