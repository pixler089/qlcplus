#ifndef COMMAND_WORKSPACESELECT_H
#define COMMAND_WORKSPACESELECT_H

#include "../command.h"

#include <iostream>

namespace Command
{

class CommandWorkspaceSelect : public CommandBase
{
public:
	CommandWorkspaceSelect();
	virtual ~CommandWorkspaceSelect();
	void setWorkspaceToSelect(int id) { m_workspaceID=id; };
	///@brief executes the command
	virtual void execute() override;
private:
	int m_workspaceID=-1;
};

}

#endif
