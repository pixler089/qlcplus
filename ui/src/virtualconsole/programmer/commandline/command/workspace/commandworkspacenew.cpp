#include "commandworkspacenew.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"
#include "virtualconsole/programmer/commandline/objects/workspace/workspace.h"

using namespace Command;

CommandWorkspaceNew::CommandWorkspaceNew(int id, std::string name)
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Workspace),
	  m_id(id),
	  m_name(name)
{
}

CommandWorkspaceNew::~CommandWorkspaceNew()
{
}

void CommandWorkspaceNew::execute()
{
	if (ObjectManager::instance().workspaceGetByExternalId(m_id).expired())
	{
		std::shared_ptr<Workspace> workspace(new Workspace());
		workspace->setExternalId(m_id);
		workspace->setTitle(m_name);
		ObjectManager::instance().workspacePublish(workspace);
	}
}

