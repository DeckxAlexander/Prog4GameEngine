#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_scenes[m_ActiveSceneIndex]->Update();
	Cleanup();
}

void dae::SceneManager::Render()
{
	m_scenes[m_ActiveSceneIndex]->Render();
	
}

void dae::SceneManager::Cleanup()
{
	Scene& activeScene = GetActiveScene();
	m_scenes.erase(
		std::remove_if(
			m_scenes.begin(),
			m_scenes.end(),
			[&activeScene](const std::unique_ptr<Scene>& s)
			{
				return s->m_MarkedForDelete && s.get() != &activeScene;
			}),
		m_scenes.end());

	SetActiveScene(activeScene);
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


void dae::SceneManager::RemoveScene(Scene& scene)
{
	scene.m_MarkedForDelete = true;
}

void dae::SceneManager::SetActiveScene(size_t index)
{
	m_ActiveSceneIndex = index;
}

void dae::SceneManager::SetActiveScene(const Scene& scene)
{
	auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
		[&scene](const std::unique_ptr<Scene>& s)
		{
			return s.get() == &scene;
		});

	if (it != m_scenes.end())
	{
		m_ActiveSceneIndex = std::distance(m_scenes.begin(), it);
	}
}
