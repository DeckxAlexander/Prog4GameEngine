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


#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();


	auto tileGameObject = std::make_unique<dae::GameObject>();
	auto tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "background.png");
	tileGameObject.get()->AddComponent(std::move(tileRenderComponent));
	scene.Add(std::move(tileGameObject));


	auto GridManager = std::make_unique<dae::GameObject>();
	auto GridComp = std::make_unique<dae::GridComponent>(GridManager.get(), 32, 18);
	dae::GridComponent* grid = GridComp.get();
	GridComp->SetTileScale(32.f, 32.f);
	GridManager.get()->AddComponent(std::move(GridComp));
	GridManager.get()->SetPosition(-0.3f, -0.3f);
	scene.Add(std::move(GridManager));

	//Create Grid
	for (int indexX{}; indexX < 32; indexX++) 
	{
		for (int indexY{}; indexY < 18; indexY++) 
		{
			tileGameObject = std::make_unique<dae::GameObject>(true, grid);
			if (indexX == 0 || indexX == 31 || indexY == 0 || indexY == 17) 
			{
				tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "HardStoneTile.png");
			}
			else 
			{
				tileRenderComponent = std::make_unique<dae::RenderComponent>(tileGameObject.get(), "GreenTile.png");
			}

			
			tileGameObject.get()->AddComponent(std::move(tileRenderComponent));
			tileGameObject->SetScale(2.f, 2.f);
			dynamic_cast<dae::GridTransform*>(tileGameObject->GetTransform())->SetGridTile(indexX, indexY);
			scene.Add(std::move(tileGameObject));
		}

	}


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
