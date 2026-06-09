#pragma once
#include "ObjectComponent.h"
namespace dae
{

	class TextComponent;
	class GameSceneLoaderComponent : public ObjectComponent
	{
	public:
		struct SceneDetails
		{
			int randomSeed;
			int enemyCount;
			int playersAmount;
			bool isVersus;
		};



		virtual void Start() override;
		virtual void Update() override {}
		virtual void Render() const override {}

		void LoadScene(SceneDetails details);


		GameSceneLoaderComponent();
		~GameSceneLoaderComponent() = default;
		GameSceneLoaderComponent(const GameSceneLoaderComponent& other) = delete;
		GameSceneLoaderComponent(GameSceneLoaderComponent&& other) = delete;
		GameSceneLoaderComponent& operator=(const GameSceneLoaderComponent& other) = delete;
		GameSceneLoaderComponent& operator=(GameSceneLoaderComponent&& other) = delete;

	private:

	};


}