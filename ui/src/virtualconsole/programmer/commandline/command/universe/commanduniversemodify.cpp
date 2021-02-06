
#include "commanduniversemodify.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"
#include "virtualconsole/programmer/commandline/objects/universe/universe.h"

using namespace Command;

CommandUniverseModify::CommandUniverseModify()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Universe)
{
}

CommandUniverseModify::~CommandUniverseModify()
{
}

void CommandUniverseModify::execute()
{
	if (!m_universe)
		return;
	ObjectManager::instance().universeUpdate(m_internalId, m_universe);
	m_universe.reset();
}
