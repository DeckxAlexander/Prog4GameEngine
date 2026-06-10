#include "MenuComponent.h"
#include "TextComponent.h"
#include "SceneManager.h"
#include "GameSceneLoaderComponent.h"
#include "GameManager.h"
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
		GameManager::GetInstance().ResetGame();
		GameManager::GetInstance().SetPlayerAmount(1);
		GameSceneLoader::GetInstance().LoadLevelFromFile("Levels/Level1.txt",1);
	}
	if (m_Buttons[m_SelectedIndex].event == MenuEvent::Coop)
	{
		GameManager::GetInstance().ResetGame();
		GameManager::GetInstance().SetPlayerAmount(2);
		GameSceneLoader::GetInstance().LoadLevelFromFile("Levels/Level1.txt", 2);
	}

}

dae::MenuComponent::MenuComponent() : ObjectComponent(nullptr)
{}
