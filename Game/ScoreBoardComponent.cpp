#include "ScoreBoardComponent.h"
#include "TextComponent.h"
#include "SceneManager.h"
#include "GameSceneLoaderComponent.h"
#include "GameManager.h"
#include "SoundSystem.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

void dae::ScoreBoardComponent::AddButton(TextComponent* text, bool confirm)
{
	if (text == nullptr) throw std::runtime_error("text was null");
	m_Buttons.push_back({ text, confirm, 'A'});
	if (confirm) return;
	text->SetText("A");
}

void dae::ScoreBoardComponent::RemoveButton(size_t index)
{
	m_Buttons.erase(m_Buttons.begin() + index);
}

void dae::ScoreBoardComponent::MoveSelected(size_t index)
{
	m_Buttons[m_SelectedIndex].text->SetColor(m_NotSelectedColor);
	m_SelectedIndex += index;
	m_SelectedIndex %= m_Buttons.size();
	if (m_SelectedIndex < 0) m_SelectedIndex = m_Buttons.size() - 1;
	m_Buttons[m_SelectedIndex].text->SetColor(m_SelectedColor);
}

void dae::ScoreBoardComponent::LetterChangeSelected(size_t index)
{ 
	if (m_Buttons[m_SelectedIndex].isConfirm) return;
	constexpr int alphabetSize = 'Z' - 'A' + 1; // 24
	int current = m_Buttons[m_SelectedIndex].letter - 'A';
	current = (current + static_cast<int>(index)) % alphabetSize;
	if (current < 0) current = alphabetSize - 1;

	m_Buttons[m_SelectedIndex].letter = static_cast<char>('A' + current);
	m_Buttons[m_SelectedIndex].text->SetText(std::string(1, m_Buttons[m_SelectedIndex].letter));
}

void dae::ScoreBoardComponent::ExecuteSelected()
{
	if (m_Buttons[m_SelectedIndex].isConfirm )
	{
		auto& oldScene = SceneManager::GetInstance().GetActiveScene();
		GameSceneLoader::GetInstance().OpenMainMenu();
		SceneManager::GetInstance().RemoveScene(oldScene);
		std::string name = "";
		for (auto button: m_Buttons) 
		{
			if (button.isConfirm) continue;
			name += std::string(1,button.letter);
		}
		AddScore("scores.csv", name, GameManager::GetInstance().GetScore());
	}
	else 
	{
		MoveSelected(1);
	}
}

void dae::ScoreBoardComponent::AddScore(const std::string& filename, const std::string& playerName, int newScore)
{
	struct PlayerScore {
		std::string name;
		int score;
	};
	const auto fullPath = m_dataPath / filename;
	std::vector<PlayerScore> scores{};
	std::ifstream inFile(fullPath);

	if (inFile)
	{
		std::string line;

		while (std::getline(inFile, line))
		{
			std::stringstream ss(line);

			std::string name;
			std::string scoreStr;

			if (std::getline(ss, name, ',') &&
				std::getline(ss, scoreStr))
			{
				scores.push_back({ name, std::stoi(scoreStr) });
			}
		}
		inFile.close();
	}

	
	scores.push_back({ playerName, newScore });

	std::sort(scores.begin(), scores.end(),
		[](const PlayerScore& a, const PlayerScore& b)
		{
			return a.score > b.score;
		});

	//Rewrite file
	std::ofstream outFile(fullPath);

	for (const auto& entry : scores)
	{
		outFile << entry.name << "," << entry.score << "\n";
	}
	outFile.close();
}

dae::ScoreBoardComponent::ScoreBoardComponent() : ObjectComponent(nullptr)
{

}
