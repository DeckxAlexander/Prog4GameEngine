#include "ObjectComponent.h"
#include "SceneManager.h"
#include "Singleton.h"
#include <filesystem>
namespace dae
{
	class GridComponent;
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

		bool DoesSceneExist(const std::string& filename);
		void LoadLevelFromFile(const std::string& filename, int playersAmount, bool isVersus = false);
		void LoadScoreBoard();
		void LoadMainMenu();
		void OpenMainMenu();
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
		void SpawnPlayers(int amount);

		void SpawnVersusPlayer(int x, int y, GridComponent* grid, Scene& scene);

		void SpawnEnemies(SceneDetails details);


		void SpawnBalloom(int x, int y, GridComponent* grid, Scene& scene);
		void SpawnOneal(int x, int y, GridComponent* grid, Scene& scene);
		void SpawnDoll(int x, int y, GridComponent* grid, Scene& scene);
		void SpawnMinvo(int x, int y, GridComponent* grid, Scene& scene);

		std::filesystem::path m_dataPath{};
		friend class Singleton<GameSceneLoader>;
	};


}