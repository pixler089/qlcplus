
#include "commandfixturedelete.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"

using namespace Command;

CommandFixtureDelete::CommandFixtureDelete()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Fixture)
{
}

CommandFixtureDelete::~CommandFixtureDelete()
{
}

void CommandFixtureDelete::execute()
{
	if (m_fixtureInternalId<=0)
		return;
	ObjectManager::instance().fixtureRemove(m_fixtureInternalId);
}
