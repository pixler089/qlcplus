#ifndef COMMAND_COMMANDWORKSPACENEW_H
#define COMMAND_COMMANDWORKSPACENEW_H

#include "../command.h"

namespace Command
{

class CommandWorkspaceNew : public CommandBase
{
public:
	CommandWorkspaceNew(int id, std::string name);
	virtual ~CommandWorkspaceNew();
	///@brief executes the command
	virtual void execute() override;
private:
	int m_id;
	std::string m_name;
};

}

#endif
