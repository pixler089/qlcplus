#ifndef COMMAND_H
#define COMMAND_H

#include "virtualconsole/programmer/commandline/objects/object.h"

#include <string>
#include <list>

namespace Command
{

class ICommandGui;

class CommandBase
{
public:
	enum class EExecuteType
	{
		GUI,
		Backend
	};
	CommandBase(EExecuteType executeType, Object::Type objectType);
	~CommandBase();
	///@brief executes the command
	virtual void execute() = 0;
	///@brief 
	virtual EExecuteType getExecuteType() { return m_executeType; };
	void setWorkspaceId(int id) {m_workspaceID=id;};
	void setCommandGuiInterface(ICommandGui* commandGuiInterface){m_commandGuiInterface=commandGuiInterface;};
protected:
	ICommandGui* getCommandGui() const { return m_commandGuiInterface; };
private:
	Object::Type m_objectType;
	EExecuteType m_executeType;
	int m_workspaceID=-1;
	ICommandGui* m_commandGuiInterface=0;
};

};

#endif
