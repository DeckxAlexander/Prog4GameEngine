#include "MenuComponent.h"
#include "TextComponent.h"
#include "SceneManager.h"
#include "GameSceneLoaderComponent.h"
#include "GameManager.h"
#include "SoundSystem.h"
#include <iostream>
#include <stdexcept>



void dae::MenuComponent::AddButton(TextComponent* text, MenuEvent event)
{
	if (text == nullptr) throw std::runtime_error("text was null");
	m_Buttons.push_back({ text, event });
}


void dae::MenuComponent::RemoveButton(size_t index)
{
	m_Buttons.erase(m_Buttons.begin() + index);
}

void dae::MenuComponent::MoveSelected(size_t index)
{
	m_Buttons[m_SelectedIndex].text->SetColor(m_NotSelectedColor);

	m_SelectedIndex += index;
	m_SelectedIndex %= m_Buttons.size();
	if (m_SelectedIndex < 0) m_SelectedIndex = m_Buttons.size() - 1;
	m_Buttons[m_SelectedIndex].text->SetColor(m_SelectedColor);
}

void dae::MenuComponent::ExecuteSelected()
{
	std::cout << "Execute";


	if (m_Buttons[m_SelectedIndex].event == MenuEvent::Solo) 
	{
		SoundSystemLocator::get_sound_system().StopAllSounds();
		GameManager::GetInstance().ResetGame();
		GameManager::GetInstance().SetPlayerAmount(1);
		GameManager::GetInstance().SetVersus(false);
		GameSceneLoader::GetInstance().LoadLevelFromFile("Levels/Level1.txt",1);
		SoundSystemLocator::get_sound_system().PlaySound(1);
	}
	if (m_Buttons[m_SelectedIndex].event == MenuEvent::Coop)
	{
		SoundSystemLocator::get_sound_system().StopAllSounds();
		GameManager::GetInstance().ResetGame();
		GameManager::GetInstance().SetPlayerAmount(2);
		GameManager::GetInstance().SetVersus(false);
		GameSceneLoader::GetInstance().LoadLevelFromFile("Levels/Level1.txt", 2);
		SoundSystemLocator::get_sound_system().PlaySound(1);
	}

	if (m_Buttons[m_SelectedIndex].event == MenuEvent::Versus)
	{
		SoundSystemLocator::get_sound_system().StopAllSounds();
		GameManager::GetInstance().ResetGame();
		GameManager::GetInstance().SetPlayerAmount(1);
		GameManager::GetInstance().SetVersus(true);
		GameSceneLoader::GetInstance().LoadLevelFromFile("Levels/Level1.txt", 1, true);
		SoundSystemLocator::get_sound_system().PlaySound(1);
	}

}

dae::MenuComponent::MenuComponent() : ObjectComponent(nullptr)
{}
