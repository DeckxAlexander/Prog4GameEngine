#include "ObjectComponent.h"
#include <SDL3/SDL.h> 
#include <filesystem>
#include <vector>

namespace dae
{

	class TextComponent;
	class ScoreBoardComponent : public ObjectComponent
	{
	public:

		virtual void Update() override {}

		void AddButton(TextComponent* text, bool confirm = false);
		void RemoveButton(size_t index);
		void MoveSelected(size_t index);

		void LetterChangeSelected(size_t index);
		void ExecuteSelected();

		void AddScore(const std::string& filename, const std::string& playerName, int newScore);
		void SetDataPath(const std::filesystem::path& filePath)
		{
			m_dataPath = filePath;
		}

		ScoreBoardComponent();
		~ScoreBoardComponent() = default;
		ScoreBoardComponent(const ScoreBoardComponent& other) = delete;
		ScoreBoardComponent(ScoreBoardComponent&& other) = delete;
		ScoreBoardComponent& operator=(const ScoreBoardComponent& other) = delete;
		ScoreBoardComponent& operator=(ScoreBoardComponent&& other) = delete;

	private:

		struct Button 
		{
			TextComponent* text;
			bool isConfirm;
			char letter;
			
		};

		SDL_Color m_SelectedColor{ 255, 255, 0, 255 };
		SDL_Color m_NotSelectedColor{ 255, 255, 255, 255 };
		size_t m_SelectedIndex{};
		std::filesystem::path m_dataPath{};

		std::vector<Button> m_Buttons{};


	};


}