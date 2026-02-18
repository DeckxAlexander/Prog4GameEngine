#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "Scene.h"
#include "FPSCounterComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	auto go = std::make_unique<dae::GameObject>();
	auto gor = std::make_unique<dae::RenderComponent>("background.png");
	go.get()->AddComponent(std::move(gor));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	gor = std::make_unique<dae::RenderComponent>("logo.png");
	go.get()->AddComponent(std::move(gor));

	go->SetPosition(358, 180);
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_unique<dae::GameObject>();
	auto tc = std::make_unique<dae::TextComponent>("Dae Assignment Alexander", font);
	tc->SetColor({ 255, 255, 0, 255 });
	tc->SetPosition(292, 20);
	to.get()->AddComponent(std::move(tc));
	scene.Add(std::move(to));

	auto fpso = std::make_unique<dae::GameObject>();
	auto fpsc = std::make_unique<dae::FPSCounterComponent>("FPS", font);
	fpsc->SetColor({ 255, 255, 0, 255 });
	fpsc->SetPosition(50, 20);
	fpso.get()->AddComponent(std::move(fpsc));
	


	scene.Add(std::move(fpso));


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
