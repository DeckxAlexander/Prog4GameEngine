#include "Commands.h"
namespace dae
{
	class MenuComponent;
	class ScoreBoardComponent;
	class MenuCommand : public Command
	{
	protected:
		MenuComponent* m_MenuComponent;
	public:

		MenuCommand(MenuComponent* moveComponent);

		void Execute(CommandValue* value) override = 0;

	};

	class ScoreBoardCommand : public Command
	{
	protected:
		ScoreBoardComponent* m_ScoreBoardComponent;
	public:

		ScoreBoardCommand(ScoreBoardComponent* scoreboardComponent);

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

	class MoveScoreBoardCommand : public ScoreBoardCommand
	{
	public:

		MoveScoreBoardCommand(ScoreBoardComponent* scoreboardComponent);

		void Execute(CommandValue* value) override;

	};

	class ExecuteScoreBoardCommand : public ScoreBoardCommand
	{
	public:

		ExecuteScoreBoardCommand(ScoreBoardComponent* scoreboardComponent);

		void Execute(CommandValue* value) override;

	};
}
