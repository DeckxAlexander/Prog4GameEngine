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
	menuGameObject->AddComponent(std::make_unique<dae::MenuComponent>());
	auto menucomponent = menuGameObject->GetComponentByType<dae::MenuComponent>();

	//SOLO BUTTON
	auto textSolobutton = std::make_unique<dae::GameObject>();
	auto textSolobuttoncomponent = std::make_unique<dae::TextComponent>(textSolobutton.get(), "Solo", font);
	auto textSolobuttonRendercomponent = std::make_unique<dae::RenderComponent>(textSolobutton.get());

	menucomponent->AddButton(textSolobuttoncomponent.get(), dae::MenuComponent::MenuEvent::Solo);

	textSolobuttonRendercomponent.get()->SetRenderOnScreen(true);
	textSolobutton.get()->AddComponent(std::move(textSolobuttoncomponent));
	textSolobutton.get()->AddComponent(std::move(textSolobuttonRendercomponent));
	textSolobutton->SetPosition(512, 200);

	//COOP BUTTON
	auto textCoopbutton = std::make_unique<dae::GameObject>();
	auto textCoopbuttoncomponent = std::make_unique<dae::TextComponent>(textCoopbutton.get(), "Coop", font);
	auto textCoopbuttonRendercomponent = std::make_unique<dae::RenderComponent>(textCoopbutton.get());

	menucomponent->AddButton(textCoopbuttoncomponent.get(), dae::MenuComponent::MenuEvent::Coop);

	textCoopbuttonRendercomponent.get()->SetRenderOnScreen(true);
	textCoopbutton.get()->AddComponent(std::move(textCoopbuttoncomponent));
	textCoopbutton.get()->AddComponent(std::move(textCoopbuttonRendercomponent));
	textCoopbutton->SetPosition(512, 250);

	//Versus BUTTON
	auto textVersusbutton = std::make_unique<dae::GameObject>();
	auto textVersusbuttoncomponent = std::make_unique<dae::TextComponent>(textVersusbutton.get(), "Versus", font);
	auto textVersusbuttonRendercomponent = std::make_unique<dae::RenderComponent>(textVersusbutton.get());

	menucomponent->AddButton(textVersusbuttoncomponent.get(), dae::MenuComponent::MenuEvent::Versus);

	textVersusbuttonRendercomponent.get()->SetRenderOnScreen(true);
	textVersusbutton.get()->AddComponent(std::move(textVersusbuttoncomponent));
	textVersusbutton.get()->AddComponent(std::move(textVersusbuttonRendercomponent));
	textVersusbutton->SetPosition(512, 300);

	//QUIT BUTTON
	auto textQuitbutton = std::make_unique<dae::GameObject>();
	auto textQuitbuttoncomponent = std::make_unique<dae::TextComponent>(textQuitbutton.get(), "Quit", font);
	auto textQuitbuttonRendercomponent = std::make_unique<dae::RenderComponent>(textQuitbutton.get());

	menucomponent->AddButton(textQuitbuttoncomponent.get(), dae::MenuComponent::MenuEvent::Quit);

	textQuitbuttonRendercomponent.get()->SetRenderOnScreen(true);
	textQuitbutton.get()->AddComponent(std::move(textQuitbuttoncomponent));
	textQuitbutton.get()->AddComponent(std::move(textQuitbuttonRendercomponent));
	textQuitbutton->SetPosition(512, 350);





	menucomponent->MoveSelected(0);

	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_W, dae::KeyState::Up, std::make_unique<dae::MoveMenuCommand>(menucomponent), std::make_unique<dae::CommandValue>(glm::vec2{ -1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_S, dae::KeyState::Up, std::make_unique<dae::MoveMenuCommand>(menucomponent), std::make_unique<dae::CommandValue>(glm::vec2{ 1.f, 0.f }));
	dae::InputManager::GetInstance().BindCommand(SDL_SCANCODE_SPACE, dae::KeyState::Up, std::make_unique<dae::ExecuteMenuCommand>(menucomponent), nullptr);

	scene.Add(std::move(menuGameObject));
	scene.Add(std::move(textSolobutton));
	scene.Add(std::move(textCoopbutton));
	scene.Add(std::move(textVersusbutton));
	scene.Add(std::move(textQuitbutton));

	scene.Start();
	//SOUND



	const auto fullPath = data_location / "BombExplodes.wav";
	const auto filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::register_sound_system(std::make_unique<dae::SDLSoundSystem>());
	dae::SoundSystemLocator::get_sound_system().LoadSound(0, filename);

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
