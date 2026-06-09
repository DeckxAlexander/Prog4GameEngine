#include "GameManager.h"
#include "EnemyComponent.h"
#include "SceneManager.h"
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

	if (!enemiesAlive && m_ExitFound) ProcessWin();

}

void dae::GameManager::ProcessWin()
{
	std::cout << "WON";
}

void dae::GameManager::ProcessExitFound()
{
	m_ExitFound = true;
	CheckGameState();
}
