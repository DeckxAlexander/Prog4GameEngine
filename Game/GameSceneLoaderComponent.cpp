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
#include "States.h"
#include "PlayerComponent.h"
#include "GridTransform.h"
#include "PowerUps.h"
#include "CameraFollower.h"
#include "MenuComponent.h"
#include "MenuCommands.h"
#include "ScoreDisplayComponent.h"
#include "LivesDisplayComponent.h"
#include <fstream>

void dae::GameSceneLoader::SetupScene(SceneDetails details)
{
	srand(details.randomSeed);
	
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	dae::SceneManager::GetInstance().SetActiveScene(scene);

	auto& camera = dae::Renderer::GetInstance().GetCamera();
	if (details.playersAmount == 1 && !details.isVersus)
	{ 
		camera.SetViewDimensions(640, 280); //960 420
		camera.x = 0;
		camera.y = 0;
	}
	else 
	{ 
		camera.SetViewDimensions(1235, 560); 
		camera.x = 0;
		camera.y = -70;
	}



	auto tileGameObject = std::make_unique<dae::GameObject>();
	auto tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "background2.png");
	tileRenderComponent->SetRenderOnScreen(true);
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

	auto normalfont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48);
	auto pixelfont = dae::ResourceManager::GetInstance().LoadFont("Pixel.otf", 48);

	auto fpso = std::make_unique<dae::GameObject>();
	auto fpsc = std::make_unique<dae::FPSCounterComponent>(fpso.get());
	auto fpst = std::make_unique<dae::TextComponent>(fpso.get(), "FPS", normalfont);
	fpst.get()->SetColor({ 0,0,0,255 });
	auto fpsr = std::make_unique<dae::RenderComponent>(fpso.get());
	fpsr.get()->SetRenderOnScreen(true);
	fpso.get()->AddComponent(std::move(fpsc));
	fpso.get()->AddComponent(std::move(fpst));
	fpso.get()->AddComponent(std::move(fpsr));
	fpso->SetPosition(50, 20);

	scene.Add(std::move(fpso));

	auto livesGameObject = std::make_unique<dae::GameObject>();
	auto livesRenderComponent = std::make_unique<dae::RenderComponent>(livesGameObject.get());
	auto livesTextComponent = std::make_unique<dae::TextComponent>(livesGameObject.get(), "0", pixelfont);
	livesTextComponent.get()->SetColor({ 0,0,0,255 });
	livesRenderComponent->SetRenderOnScreen(true);
	livesGameObject.get()->AddComponent(std::move(livesTextComponent));
	livesGameObject.get()->AddComponent(std::make_unique<dae::LivesDisplayComponent>());
	livesGameObject.get()->AddComponent(std::move(livesRenderComponent));
	livesGameObject->SetPosition(1050 - float(100* details.playersAmount), 20.f);
	scene.Add(std::move(livesGameObject));


	auto scoreGameObject = std::make_unique<dae::GameObject>();
	auto scoreRenderComponent = std::make_unique<dae::RenderComponent>(scoreGameObject.get());
	auto scoreTextComponent = std::make_unique<dae::TextComponent>(scoreGameObject.get(), "0", pixelfont);
	scoreTextComponent.get()->SetColor({ 0,0,0,255 });
	scoreRenderComponent->SetRenderOnScreen(true);
	scoreGameObject.get()->AddComponent(std::move(scoreTextComponent));
	scoreGameObject.get()->AddComponent(std::make_unique<dae::ScoreDisplayComponent>());
	scoreGameObject.get()->AddComponent(std::move(scoreRenderComponent));
	scoreGameObject->SetPosition(512.f, 20.f);
	scene.Add(std::move(scoreGameObject));



	//Players
	if (details.playersAmount > 0)
	{
		if (details.playersAmount == 1 && !details.isVersus)
		{
			auto CameraFollowerGameObject = std::make_unique<dae::GameObject>();
			auto CameraFollowerComponent = std::make_unique<dae::CameraFollowerComponent>(float(details.width) * 32.f, float(details.height) * 32.f);
			CameraFollowerGameObject.get()->AddComponent(std::move(CameraFollowerComponent));
			scene.Add(std::move(CameraFollowerGameObject));
		}

		SpawnPlayers(details.playersAmount);
	}

	SpawnEnemies(details);

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_F1, dae::KeyState::Up, std::make_unique<dae::ToggleMuteCommand>(), nullptr);

	
	scene.Start();

}

