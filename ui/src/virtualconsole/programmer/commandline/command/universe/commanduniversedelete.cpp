
#include "commanduniversedelete.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"

using namespace Command;

CommandUniverseDelete::CommandUniverseDelete()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Universe)
{
}

CommandUniverseDelete::~CommandUniverseDelete()
{
}

void CommandUniverseDelete::execute()
{
	if (m_universeInternalId<=0)
		return;
	ObjectManager::instance().universeRemove(m_universeInternalId);
}
