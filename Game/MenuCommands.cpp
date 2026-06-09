#include "MenuCommands.h"
#include "MenuComponent.h"
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
