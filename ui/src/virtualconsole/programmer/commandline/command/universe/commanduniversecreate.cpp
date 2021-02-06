#include "commanduniversecreate.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"

using namespace Command;

CommandUniverseCreate::CommandUniverseCreate()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Universe)
{
}

CommandUniverseCreate::~CommandUniverseCreate()
{
}

void CommandUniverseCreate::execute()
{
	if (!m_universe)
		return;
	ObjectManager::instance().universePublish(m_universe);
	m_universe.reset();
}
