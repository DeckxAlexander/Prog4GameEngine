#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SDLSoundSystem.h"
#include "GameSceneLoaderComponent.h"
#include <filesystem>


namespace fs = std::filesystem;

static void load()
{
	fs::path data_location = "./Data/";
	if (!fs::exists(data_location))
		data_location = "../Data/";

	dae::GameSceneLoader::GetInstance().SetDataPath(data_location);
	dae::GameSceneLoader::GetInstance().LoadMainMenu();
	
	//SOUND
	auto fullPath = data_location / "Sounds/BombExplodes.wav";
	auto filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::register_sound_system(std::make_unique<dae::SDLSoundSystem>());
	dae::SoundSystemLocator::get_sound_system().LoadSound(0, filename);
	fullPath = data_location / "Sounds/Music.mp3";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(1, filename);
	fullPath = data_location / "Sounds/StepX.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(2, filename);
	fullPath = data_location / "Sounds/StepY.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(3, filename);
	fullPath = data_location / "Sounds/Powerup.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(4, filename);
	fullPath = data_location / "Sounds/BombLay.wav";
	filename = fs::path(fullPath).string();
	dae::SoundSystemLocator::get_sound_system().LoadSound(5, filename);
	fullPath = data_location / "Sounds/BombermanKilled.wav";
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
