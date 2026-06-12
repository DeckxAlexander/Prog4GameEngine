#include "ObjectComponent.h"
#include <SDL3/SDL.h> 
#include <vector>

namespace dae
{

	class TextComponent;
	class MenuComponent : public ObjectComponent
	{
	public:
		enum class MenuEvent
		{
			Solo,
			Coop,
			Versus,
			Quit
		};

		

		virtual void Update() override {}
		virtual void Render() const override {}

		void AddButton(TextComponent* text, MenuEvent event);
		void RemoveButton(size_t index);
		void MoveSelected(size_t index);

		void ExecuteSelected();

		MenuComponent();	
		~MenuComponent() = default;
		MenuComponent(const MenuComponent& other) = delete;
		MenuComponent(MenuComponent&& other) = delete;
		MenuComponent& operator=(const MenuComponent& other) = delete;
		MenuComponent& operator=(MenuComponent&& other) = delete;

	private:
		struct Button
		{
			TextComponent* text;
			MenuEvent event;
		};

		SDL_Color m_SelectedColor{ 255, 255, 0, 255 };
		SDL_Color m_NotSelectedColor{ 255, 255, 255, 255 };
		size_t m_SelectedIndex{};

		std::vector<Button> m_Buttons{};

		
	};


}