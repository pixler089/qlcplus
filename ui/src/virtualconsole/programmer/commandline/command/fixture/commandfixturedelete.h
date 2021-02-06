#ifndef COMMANDFIXTUREDELETE_H
#define COMMANDFIXTUREDELETE_H

#include "../command.h"

#include <memory>


namespace Command
{

class CommandFixtureDelete : public CommandBase
{
public:
	CommandFixtureDelete();
	virtual ~CommandFixtureDelete();
	virtual void execute() override;
	void setFixtureInternalId(int internalId) { m_fixtureInternalId=internalId;};
private:
	int m_fixtureInternalId=-1;
};

}

#endif
