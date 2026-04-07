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
#include "ScoreManager.h"
#include "Scene.h"
#include "FPSCounterComponent.h"
#include "HealthObserver.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "SteamObserver.h"
#include "GridComponent.h"
#include "CollisionComponent.h"
#include "BombComponent.h"
#include "PlaceBombComponent.h"


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
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
	auto playerCollider = std::make_unique<dae::CollisionComponent>(playerGameObject.get(), 18.f, 27.f);
	auto playerplacebombcomponent = std::make_unique<dae::PlaceBombComponent>(playerGameObject.get(), grid);
	playerGameObject.get()->AddComponent(std::move(playerRenderComponent));
	playerGameObject.get()->AddComponent(std::move(playerMovementComponent));
	playerGameObject.get()->AddComponent(std::move(playerCollider));
	playerGameObject.get()->AddComponent(std::move(playerplacebombcomponent));
	playerGameObject.get()->SetPosition(35, 35);
	playerGameObject.get()->SetScale(1.5f, 1.5f);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(playerGameObject.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_X, dae::KeyState::Pressed, std::make_unique<dae::PlaceBomb>(playerGameObject.get()),nullptr);

	scene.Add(std::move(playerGameObject));


	////Place TestBomb
	//auto bombGameObject = std::make_unique<dae::GameObject>(true, grid);
	//auto bombRenderComponent = std::make_unique<dae::RenderComponent>(bombGameObject.get(), "Bomb.png");
	//auto bombBombComponent = std::make_unique<dae::BombComponent>(bombGameObject.get(), 5.f);
	//auto bombEx = bombBombComponent.get();
	//bombGameObject.get()->AddComponent(std::move(bombRenderComponent));
	//bombGameObject.get()->AddComponent(std::move(bombBombComponent));
	//bombGameObject.get()->SetPosition(35, 35);
	//bombGameObject.get()->SetScale(1.5f, 1.5f);


	//scene.Add(std::move(bombGameObject));
	//bombEx->StartDetonate();

	
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
