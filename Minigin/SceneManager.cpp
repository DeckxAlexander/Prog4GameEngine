#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_scenes[m_ActiveSceneIndex]->Update();
	
}

void dae::SceneManager::Render()
{
	m_scenes[m_ActiveSceneIndex]->Render();
	
}

dae::Scene& dae::SceneManager::CreateScene()
{
	m_scenes.emplace_back(new Scene());
	return *m_scenes.back();
}

dae::Scene& dae::SceneManager::GetActiveScene()
{

	return *m_scenes[m_ActiveSceneIndex];
}

void dae::SceneManager::SetActiveScene(size_t index)
{
	m_ActiveSceneIndex = index;
}
