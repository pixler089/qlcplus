#include "commandfixturecreate.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"

using namespace Command;

CommandFixtureCreate::CommandFixtureCreate()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Universe)
{
}

CommandFixtureCreate::~CommandFixtureCreate()
{
}

void CommandFixtureCreate::execute()
{
	for (std::shared_ptr<Fixture> newFixture : m_fixtures)
	{
		ObjectManager::instance().fixturePublish(newFixture);
	}
	m_fixtures.clear();
}
