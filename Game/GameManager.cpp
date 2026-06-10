#include "GameManager.h"
#include "EnemyComponent.h"
#include "SceneManager.h"
#include "PlayerComponent.h"
#include "GameSceneLoaderComponent.h"
#include "GameObject.h"
#include <iostream>

void dae::GameManager::CheckGameState()
{
	auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	auto enemies = scene.GetAllObjectsByComponent<EnemyComponent>();

	bool enemiesAlive{ false };
	for (auto enemy : enemies)
	{
		if (!enemy->IsMarkedForDelete()) 
		{
			enemiesAlive = true; 
			break;
		}
	}

	if (!enemiesAlive) ProcessWin();

}

void dae::GameManager::ProcessWin()
{
	m_CurrentLevel++;
	auto& oldScene = SceneManager::GetInstance().GetActiveScene();
	auto players = oldScene.GetAllObjectsByComponent<PlayerComponent>();
	std::vector<int> savedLives(m_PlayerAmount);
	for (auto player : players) 
	{
		auto playerComp = player->GetComponentByType<PlayerComponent>();
		savedLives[playerComp->m_PlayerIndex] = playerComp->m_PlayerLives;
	}
	std::string filename = "Levels/Level" + std::to_string(m_CurrentLevel) + ".txt";

	GameSceneLoader::GetInstance().LoadLevelFromFile(filename, m_PlayerAmount, false);

	players = SceneManager::GetInstance().GetActiveScene().GetAllObjectsByComponent<PlayerComponent>();
	for (auto player : players)
	{
		auto playerComp = player->GetComponentByType<PlayerComponent>();
		int lives = savedLives[playerComp->m_PlayerIndex];
		playerComp->m_PlayerLives = savedLives[playerComp->m_PlayerIndex];
		if (lives <= 0) playerComp->PlayerDeath();

	}
	SceneManager::GetInstance().RemoveScene(oldScene);


}

void dae::GameManager::ProcessGameOver()
{

	auto& oldScene = SceneManager::GetInstance().GetActiveScene();
	GameSceneLoader::GetInstance().OpenMainMenu();
	SceneManager::GetInstance().RemoveScene(oldScene);
}

void dae::GameManager::ResetGame()
{
	m_CurrentLevel = 1;
}

void dae::GameManager::SetPlayerAmount(int amount)
{
	m_PlayerAmount = amount;
}

void dae::GameManager::CheckPlayerDeath()
{

	auto players = SceneManager::GetInstance().GetActiveScene().GetAllObjectsByComponent<PlayerComponent>();
	if (players.size() < 2) ProcessGameOver();

}



