#ifndef COMMANDUNIVERSEDELETE_H
#define COMMANDUNIVERSEDELETE_H

#include "../command.h"

#include <memory>

class Universe;

namespace Command
{

class CommandUniverseDelete : public CommandBase
{
public:
	CommandUniverseDelete();
	virtual ~CommandUniverseDelete();
	virtual void execute() override;
	void setUniverseInternalId(int internalId) { m_universeInternalId=internalId;};
private:
	int m_universeInternalId=-1;
};

}

#endif
