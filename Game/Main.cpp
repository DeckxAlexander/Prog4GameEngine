#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "ThrashCacheComponent.h"
#include "TextComponent.h"
#include "Scene.h"
#include "FPSCounterComponent.h"

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


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{

	srand(int(time(nullptr)));
	auto& scene = dae::SceneManager::GetInstance().CreateScene();


	auto tileGameObject = std::make_unique<dae::GameObject>();
	auto tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "background2.png");
	tileGameObject.get()->AddComponent(std::move(tileRenderComponent));
	scene.Add(std::move(tileGameObject));


	auto GridManager = std::make_unique<dae::GameObject>();
	auto GridComp = std::make_unique<dae::GridComponent>(GridManager.get(), 32, 18);
	dae::GridComponent* grid = GridComp.get();
	GridComp->SetTileScale(32.f, 32.f);
	GridManager.get()->AddComponent(std::move(GridComp));
	GridManager.get()->SetPosition(-0.3f, -0.3f);
	scene.Add(std::move(GridManager));

	grid->SetupGrid();
	grid->SpawnGrid();




	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<dae::GameObject>();
	auto tc = std::make_unique<dae::TextComponent>(to.get(),"Dae Assignment Alexander", font);
	auto tor = std::make_unique<dae::RenderComponent>(to.get());
	tc->SetColor({ 255, 255, 0, 255 });
	to.get()->AddComponent(std::move(tc));
	to.get()->AddComponent(std::move(tor));
	to->SetPosition(292, 20);
	scene.Add(std::move(to));

	auto fpso = std::make_unique<dae::GameObject>();
	auto fpsc = std::make_unique<dae::FPSCounterComponent>(fpso.get());
	auto fpst = std::make_unique<dae::TextComponent>(fpso.get(), "FPS", font);
	auto fpsr = std::make_unique<dae::RenderComponent>(fpso.get());
	fpso.get()->AddComponent(std::move(fpsc));
	fpso.get()->AddComponent(std::move(fpst));
	fpso.get()->AddComponent(std::move(fpsr));
	
	scene.Add(std::move(fpso));


	//Player
	auto playerGameObject = std::make_unique<dae::GameObject>();
	auto playerRenderComponent = std::make_unique<dae::RenderComponent>(playerGameObject.get(), "Bomberman.png");
	auto playerMovementComponent = std::make_unique<dae::MovementComponent>(playerGameObject.get(), 50.f);
	auto playerHealthComponent = std::make_unique<dae::HealthComponent>(playerGameObject.get(), 5);
	auto playerCollider = std::make_unique<dae::CollisionComponent>(playerGameObject.get(), 18.f, 27.f, 'e');
	auto playerplacebombcomponent = std::make_unique<dae::PlaceBombComponent>(playerGameObject.get(), grid);
	auto playerComponent = std::make_unique<dae::PlayerComponent>(playerGameObject.get(), 0);
	playerGameObject.get()->AddComponent(std::move(playerRenderComponent));
	playerGameObject.get()->AddComponent(std::move(playerMovementComponent));
	playerGameObject.get()->AddComponent(std::move(playerCollider));
	playerGameObject.get()->AddComponent(std::move(playerplacebombcomponent));
	playerGameObject.get()->AddComponent(std::move(playerHealthComponent));
	playerGameObject.get()->AddComponent(std::move(playerComponent));
	playerGameObject.get()->SetPosition(35, 35);
	playerGameObject.get()->SetScale(1.5f, 1.5f);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_X, dae::KeyState::Pressed, std::make_unique<dae::PlaceBomb>(playerGameObject.get()),nullptr);

	scene.Add(std::move(playerGameObject));
	auto gridLayout = grid->GetGridLayout();
	int EnemiesSpawned = 0;
	std::vector<int> possibleIndexes{};




	for (size_t i{}; i < gridLayout.size(); i++)
	{
		auto gridVal = gridLayout[int(i)];

		if (gridVal != dae::GridComponent::GridValue::empty) continue;

		possibleIndexes.push_back(int(i));
	}



	while (EnemiesSpawned < 10)
	{
		//Choose Index

		int chosenIndex = possibleIndexes[rand() % possibleIndexes.size()];
		possibleIndexes.erase(std::remove(possibleIndexes.begin(), possibleIndexes.end(), chosenIndex), possibleIndexes.end());

		auto enemyGameObject = std::make_unique<dae::GameObject>();
		auto enemyRenderComponent = std::make_unique<dae::RenderComponent>(enemyGameObject.get(), "Bomberman.png");
		auto enemyMovementComponent = std::make_unique<dae::AIMovementComponent>(enemyGameObject.get(), 50.f, grid);



		if (rand()%2 == 1) 
		{
			enemyMovementComponent.get()->SetVelocity(0.f, 1.f);
		}
		else enemyMovementComponent.get()->SetVelocity(1.f, 0.f);


		auto enemyHealthComponent = std::make_unique<dae::HealthComponent>(enemyGameObject.get(), 5);
		auto enemyCollider = std::make_unique<dae::CollisionComponent>(enemyGameObject.get(), 18.f, 27.f, 'e');
		enemyCollider.get()->AddBlockingTag('b');
		auto enemyplacebombcomponent = std::make_unique<dae::PlaceBombComponent>(enemyGameObject.get(), grid);
		auto enemyComponent = std::make_unique<dae::EnemyComponent>(enemyGameObject.get());
		enemyGameObject.get()->AddComponent(std::move(enemyRenderComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyMovementComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyCollider));
		enemyGameObject.get()->AddComponent(std::move(enemyplacebombcomponent));
		enemyGameObject.get()->AddComponent(std::move(enemyHealthComponent));
		enemyGameObject.get()->AddComponent(std::move(enemyComponent));
		int x = chosenIndex % grid->GetColums();
		int y = chosenIndex / grid->GetColums();
		enemyGameObject.get()->SetPosition(32*float(x), 32*float(y));
		enemyGameObject.get()->SetScale(1.5f, 1.5f);

		scene.Add(std::move(enemyGameObject));
		EnemiesSpawned++;
		if (EnemiesSpawned >= 10) break;
	}



	//SOUND

	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";


	const auto fullPath = data_location / "TestSound.mp3";
	const auto filename = fs::path(fullPath).filename().string();
	dae::SoundSystemLocator::register_sound_system(std::make_unique<dae::SDLSoundSystem>());
	//dae::SoundSystemLocator::get_sound_system().LoadSound(0, filename);

}

int main(int, char*[]) {

#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);


    return 0;
}
