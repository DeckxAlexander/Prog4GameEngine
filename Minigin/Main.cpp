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

	auto pivot = std::make_unique<dae::GameObject>();
	pivot.get()->SetPosition(250, 100);

	go = std::make_unique<dae::GameObject>();
	gor = std::make_unique<dae::RenderComponent>(go.get(), "Bomb.png");
	auto gom = std::make_unique<dae::MovementComponent>(go.get(), 5.f, 25.f);
	go.get()->AddComponent(std::move(gor));
	go.get()->AddComponent(std::move(gom));
	go.get()->SetParent(pivot.get());

	

	auto child = std::make_unique<dae::GameObject>();
	gor = std::make_unique<dae::RenderComponent>(child.get(), "Bomberman.png");
	gom = std::make_unique<dae::MovementComponent>(child.get(), -10.f, 50.f);
	child.get()->AddComponent(std::move(gor));
	child.get()->AddComponent(std::move(gom));
	child.get()->SetParent(go.get());

	scene.Add(std::move(pivot));

	scene.Add(std::move(go));
	scene.Add(std::move(child));

	auto Menu = std::make_unique<dae::GameObject>();
	auto thr = std::make_unique<dae::ThrashCacheComponent>(Menu.get());
	Menu.get()->AddComponent(std::move(thr));
	scene.Add(std::move(Menu));
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
