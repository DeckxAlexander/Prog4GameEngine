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


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	auto playerRenderComponent = std::make_unique<dae::RenderComponent>(go.get(), "background.png");
	go.get()->AddComponent(std::move(playerRenderComponent));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	playerRenderComponent = std::make_unique<dae::RenderComponent>(go.get(), "logo.png");
	go.get()->AddComponent(std::move(playerRenderComponent));

	go->SetPosition(358, 180);
	scene.Add(std::move(go));

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


	go = std::make_unique<dae::GameObject>();
	playerRenderComponent = std::make_unique<dae::RenderComponent>(go.get(), "Bomb.png");
	auto playerMovementComponent = std::make_unique<dae::MovementComponent>(go.get(), 50.f);
	go.get()->AddComponent(std::move(playerRenderComponent));
	go.get()->AddComponent(std::move(playerMovementComponent));
	go.get()->SetPosition(250, 100);


	

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2 { 1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2 { -1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2{ 0.f, -1.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2{ 0.f, 1.f }));

//Player 1


	auto SteamObject = std::make_unique<dae::GameObject>();
	auto steamObserver = std::make_unique<dae::SteamObserver>(SteamObject.get());

	auto healthObject = std::make_unique<dae::GameObject>();
	auto scoreObject = std::make_unique<dae::GameObject>();
	auto healthObserver = std::make_unique<dae::HealthObserver>(healthObject.get());
	auto scoreObserver = std::make_unique<dae::ScoreObserver>(scoreObject.get());
	auto Player1 = std::make_unique<dae::GameObject>();
	playerRenderComponent = std::make_unique<dae::RenderComponent>(Player1.get(), "Bomberman.png");
	playerMovementComponent = std::make_unique<dae::MovementComponent>(Player1.get(), 100.f);
	auto playerHealthComponent = std::make_unique<dae::HealthComponent>(Player1.get(), 3);
	auto playerScoreComponent = std::make_unique<dae::ScoreComponent>(Player1.get());
	playerHealthComponent.get()->GetSubject()->AddObserver(healthObserver.get());
	playerHealthComponent.get()->GetSubject()->AddObserver(playerScoreComponent.get());
	playerScoreComponent.get()->GetSubject()->AddObserver(scoreObserver.get());
	playerScoreComponent.get()->GetSubject()->AddObserver(steamObserver.get());
	playerScoreComponent.get()->SetScore(50);

	Player1.get()->AddComponent(std::move(playerRenderComponent));
	Player1.get()->AddComponent(std::move(playerScoreComponent));
	Player1.get()->AddComponent(std::move(playerMovementComponent));
	Player1.get()->AddComponent(std::move(playerHealthComponent));
	Player1.get()->SetPosition(300, 100);


	SteamObject.get()->AddComponent(std::move(steamObserver));
	scene.Add(std::move(SteamObject));




	auto scoreTextComp = std::make_unique<dae::TextComponent>(scoreObject.get(), "50 Score", font);
	auto scoreRenderComp = std::make_unique<dae::RenderComponent>(scoreObject.get());
	scoreObject.get()->AddComponent(std::move(scoreTextComp));
	scoreObject.get()->AddComponent(std::move(scoreRenderComp));
	scoreObject.get()->AddComponent(std::move(scoreObserver));
	scoreObject.get()->SetPosition(10.f, 250.f);

	auto healthTextComp = std::make_unique<dae::TextComponent>(healthObject.get(), "3 Health", font);
	auto healthRenderComp = std::make_unique<dae::RenderComponent>(healthObject.get());
	healthObject.get()->AddComponent(std::move(healthTextComp));
	healthObject.get()->AddComponent(std::move(healthRenderComp));
	healthObject.get()->AddComponent(std::move(healthObserver));
	healthObject.get()->SetPosition(10.f, 200.f);
	scene.Add(std::move(healthObject));
	scene.Add(std::move(scoreObject));


	//Player2


	healthObject = std::make_unique<dae::GameObject>();
	scoreObject = std::make_unique<dae::GameObject>();
	healthObserver = std::make_unique<dae::HealthObserver>(healthObject.get());
	scoreObserver = std::make_unique<dae::ScoreObserver>(scoreObject.get());
	auto Player2 = std::make_unique<dae::GameObject>();
	playerRenderComponent = std::make_unique<dae::RenderComponent>(Player2.get(), "Bomberman.png");
	playerMovementComponent = std::make_unique<dae::MovementComponent>(Player2.get(), 100.f);
	playerHealthComponent = std::make_unique<dae::HealthComponent>(Player2.get(), 3);
	playerScoreComponent = std::make_unique<dae::ScoreComponent>(Player2.get());
	playerHealthComponent.get()->GetSubject()->AddObserver(healthObserver.get());
	playerHealthComponent.get()->GetSubject()->AddObserver(playerScoreComponent.get());
	playerScoreComponent.get()->GetSubject()->AddObserver(scoreObserver.get());
	playerScoreComponent.get()->SetScore(50);
	Player2.get()->AddComponent(std::move(playerRenderComponent));
	Player2.get()->AddComponent(std::move(playerMovementComponent));
	Player2.get()->AddComponent(std::move(playerHealthComponent));
	Player2.get()->AddComponent(std::move(playerScoreComponent));


	Player2.get()->SetPosition(350, 100);

	scoreTextComp = std::make_unique<dae::TextComponent>(scoreObject.get(), "50 Score", font);
	scoreRenderComp = std::make_unique<dae::RenderComponent>(scoreObject.get());
	scoreObject.get()->AddComponent(std::move(scoreTextComp));
	scoreObject.get()->AddComponent(std::move(scoreRenderComp));
	scoreObject.get()->AddComponent(std::move(scoreObserver));
	scoreObject.get()->SetPosition(10.f, 350.f);

	healthTextComp = std::make_unique<dae::TextComponent>(healthObject.get(), "3 Health", font);
	healthRenderComp = std::make_unique<dae::RenderComponent>(healthObject.get());
	healthObject.get()->AddComponent(std::move(healthTextComp));
	healthObject.get()->AddComponent(std::move(healthRenderComp));
	healthObject.get()->AddComponent(std::move(healthObserver));
	healthObject.get()->SetPosition(10.f, 300.f);

	scene.Add(std::move(healthObject));
	scene.Add(std::move(scoreObject));
	

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_U, dae::KeyState::Down, std::make_unique<dae::Damage>(Player1.get()), nullptr);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_I, dae::KeyState::Down, std::make_unique<dae::AddScore>(Player1.get()), nullptr);

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_J, dae::KeyState::Down, std::make_unique<dae::Damage>(Player2.get()), nullptr);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_K, dae::KeyState::Down, std::make_unique<dae::AddScore>(Player2.get()), nullptr);
	auto controller1 = std::make_unique<dae::Controller>(0);

	controller1->BindCommand(GAMEPAD_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(Player2.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
	controller1->BindCommand(GAMEPAD_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(Player2.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
	controller1->BindCommand(GAMEPAD_DPAD_RIGHT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(Player2.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0 }));
	controller1->BindCommand(GAMEPAD_DPAD_LEFT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(Player2.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0 }));
	controller1->BindAxis(std::make_unique<dae::MoveAround>(Player1.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 1.f }), true);


	dae::InputManager::GetInstance().AddController(std::move(controller1));



	
	scene.Add(std::move(go));
	scene.Add(std::move(Player1));
	scene.Add(std::move(Player2));


	auto ExplanationObject = std::make_unique<dae::GameObject>();
	auto ExplanationTextComp = std::make_unique<dae::TextComponent>(ExplanationObject.get(), "(U / J) - Decrease Health and Score", font);
	auto ExplanationRenderComp = std::make_unique<dae::RenderComponent>(ExplanationObject.get());
	ExplanationObject.get()->AddComponent(std::move(ExplanationTextComp));
	ExplanationObject.get()->AddComponent(std::move(ExplanationRenderComp));
	ExplanationObject.get()->SetPosition(10.f, 400.f);
	scene.Add(std::move(ExplanationObject));

	ExplanationObject = std::make_unique<dae::GameObject>();
	ExplanationTextComp = std::make_unique<dae::TextComponent>(ExplanationObject.get(), "(I / K) - Increase Score", font);
	ExplanationRenderComp = std::make_unique<dae::RenderComponent>(ExplanationObject.get());
	ExplanationObject.get()->AddComponent(std::move(ExplanationTextComp));
	ExplanationObject.get()->AddComponent(std::move(ExplanationRenderComp));
	ExplanationObject.get()->SetPosition(10.f, 440.f);
	scene.Add(std::move(ExplanationObject));

	//auto Menu = std::make_unique<dae::GameObject>();
	//auto thr = std::make_unique<dae::ThrashCacheComponent>(Menu.get());
	//Menu.get()->AddComponent(std::move(thr));
	//scene.Add(std::move(Menu));

	dae::InputManager::GetInstance().UnbindCommand(SDL_SCANCODE_B, dae::KeyState::Down);



	
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
