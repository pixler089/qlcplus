#ifndef COMMANDFIXTURMODIFY_HE
#define COMMANDFIXTURMODIFY_HE

#include "../command.h"

#include <memory>
#include <list>

class Fixture;

namespace Command
{

class CommandFixtureModify : public CommandBase
{
public:
	CommandFixtureModify();
	virtual ~CommandFixtureModify();
	virtual void execute() override;
	
	void setFixture(int internalId) { m_internalId=internalId; };

	void setExternalId(int newExternalId) { m_externalIdValue=newExternalId; m_externalIdModified=true; }
	void setName(const std::string& newName) { m_nameValue=newName; m_nameModified=true; }
	void setDmxStartAddress(int newStartAddress) { m_dmxStartAddressValue=newStartAddress; m_dmxStartAddressModified=true; }
private:
	int m_internalId=-1;

	bool m_externalIdModified=false;
	int m_externalIdValue=-1;

	bool m_nameModified=false;
	std::string m_nameValue;

	bool m_dmxStartAddressModified=false;
	int m_dmxStartAddressValue=-1;
};

}

#endif
