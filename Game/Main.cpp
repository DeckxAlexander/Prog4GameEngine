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
#include "GameSceneLoaderComponent.h"
#include "ScoreBoardDisplayComponent.h"
#include <filesystem>


namespace fs = std::filesystem;

static void load()
{
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	dae::GameSceneLoader::GetInstance().SetDataPath(data_location);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Pixel.otf", 36);
	auto& scene = dae::SceneManager::GetInstance().CreateScene();
	dae::SceneManager::GetInstance().SetActiveScene(0);
	auto menuGameObject = std::make_unique<dae::GameObject>();
	auto menuRenderComponentLogo = std::make_unique<dae::RenderComponent>(menuGameObject.get(), "BombermanLogo.png");
	menuRenderComponentLogo.get()->SetRenderOnScreen(true);
	menuGameObject->AddComponent(std::make_unique<dae::MenuComponent>());
	menuGameObject->AddComponent(std::move(menuRenderComponentLogo));
	auto menucomponent = menuGameObject->GetComponentByType<dae::MenuComponent>();
	menuGameObject->SetPosition(512, 150);
	menuGameObject->SetScale(1.5f, 1.5f);


	//SOLO BUTTON
	auto textSolobutton = std::make_unique<dae::GameObject>();
	auto textSolobuttoncomponent = std::make_unique<dae::TextComponent>(textSolobutton.get(), "Solo", font);
	auto textSolobuttonRendercomponent = std::make_unique<dae::RenderComponent>(textSolobutton.get());

	menucomponent->AddButton(textSolobuttoncomponent.get(), dae::MenuComponent::MenuEvent::Solo);

	textSolobuttonRendercomponent.get()->SetRenderOnScreen(true);
	textSolobutton.get()->AddComponent(std::move(textSolobuttoncomponent));
	textSolobutton.get()->AddComponent(std::move(textSolobuttonRendercomponent));
	textSolobutton->SetPosition(512, 300);

	//COOP BUTTON
	auto textCoopbutton = std::make_unique<dae::GameObject>();
	auto textCoopbuttoncomponent = std::make_unique<dae::TextComponent>(textCoopbutton.get(), "Coop", font);
	auto textCoopbuttonRendercomponent = std::make_unique<dae::RenderComponent>(textCoopbutton.get());

	menucomponent->AddButton(textCoopbuttoncomponent.get(), dae::MenuComponent::MenuEvent::Coop);

	textCoopbuttonRendercomponent.get()->SetRenderOnScreen(true);
	textCoopbutton.get()->AddComponent(std::move(textCoopbuttoncomponent));
	textCoopbutton.get()->AddComponent(std::move(textCoopbuttonRendercomponent));
	textCoopbutton->SetPosition(512, 350);

	//Versus BUTTON
	auto textVersusbutton = std::make_unique<dae::GameObject>();
	auto textVersusbuttoncomponent = std::make_unique<dae::TextComponent>(textVersusbutton.get(), "Versus", font);
	auto textVersusbuttonRendercomponent = std::make_unique<dae::RenderComponent>(textVersusbutton.get());

	menucomponent->AddButton(textVersusbuttoncomponent.get(), dae::MenuComponent::MenuEvent::Versus);

	textVersusbuttonRendercomponent.get()->SetRenderOnScreen(true);
	textVersusbutton.get()->AddComponent(std::move(textVersusbuttoncomponent));
	textVersusbutton.get()->AddComponent(std::move(textVersusbuttonRendercomponent));
	textVersusbutton->SetPosition(512, 400);

	//QUIT BUTTON
	auto textQuitbutton = std::make_unique<dae::GameObject>();
	auto textQuitbuttoncomponent = std::make_unique<dae::TextComponent>(textQuitbutton.get(), "Quit", font);
	auto textQuitbuttonRendercomponent = std::make_unique<dae::RenderComponent>(textQuitbutton.get());

	menucomponent->AddButton(textQuitbuttoncomponent.get(), dae::MenuComponent::MenuEvent::Quit);

	textQuitbuttonRendercomponent.get()->SetRenderOnScreen(true);
	textQuitbutton.get()->AddComponent(std::move(textQuitbuttoncomponent));
	textQuitbutton.get()->AddComponent(std::move(textQuitbuttonRendercomponent));
	textQuitbutton->SetPosition(512, 450);

	menucomponent->MoveSelected(0);

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Up, std::make_unique<dae::MoveMenuCommand>(menucomponent), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Up, std::make_unique<dae::MoveMenuCommand>(menucomponent), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, dae::KeyState::Up, std::make_unique<dae::ExecuteMenuCommand>(menucomponent), nullptr);
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_F1, dae::KeyState::Up, std::make_unique<dae::ToggleMuteCommand>(), nullptr);

	scene.Add(std::move(menuGameObject));
	scene.Add(std::move(textSolobutton));
	scene.Add(std::move(textCoopbutton));
	scene.Add(std::move(textVersusbutton));
	scene.Add(std::move(textQuitbutton));

	//ScoreBoard
	auto scoreboardTitleObject = std::make_unique<dae::GameObject>();
	auto scoreboardTitleTextComponent = std::make_unique<dae::TextComponent>(scoreboardTitleObject.get(), "ScoreBoard:",font);
	auto scoreboardTitleRenderComponent = std::make_unique<dae::RenderComponent>(scoreboardTitleObject.get());

	scoreboardTitleRenderComponent.get()->SetRenderOnScreen(true);
	scoreboardTitleObject.get()->AddComponent(std::move(scoreboardTitleTextComponent));
	scoreboardTitleObject.get()->AddComponent(std::move(scoreboardTitleRenderComponent));
	scoreboardTitleObject.get()->AddComponent(std::make_unique<dae::ScoreBoardDisplayComponent>());
	auto scoreboardDisplay = scoreboardTitleObject.get()->GetComponentByType<dae::ScoreBoardDisplayComponent>();
	scoreboardTitleObject->SetPosition(850.f, 100.f);
	scene.Add(std::move(scoreboardTitleObject));


	auto textfont = dae::ResourceManager::GetInstance().LoadFont("Pixel.otf", 24);
	for (int index{}; index<7; index++) 
	{
		auto scoreboardTextObject = std::make_unique<dae::GameObject>();
		auto scoreboardTextComponent = std::make_unique<dae::TextComponent>(scoreboardTextObject.get(), "-", textfont);
		auto scoreboardRenderComponent = std::make_unique<dae::RenderComponent>(scoreboardTextObject.get());

		scoreboardDisplay->AddText(scoreboardTextComponent.get());

		scoreboardRenderComponent.get()->SetRenderOnScreen(true);
		scoreboardTextObject.get()->AddComponent(std::move(scoreboardTextComponent));
		scoreboardTextObject.get()->AddComponent(std::move(scoreboardRenderComponent));
		scoreboardTextObject->SetPosition(850.f, 150.f+float(index*50.f));
		scene.Add(std::move(scoreboardTextObject));
		
	}

	
	scoreboardDisplay->SetDataPath(data_location);
	scoreboardDisplay->RefreshScore("scores.csv");


	scene.Start();
	//SOUND



	auto fullPath = data_location / "BombExplodes.wav";
	auto filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::register_sound_system(std::make_unique<dae::SDLSoundSystem>());
	dae::SoundSystemLocator::get_sound_system().LoadSound(0, filename);
	fullPath = data_location / "Music.mp3";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(1, filename);
	fullPath = data_location / "StepX.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(2, filename);
	fullPath = data_location / "StepY.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(3, filename);
	fullPath = data_location / "Powerup.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(4, filename);
	fullPath = data_location / "BombLay.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(5, filename);
	fullPath = data_location / "BombermanKilled.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(6, filename);
	dae::SoundSystemLocator::get_sound_system().PlaySound(1);


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
