#include "commandworkspaceselect.h"

#include "../icommandgui.h"

#include <iostream>

using namespace Command;

CommandWorkspaceSelect::CommandWorkspaceSelect()
	: CommandBase(CommandBase::EExecuteType::GUI, Object::Type::Workspace)
{
}

CommandWorkspaceSelect::~CommandWorkspaceSelect()
{
}

void CommandWorkspaceSelect::execute()
{
	if (getCommandGui())
	{
		getCommandGui()->setCurrentWorkspace(m_workspaceID);
	}
}
