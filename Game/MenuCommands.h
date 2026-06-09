#include "Commands.h"
namespace dae
{
	class MenuComponent;
	class MenuCommand : public Command
	{
	protected:
		MenuComponent* m_MenuComponent;
	public:

		MenuCommand(MenuComponent* moveComponent);

		void Execute(CommandValue* value) override = 0;

	};

	class MoveMenuCommand : public MenuCommand
	{
	public:

		MoveMenuCommand(MenuComponent* moveComponent);

		void Execute(CommandValue* value) override;

	};

	class ExecuteMenuCommand : public MenuCommand
	{
	public:

		ExecuteMenuCommand(MenuComponent* moveComponent);

		void Execute(CommandValue* value) override;

	};

}
