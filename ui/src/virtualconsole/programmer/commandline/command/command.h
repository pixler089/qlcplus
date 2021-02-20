#ifndef COMMAND_H
#define COMMAND_H

#include "virtualconsole/programmer/commandline/objects/object.h"

#include <string>
#include <list>
#include <functional>
#include <memory>

namespace Command
{

class ICommandGui;


class CommandBase
{
public:
	using List=std::list<std::shared_ptr<CommandBase>>;
	enum class EExecuteType
	{
		GUI,
		Backend
	};
	CommandBase(EExecuteType executeType, Object::Type objectType);
	~CommandBase();
	///@brief executes the command
	virtual void execute()
	{
		if (m_genericCommand && m_commandGuiInterface)
		{
			m_genericCommand(m_commandGuiInterface);
		}
	}
	///@brief 
	virtual EExecuteType getExecuteType() { return m_executeType; };
	void setCommand(std::function<void(ICommandGui* commandGui)> cmd) {m_genericCommand=cmd;};
	void setWorkspaceId(int id) {m_workspaceID=id;};
	void setCommandGuiInterface(ICommandGui* commandGuiInterface){m_commandGuiInterface=commandGuiInterface;};
protected:
	ICommandGui* getCommandGui() const { return m_commandGuiInterface; };
private:
	std::function<void(ICommandGui* commandGui)> m_genericCommand;
	Object::Type m_objectType;
	EExecuteType m_executeType;
	int m_workspaceID=-1;
	ICommandGui* m_commandGuiInterface=0;
};

};

#endif
