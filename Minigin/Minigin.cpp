#include <stdexcept>
#include <sstream>
#include <iostream>
#include <thread>
#include <SDL3_mixer/SDL_mixer.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#if USE_STEAMWORKS
#pragma warning (push)
#pragma warning (disable:4996)
#include <steam_api.h>
#pragma warning (pop)
#endif


#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "SoundSystem.h"

SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
#if WIN32
	std::stringstream ss;
	ss << message << major << "." << minor << "." << patch << "\n";
	OutputDebugString(ss.str().c_str());
#else
	std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
	LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
	int version = SDL_GetVersion();
	LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	// LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
	// version = IMG_Version();
	// LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
	LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
	version = TTF_Version();
	LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

dae::Minigin::Minigin(const std::filesystem::path& dataPath)
{
	PrintSDLVersion();
	
	if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		SDL_Log("Renderer error: %s", SDL_GetError());
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		1024,
		576,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD))
	{
		SDL_Log("Input error: %s", SDL_GetError());
		throw std::runtime_error(std::string("Input Error: ") + SDL_GetError());
	}

	if (!SDL_InitSubSystem(SDL_INIT_AUDIO) )
	{
		std::cout << "SDL Init failed\n";
	}


	
#if USE_STEAMWORKS
	if (!SteamAPI_Init())
		throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));
#endif
	Renderer::GetInstance().Init(g_window);
	ResourceManager::GetInstance().Init(dataPath);

}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
#if USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	SoundSystemLocator::get_sound_system().Destroy();


	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();
	m_LastTime = std::chrono::high_resolution_clock::now();
#ifndef __EMSCRIPTEN__
	while (!m_quit)
		RunOneFrame();


#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void dae::Minigin::RunOneFrame()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	const float deltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
	TimeManager::GetInstance().SetDeltaTime(deltaTime);
	m_LastTime = currentTime;

	m_quit = !InputManager::GetInstance().ProcessInput();
	SceneManager::GetInstance().Update();
	Renderer::GetInstance().Render();
	const auto endTime = std::chrono::high_resolution_clock::now();
	const auto loopDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - currentTime);
	const auto sleepTime = m_MsPerFrame - loopDuration;

	
	if (sleepTime.count()>0.f) std::this_thread::sleep_for(sleepTime);
#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif
	
}
