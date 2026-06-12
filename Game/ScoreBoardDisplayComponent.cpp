#include "ScoreBoardDisplayComponent.h"
#include <fstream>
#include "TextComponent.h"
#include "SceneManager.h"
#include "GameSceneLoaderComponent.h"
#include "GameManager.h"
#include "SoundSystem.h"
#include <string>
#include <sstream>


void dae::ScoreBoardDisplayComponent::AddText(TextComponent* text)
{
	m_Displays.push_back(text);
}

void dae::ScoreBoardDisplayComponent::RemoveText(size_t index)
{
	m_Displays.erase(m_Displays.begin() + index);
}

void dae::ScoreBoardDisplayComponent::RefreshScore(const std::string& filename)
{
	struct PlayerScore {
		std::string name;
		std::string score;
	};


	for (auto text : m_Displays)
	{
		text->SetText(" - ");
	}

	const auto fullPath = m_dataPath / filename;
	std::vector<PlayerScore> scores{};
	std::ifstream inFile(fullPath);

	if (!inFile) return;
	
	std::string line;

	while (std::getline(inFile, line))
	{
		std::stringstream ss(line);

		std::string name;
		std::string scoreStr;

		if (std::getline(ss, name, ',') &&
			std::getline(ss, scoreStr))
		{
			scores.push_back({ name, scoreStr });
		}
	}
	inFile.close();

	size_t index{};
	for (auto text : m_Displays)
	{
		if (index >= scores.size()) break;
		text->SetText(scores[index].name + " - " + scores[index].score);
		index++;
	}
	


	

}

dae::ScoreBoardDisplayComponent::ScoreBoardDisplayComponent() : ObjectComponent()
{}
