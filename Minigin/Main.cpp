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
#include "HealthDisplay.h"
#include "HealthComponent.h"
#include "InputManager.h"


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	auto gor = std::make_unique<dae::RenderComponent>(go.get(), "background.png");
	go.get()->AddComponent(std::move(gor));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	gor = std::make_unique<dae::RenderComponent>(go.get(), "logo.png");
	go.get()->AddComponent(std::move(gor));

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
	gor = std::make_unique<dae::RenderComponent>(go.get(), "Bomb.png");
	auto gom = std::make_unique<dae::MovementComponent>(go.get(), 50.f);
	go.get()->AddComponent(std::move(gor));
	go.get()->AddComponent(std::move(gom));
	go.get()->SetPosition(250, 100);


	

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2 { 1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2 { -1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2{ 0.f, -1.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(go.get()), std::make_unique<dae::CommandValue>( glm::vec2{ 0.f, 1.f }));

	auto healthObject = std::make_unique<dae::GameObject>();
	auto healthDisplayComp = std::make_unique<dae::HealthDisplayComponent>(healthObject.get());
	auto obs = new dae::HealthObserver(healthDisplayComp.get());
	auto Dgo = std::make_unique<dae::GameObject>();
	gor = std::make_unique<dae::RenderComponent>(Dgo.get(), "Bomberman.png");
	gom = std::make_unique<dae::MovementComponent>(Dgo.get(), 100.f);
	auto goh = std::make_unique<dae::HealthComponent>(Dgo.get(), 3);
	goh.get()->GetSubject()->AddObserver(obs);

	Dgo.get()->AddComponent(std::move(gor));
	Dgo.get()->AddComponent(std::move(gom));
	Dgo.get()->AddComponent(std::move(goh));
	Dgo.get()->SetPosition(300, 100);




	auto healthTextComp = std::make_unique<dae::TextComponent>(healthObject.get(), "Healh", font);
	auto healthRenderComp = std::make_unique<dae::RenderComponent>(healthObject.get());
	healthObject.get()->AddComponent(std::move(healthDisplayComp));
	healthObject.get()->AddComponent(std::move(healthTextComp));
	healthObject.get()->AddComponent(std::move(healthRenderComp));

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_U, dae::KeyState::Down, std::make_unique<dae::Damage>(Dgo.get()), nullptr);

	healthObject.get()->SetPosition(200.f, 200.f);

	scene.Add(std::move(healthObject));
	auto child = std::make_unique<dae::GameObject>();
	gor = std::make_unique<dae::RenderComponent>(child.get(), "Bomberman.png");
	gom = std::make_unique<dae::MovementComponent>(child.get(), 100.f);
	child.get()->AddComponent(std::move(gor));
	child.get()->AddComponent(std::move(gom));
	child.get()->SetPosition(350, 100);

	auto controller1 = std::make_unique<dae::Controller>(0);

	controller1->BindCommand(GAMEPAD_DPAD_DOWN, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(child.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, 1.f }));
	controller1->BindCommand(GAMEPAD_DPAD_UP, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(child.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 0.f, -1.f }));
	controller1->BindCommand(GAMEPAD_DPAD_RIGHT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(child.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0 }));
	controller1->BindCommand(GAMEPAD_DPAD_LEFT, dae::KeyState::Pressed, std::make_unique<dae::MoveAround>(child.get()), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0 }));
	controller1->BindAxis(std::make_unique<dae::MoveAround>(Dgo.get()), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 1.f }), true);


	dae::InputManager::GetInstance().AddController(std::move(controller1));

	scene.Add(std::move(go));
	scene.Add(std::move(Dgo));
	scene.Add(std::move(child));


	auto Menu = std::make_unique<dae::GameObject>();
	auto thr = std::make_unique<dae::ThrashCacheComponent>(Menu.get());
	Menu.get()->AddComponent(std::move(thr));
	scene.Add(std::move(Menu));


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
