#ifndef COMMANDUNIVERSEMODIFY_H
#define COMMANDUNIVERSEMODIFY_H

#include "../command.h"

#include <memory>

class Universe;

namespace Command
{

class CommandUniverseModify : public CommandBase
{
public:
	CommandUniverseModify();
	virtual ~CommandUniverseModify();
	virtual void execute() override;
	void setUniverse(int internalId, std::shared_ptr<Universe> universe) { m_internalId=internalId; m_universe=universe;};
private:
	int m_internalId=-1;
	std::shared_ptr<Universe> m_universe;
};

}

#endif
