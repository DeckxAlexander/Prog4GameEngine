#include "GameSceneLoaderComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "ThrashCacheComponent.h"
#include "TextComponent.h"
#include "Scene.h"
#include "FPSCounterComponent.h"
#include "Renderer.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "SteamObserver.h"
#include "GridComponent.h"
#include "CollisionComponent.h"
#include "BombComponent.h"
#include "PlaceBombComponent.h"
#include "EnemyComponent.h"
#include "GameCommands.h"
#include "SDLSoundSystem.h"
#include "States.h"
#include "PlayerComponent.h"
#include "GridTransform.h"
#include "PowerUps.h"
#include "CameraFollower.h"
#include "MenuComponent.h"
#include "MenuCommands.h"
#include <fstream>

void dae::GameSceneLoader::SetupScene(SceneDetails details)
{
	srand(details.randomSeed);
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	dae::SceneManager::GetInstance().SetActiveScene(scene);

	auto& camera = dae::Renderer::GetInstance().GetCamera();
	camera.SetViewDimensions(640, 280); //960 420
	camera.x = 0;
	camera.y = 0;
	auto tileGameObject = std::make_unique<dae::GameObject>();
	auto tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "background2.png");
	tileGameObject.get()->AddComponent(std::move(tileRenderComponent));
	tileGameObject->SetPosition(512.f, 288.f);
	scene.Add(std::move(tileGameObject));

	auto GridManager = std::make_unique<dae::GameObject>();
	auto GridComp = std::make_unique<dae::GridComponent>(GridManager.get(), details.width, details.height); //31 13
	dae::GridComponent* grid = GridComp.get();
	GridComp->SetTileScale(32.f, 32.f);
	GridManager.get()->AddComponent(std::move(GridComp));
	GridManager.get()->SetPosition(-0.3f, -0.3f);
	scene.Add(std::move(GridManager));
	grid->SetSoftBlocksAmount(details.blocksCount);
	grid->SetupGrid();
	grid->SpawnPowerUps();
	grid->SpawnGrid();
	dae::GridLocator::SetGrid(grid);
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto fpso = std::make_unique<dae::GameObject>();
	auto fpsc = std::make_unique<dae::FPSCounterComponent>(fpso.get());
	auto fpst = std::make_unique<dae::TextComponent>(fpso.get(), "FPS", font);
	auto fpsr = std::make_unique<dae::RenderComponent>(fpso.get());
	fpsr.get()->SetRenderOnScreen(true);
	fpso.get()->AddComponent(std::move(fpsc));
	fpso.get()->AddComponent(std::move(fpst));
	fpso.get()->AddComponent(std::move(fpsr));
	fpso->SetPosition(50, 20);

	scene.Add(std::move(fpso));

	//Players
	if (details.playersAmount > 0) dae::InputManager::GetInstance().UnbindAll();

	if (details.playersAmount == 1) 
	{
		auto CameraFollowerGameObject = std::make_unique<dae::GameObject>();
		auto CameraFollowerComponent = std::make_unique<dae::CameraFollowerComponent>(float(details.width)*32.f, float(details.height) * 32.f);
		CameraFollowerGameObject.get()->AddComponent(std::move(CameraFollowerComponent));
		scene.Add(std::move(CameraFollowerGameObject));
	}

	for (int index{}; index < details.playersAmount; index++)
	{
	
		auto playerGameObject = std::make_unique<dae::GameObject>();
		auto playerRenderComponent = std::make_unique<dae::RenderComponent>(playerGameObject.get(), "Bomberman.png");
		auto playerMovementComponent = std::make_unique<dae::MovementComponent>(playerGameObject.get(), 50.f, grid);
		auto playerHealthComponent = std::make_unique<dae::HealthComponent>(playerGameObject.get());
		auto playerCollider = std::make_unique<dae::CollisionComponent>(playerGameObject.get(), 18.f, 27.f, 'e');
		auto playerplacebombcomponent = std::make_unique<dae::PlaceBombComponent>(playerGameObject.get(), grid);
		auto playerComponent = std::make_unique<dae::PlayerComponent>(playerGameObject.get(), index);
		playerGameObject.get()->AddComponent(std::move(playerRenderComponent));
		playerGameObject.get()->AddComponent(std::move(playerMovementComponent));
		playerGameObject.get()->AddComponent(std::move(playerCollider));
		playerGameObject.get()->AddComponent(std::move(playerplacebombcomponent));
		playerGameObject.get()->AddComponent(std::move(playerHealthComponent));
		playerGameObject.get()->AddComponent(std::move(playerComponent));
		playerGameObject.get()->SetPosition(50, 50);
		playerGameObject.get()->SetScale(1.5f, 1.5f);

		//Bindings
		if (index == 0) 
		{
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_X, dae::KeyState::Up, std::make_unique<dae::PlaceBomb>(playerGameObject.get()), nullptr);
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_C, dae::KeyState::Up, std::make_unique<dae::DetonateBomb>(playerGameObject.get()), nullptr);
		}
		auto controller1 = std::make_unique<Controller>(index);
		controller1->BindAxis(std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 1.f }), true);
		dae::InputManager::GetInstance().AddController(std::move(controller1));

		scene.Add(std::move(playerGameObject));
	}

	auto gridLayout = grid->GetGridLayout();
	int EnemiesSpawned = 0;
	std::vector<int> possibleIndexes{};

	for (size_t i{}; i < gridLayout.size(); i++)
	{
		auto gridVal = gridLayout[int(i)];

		if (gridVal != dae::GridComponent::GridValue::empty) continue;

		possibleIndexes.push_back(int(i));
	}

	while (EnemiesSpawned < details.balloomCount)
	{
		//Choose Index

		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());

		auto enemyGameObject = std::make_unique<dae::GameObject>();
		auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Balloom.png");
		auto enemyMovementComponent = std::make_unique<dae::WanderMovementComponent>(enemyGameObject.get(), 40.f, grid);
		auto enemyChaseMovementComponent = std::make_unique<dae::ChaseMovementComponent>(enemyGameObject.get(), 25.f, grid);
		if (rand() % 2 == 1)
		{
			enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
		}
		else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);


		auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get());
		auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
		enemyCollider.get()->AddBlockingTag('b');
		auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get(), false);
		enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyCollider));
		enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyChaseMovementComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyComponent));
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();
		enemyGameObject.get()->SetPosition(32 * float(x) + 16, 32 * float(y) + 16);
		enemyGameObject.get()->SetScale(1.5f, 1.5f);
		enemyGameObject->GetComponentByType<dae::ChaseMovementComponent>()->SetEnabled(false);
		scene.Add(std::move(enemyGameObject));
		EnemiesSpawned++;
		if (EnemiesSpawned >= 10) break;


	}






	//EnemiesSpawned = 0;
	//
	//while (EnemiesSpawned < 5)
	//{
	//	//Choose Index

	//	int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
	//	possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());

	//	auto enemyGameObject = std::make_unique<dae::GameObject>();
	//	auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Oneal.png");
	//	auto enemyMovementComponent = std::make_unique<dae::WanderMovementComponent>(enemyGameObject.get(), 60.f, grid);
	//	auto enemyChaseMovementComponent = std::make_unique<dae::ChaseMovementComponent>(enemyGameObject.get(), 60.f, grid);




	//	if (rand() % 2 == 1)
	//	{
	//		enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
	//	}
	//	else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);


	//	auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get());
	//	auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
	//	enemyCollider.get()->AddBlockingTag('b');
	//	auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get(), true);
	//	enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
	//	enemyGameObject.get()->AddComponent(std::move(enemyCollider));
	//	enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
	//	enemyGameObject.get()->AddComponent(std::move(enemyChaseMovementComponent));
	//	enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
	//	enemyGameObject.get()->AddComponent(std::move(enemyComponent));
	//	int x = chosenIndex % grid->GetColums();
	//	int y = chosenIndex / grid->GetColums();
	//	enemyGameObject.get()->SetPosition(32 * float(x) + 16, 32 * float(y) + 16);
	//	enemyGameObject.get()->SetScale(1.5f, 1.5f);
	//	enemyGameObject->GetComponentByType<dae::ChaseMovementComponent>()->SetEnabled(false);
	//	scene.Add(std::move(enemyGameObject));
	//	EnemiesSpawned++;
	//	if (EnemiesSpawned >= 5) break;


	//}


	scene.Start();

}


