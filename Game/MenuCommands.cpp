#include "MenuCommands.h"
#include "MenuComponent.h"
#include "ScoreBoardComponent.h"
#include "GameObject.h"

dae::MenuCommand::MenuCommand(MenuComponent* moveComponent) : m_MenuComponent{moveComponent}
{
	
}

dae::MoveMenuCommand::MoveMenuCommand(MenuComponent* moveComponent) : MenuCommand(moveComponent)
{
}

void dae::MoveMenuCommand::Execute(CommandValue* value)
{
	if (m_MenuComponent == nullptr)  return;
	
	m_MenuComponent->MoveSelected(static_cast<size_t>(value->Vec2D.x));
	
}

dae::ExecuteMenuCommand::ExecuteMenuCommand(MenuComponent* moveComponent) : MenuCommand(moveComponent)
{}

void dae::ExecuteMenuCommand::Execute(CommandValue*)
{
	if (m_MenuComponent == nullptr)  return;

	m_MenuComponent->ExecuteSelected();
}

dae::ScoreBoardCommand::ScoreBoardCommand(ScoreBoardComponent* scoreboardComponent) : m_ScoreBoardComponent{ scoreboardComponent }
{

}

dae::MoveScoreBoardCommand::MoveScoreBoardCommand(ScoreBoardComponent* scoreboardComponent) : ScoreBoardCommand(scoreboardComponent)
{}

void dae::MoveScoreBoardCommand::Execute(CommandValue * value)
{
	if (m_ScoreBoardComponent == nullptr)  return;

	m_ScoreBoardComponent->MoveSelected(static_cast<size_t>(value->Vec2D.x));
	m_ScoreBoardComponent->LetterChangeSelected(static_cast<size_t>(value->Vec2D.y));
}

dae::ExecuteScoreBoardCommand::ExecuteScoreBoardCommand(ScoreBoardComponent* scoreboardComponent) : ScoreBoardCommand(scoreboardComponent)
{}

void dae::ExecuteScoreBoardCommand::Execute(CommandValue *)
{
	m_ScoreBoardComponent->ExecuteSelected();
}
