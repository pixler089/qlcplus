#ifndef COMMANDUNIVERSECREATE_H
#define COMMANDUNIVERSECREATE_H

#include "../command.h"

#include <memory>

class Universe;

namespace Command
{

class CommandUniverseCreate : public CommandBase
{
public:
	CommandUniverseCreate();
	virtual ~CommandUniverseCreate();
	virtual void execute() override;
	void setUniverse(std::shared_ptr<Universe> universe) { m_universe=universe;};
private:
	std::shared_ptr<Universe> m_universe;
};

}

#endif