void dae::GameSceneLoader::LoadLevelFromFile(std::string filename, int playersAmount, bool isVersus)
{
	const auto fullPath = m_dataPath / filename;
	std::ifstream file(fullPath);
	if (!file)
	{
		throw std::runtime_error("Failed to open level file: " + filename);
	}

	SceneDetails sceneDetails{};
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
			break;

		size_t pos = line.find('=');

		if (pos == std::string::npos)
			continue;

		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 1);

		if (key == "seed")
			sceneDetails.randomSeed = std::stoi(value);
		else if (key == "width")
			sceneDetails.width = std::stoi(value);
		else if (key == "height")
			sceneDetails.height = std::stoi(value);
		else if (key == "blockscount")
			sceneDetails.blocksCount = std::stoi(value);
		else if (key == "bcount")
			sceneDetails.balloomCount = std::stoi(value);
		else if (key == "ocount")
			sceneDetails.onealCount = std::stoi(value);
		else if (key == "dcount")
			sceneDetails.dollCount = std::stoi(value);
		else if (key == "mcount")
			sceneDetails.minvoCount = std::stoi(value);
	}
	sceneDetails.playersAmount = playersAmount;
	sceneDetails.isVersus = isVersus;
	file.close();
	SetupScene(sceneDetails);
}






