#pragma once
#include "ObjectComponent.h"
#include "Singleton.h"
#include <filesystem>
namespace dae
{
	class GameSceneLoader : public Singleton<GameSceneLoader>
	{
	public:
		struct SceneDetails
		{
			int randomSeed;
			int width;
			int height;
			int playersAmount;
			bool isVersus;

			int blocksCount;
			int balloomCount;
			int onealCount;
			int dollCount;
			int minvoCount;
			
		};

		void LoadLevelFromFile(std::string filename, int playersAmount, bool isVersus = false);
		void SetDataPath(const std::filesystem::path& filePath) 
		{
			m_dataPath = filePath;
		}

		GameSceneLoader() = default;
		~GameSceneLoader() = default;
		GameSceneLoader(const GameSceneLoader& other) = delete;
		GameSceneLoader(GameSceneLoader&& other) = delete;
		GameSceneLoader& operator=(const GameSceneLoader& other) = delete;
		GameSceneLoader& operator=(GameSceneLoader&& other) = delete;

	private:
		void SetupScene(SceneDetails details);
		std::filesystem::path m_dataPath{};
		friend class Singleton<GameSceneLoader>;
	};


}