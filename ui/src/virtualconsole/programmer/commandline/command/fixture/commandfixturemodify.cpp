#include "commandfixturemodify.h"

#include "virtualconsole/programmer/commandline/objects/objectmanager.h"
#include "virtualconsole/programmer/commandline/objects/fixture/fixture.h"

using namespace Command;

CommandFixtureModify::CommandFixtureModify()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Universe)
{
}

CommandFixtureModify::~CommandFixtureModify()
{
}

void CommandFixtureModify::execute()
{
	bool bValueChanged=false;
	std::shared_ptr<Fixture> fixture=ObjectManager::instance().fixtureGetByInternalId(m_internalId).lock();
	if (!fixture)
		return;
	if (m_externalIdModified)
	{
		bValueChanged=true;
		m_externalIdModified=false;
		fixture->setExternalId(m_externalIdValue);
	}
	if (m_nameModified)
	{
		bValueChanged=true;
		m_nameModified=false;
		fixture->setTitle(m_nameValue);
	}
	if (m_dmxStartAddressModified)
	{
		bValueChanged=true;
		m_dmxStartAddressModified=false;
		fixture->setDmxStartAddress(m_dmxStartAddressValue);
	}
	if (bValueChanged)
		ObjectManager::instance().fixtureUpdated(m_internalId);
}
