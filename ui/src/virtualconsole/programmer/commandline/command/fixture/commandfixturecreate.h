#ifndef COMMANDFIXTURECREATE_H
#define COMMANDFIXTURECREATE_H

#include "../command.h"

#include <memory>
#include <list>

class Fixture;

namespace Command
{

class CommandFixtureCreate : public CommandBase
{
public:
	CommandFixtureCreate();
	virtual ~CommandFixtureCreate();
	virtual void execute() override;
	void setFixtures(std::list<std::shared_ptr<Fixture>> fixtures) {m_fixtures=fixtures;};
private:
	std::list<std::shared_ptr<Fixture>> m_fixtures;
};

}

#endif