void dae::GameSceneLoader::SpawnPlayers(int amount)
{
	InputManager::GetInstance().UnbindAll();
	auto grid = GridLocator::GetGrid();
	auto& scene = SceneManager::GetInstance().GetActiveScene();

	for (int index{}; index < amount; index++)
	{
		auto playerGameObject = std::make_unique<dae::GameObject>();
		auto playerRenderComponent = std::make_unique<dae::RenderComponent>(playerGameObject.get(), "Bomberman.png");
		auto playerMovementComponent = std::make_unique<dae::PlayerMovementComponent>(playerGameObject.get(), 60.f, grid);
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
		playerGameObject.get()->SetPosition(50.f, 50.f*(index+1));
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
		else {
			auto controller1 = std::make_unique<Controller>(index-1);
			controller1->BindAxis(std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 1.f }), true);
			controller1->BindCommand(GAMEPAD_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
			controller1->BindCommand(GAMEPAD_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
			controller1->BindCommand(GAMEPAD_DPAD_LEFT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
			controller1->BindCommand(GAMEPAD_DPAD_RIGHT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
			controller1->BindCommand(GAMEPAD_A, dae::KeyState::Up, std::make_unique<dae::PlaceBomb>(playerGameObject.get()), nullptr);
			controller1->BindCommand(GAMEPAD_X, dae::KeyState::Up, std::make_unique<dae::DetonateBomb>(playerGameObject.get()), nullptr);
			dae::InputManager::GetInstance().AddController(std::move(controller1));
		}
		scene.Add(std::move(playerGameObject));
	}
}

void dae::GameSceneLoader::SpawnVersusPlayer(int x, int y, GridComponent* grid, Scene& scene)
{
	int playerIndex = 1;
	auto versusPlayerGameObject = std::make_unique<dae::GameObject>();
	auto versusPlayerRenderComponent = std::make_unique<dae::RenderComponent>(versusPlayerGameObject.get(), "Balloom.png");
	auto versusPlayerMovementComponent = std::make_unique<dae::MovementComponent>(versusPlayerGameObject.get(), 60.f, grid);
	auto versusPlayerHealthComponent = std::make_unique<dae::HealthComponent>(versusPlayerGameObject.get());
	auto versusPlayerCollider = std::make_unique<dae::CollisionComponent>(versusPlayerGameObject.get(), 18.f, 27.f, 'e');
	auto versusEnemyComponent = std::make_unique<dae::EnemyComponent>(versusPlayerGameObject.get(), EnemyComponent::IntelligenceType::player);
	versusPlayerGameObject.get()->AddComponent(std::move(versusPlayerRenderComponent));
	versusPlayerGameObject.get()->AddComponent(std::move(versusPlayerMovementComponent));
	versusPlayerGameObject.get()->AddComponent(std::move(versusPlayerCollider));
	versusPlayerGameObject.get()->AddComponent(std::move(versusPlayerHealthComponent));
	versusPlayerGameObject.get()->AddComponent(std::move(versusEnemyComponent));
	versusPlayerGameObject.get()->SetScale(1.5f, 1.5f);

	float tilescaleX = grid->GetTileScale().x;
	float tilescaleY = grid->GetTileScale().y;
	versusPlayerGameObject.get()->SetPosition(tilescaleX * float(x) + tilescaleX * 0.5f, tilescaleY * float(y) + tilescaleY * 0.5f);

	//Bindings
	if (playerIndex == 0)
	{
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
			dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
	}
	else {
		auto controller1 = std::make_unique<Controller>(playerIndex - 1);
		controller1->BindAxis(std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 1.f }), true);
		controller1->BindCommand(GAMEPAD_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
		controller1->BindCommand(GAMEPAD_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
		controller1->BindCommand(GAMEPAD_DPAD_LEFT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
		controller1->BindCommand(GAMEPAD_DPAD_RIGHT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(versusPlayerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
		dae::InputManager::GetInstance().AddController(std::move(controller1));
	}
	scene.Add(std::move(versusPlayerGameObject));
	
}

void dae::GameSceneLoader::SpawnEnemies(SceneDetails details)
{
	auto grid = GridLocator::GetGrid();
	auto& scene = SceneManager::GetInstance().GetActiveScene();
	auto gridLayout = grid->GetGridLayout();
	int EnemiesSpawned = 0;
	std::vector<int> possibleIndexes{};




	for (size_t i{}; i < gridLayout.size(); i++)
	{
		auto gridVal = gridLayout[int(i)];

		if (gridVal != dae::GridComponent::GridValue::empty) continue;

		possibleIndexes.push_back(int(i));
	}

	if (details.isVersus)
	{
		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();
		SpawnVersusPlayer(x, y, grid, scene);
	}

	while (EnemiesSpawned < details.balloomCount)
	{
		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();

		SpawnBalloom(x, y, grid, scene);

		EnemiesSpawned++;
	}

	EnemiesSpawned = 0;
	while (EnemiesSpawned < details.onealCount)
	{
		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();

		SpawnOneal(x, y, grid, scene);

		EnemiesSpawned++;
	}

	EnemiesSpawned = 0;
	while (EnemiesSpawned < details.dollCount)
	{
		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();

		SpawnDoll(x, y, grid, scene);

		EnemiesSpawned++;
	}

	EnemiesSpawned = 0;
	while (EnemiesSpawned < details.minvoCount)
	{
		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();

		SpawnMinvo(x, y, grid, scene);

		EnemiesSpawned++;
	}
}

void dae::GameSceneLoader::SpawnBalloom(int x, int y, GridComponent* grid, Scene& scene)
{
	auto enemyGameObject = std::make_unique<dae::GameObject>();
	auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Balloom.png");
	auto enemyMovementComponent = std::make_unique<dae::WanderMovementComponent>(enemyGameObject.get(), 40.f, grid);
	if (rand() % 2 == 1)
	{
		enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
	}
	else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);

	auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get());
	auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
	enemyCollider.get()->AddBlockingTag('b');
	auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get(), EnemyComponent::IntelligenceType::normal);
	enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyCollider));
	enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyComponent));

	float tilescaleX = grid->GetTileScale().x;
	float tilescaleY = grid->GetTileScale().y;
	enemyGameObject.get()->SetPosition(tilescaleX * float(x) + tilescaleX*0.5f, tilescaleY * float(y) + tilescaleY * 0.5f);
	enemyGameObject.get()->SetScale(1.5f, 1.5f);
	enemyGameObject->GetComponentByType<dae::WanderMovementComponent>()->SetEnabled(false);
	scene.Add(std::move(enemyGameObject));
}

