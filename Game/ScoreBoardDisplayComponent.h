#include "ObjectComponent.h"
#include <SDL3/SDL.h> 
#include <filesystem>
#include <vector>

namespace dae
{

	class TextComponent;
	class ScoreBoardDisplayComponent : public ObjectComponent
	{
	public:

		virtual void Update() override {}
		virtual void Render() const override {}

		void AddText(TextComponent* text);
		void RemoveText(size_t index);

		void RefreshScore(const std::string& filename);
		void SetDataPath(const std::filesystem::path& filePath)
		{
			m_dataPath = filePath;
		}

		ScoreBoardDisplayComponent();
		~ScoreBoardDisplayComponent() = default;
		ScoreBoardDisplayComponent(const ScoreBoardDisplayComponent& other) = delete;
		ScoreBoardDisplayComponent(ScoreBoardDisplayComponent&& other) = delete;
		ScoreBoardDisplayComponent& operator=(const ScoreBoardDisplayComponent& other) = delete;
		ScoreBoardDisplayComponent& operator=(ScoreBoardDisplayComponent&& other) = delete;

	private:
		std::filesystem::path m_dataPath{};

		std::vector<TextComponent*> m_Displays{};


	};
}