void dae::GameSceneLoader::SpawnOneal(int x, int y, GridComponent* grid, Scene& scene)
{
	auto enemyGameObject = std::make_unique<dae::GameObject>();
	auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Oneal.png");
	auto enemyMovementComponent = std::make_unique<dae::WanderMovementComponent>(enemyGameObject.get(), 60.f, grid);
	auto enemyChaseMovementComponent = std::make_unique<dae::ChaseMovementComponent>(enemyGameObject.get(), 60.f, grid);

	if (rand() % 2 == 1)
	{
		enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
	}
	else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);

	auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get());
	auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
	enemyCollider.get()->AddBlockingTag('b');
	auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get(), EnemyComponent::IntelligenceType::smart);
	enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyCollider));
	enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyChaseMovementComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyComponent));

	float tilescaleX = grid->GetTileScale().x;
	float tilescaleY = grid->GetTileScale().y;
	enemyGameObject.get()->SetPosition(tilescaleX * float(x) + tilescaleX * 0.5f, tilescaleY * float(y) + tilescaleY * 0.5f);
	enemyGameObject.get()->SetScale(1.5f, 1.5f);
	enemyGameObject->GetComponentByType<dae::ChaseMovementComponent>()->SetEnabled(false);
	enemyGameObject->GetComponentByType<dae::WanderMovementComponent>()->SetEnabled(false);
	scene.Add(std::move(enemyGameObject));
}

void dae::GameSceneLoader::SpawnDoll(int x, int y, GridComponent* grid, Scene& scene)
{
	auto enemyGameObject = std::make_unique<dae::GameObject>();
	auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Doll.png");
	auto enemyMovementComponent = std::make_unique<dae::WanderMovementComponent>(enemyGameObject.get(), 60.f, grid);

	if (rand() % 2 == 1)
	{
		enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
	}
	else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);

	auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get());
	auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
	enemyCollider.get()->AddBlockingTag('b');
	auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get(), EnemyComponent::IntelligenceType::normal);
	enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyCollider));
	enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
	
	enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyComponent));

	float tilescaleX = grid->GetTileScale().x;
	float tilescaleY = grid->GetTileScale().y;
	enemyGameObject.get()->SetPosition(tilescaleX * float(x) + tilescaleX * 0.5f, tilescaleY * float(y) + tilescaleY * 0.5f);
	enemyGameObject.get()->SetScale(1.5f, 1.5f);
	enemyGameObject->GetComponentByType<dae::WanderMovementComponent>()->SetEnabled(false);
	scene.Add(std::move(enemyGameObject));
}

void dae::GameSceneLoader::SpawnMinvo(int x, int y, GridComponent* grid, Scene& scene)
{
	auto enemyGameObject = std::make_unique<dae::GameObject>();
	auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Minvo.png");
	auto enemyMovementComponent = std::make_unique<dae::WanderMovementComponent>(enemyGameObject.get(), 80.f, grid);
	auto enemyChaseMovementComponent = std::make_unique<dae::ChaseMovementComponent>(enemyGameObject.get(), 80.f, grid);

	if (rand() % 2 == 1)
	{
		enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
	}
	else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);

	auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get());
	auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
	enemyCollider.get()->AddBlockingTag('b');
	auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get(), EnemyComponent::IntelligenceType::smart);
	enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyCollider));
	enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyChaseMovementComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
	enemyGameObject.get()->AddComponent(std::move(enemyComponent));

	float tilescaleX = grid->GetTileScale().x;
	float tilescaleY = grid->GetTileScale().y;
	enemyGameObject.get()->SetPosition(tilescaleX * float(x) + tilescaleX * 0.5f, tilescaleY * float(y) + tilescaleY * 0.5f);
	enemyGameObject.get()->SetScale(1.5f, 1.5f);
	enemyGameObject->GetComponentByType<dae::ChaseMovementComponent>()->SetEnabled(false);
	enemyGameObject->GetComponentByType<dae::WanderMovementComponent>()->SetEnabled(false);
	scene.Add(std::move(enemyGameObject));
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

void dae::GameSceneLoader::OpenMainMenu()
{
	InputManager::GetInstance().UnbindAll();
	SceneManager::GetInstance().SetActiveScene(0);
	auto menuComponent = SceneManager::GetInstance().GetActiveScene().GetAllObjectsByComponent<MenuComponent>().front()->GetComponentByType<MenuComponent>();
	InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Up, std::make_unique<dae::MoveMenuCommand>(menuComponent), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
	InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Up, std::make_unique<dae::MoveMenuCommand>(menuComponent), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
	InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, dae::KeyState::Up, std::make_unique<dae::ExecuteMenuCommand>(menuComponent), nullptr);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_F1, dae::KeyState::Up, std::make_unique<dae::ToggleMuteCommand>(), nullptr);
}






