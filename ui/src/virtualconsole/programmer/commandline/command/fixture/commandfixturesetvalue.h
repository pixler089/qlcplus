#ifndef COMMAND_COMMANDFIXTURESETVALUE_H
#define COMMAND_COMMANDFIXTURESETVALUE_H

#include "virtualconsole/programmer/commandline/helper/channeltype.h"
#include "virtualconsole/programmer/commandline/computation/value/dmxvalue.h"
#include "virtualconsole/programmer/commandline/command/command.h"

#include <list>
#include <map>
#include <chrono>

namespace Command
{

class CommandFixtureSetValue : public CommandBase
{
public:
	CommandFixtureSetValue();
	virtual ~CommandFixtureSetValue();

	void execute() override;

	void setFixturesToChange(std::list<int> fixturesToChange) { m_fixturesToChange=fixturesToChange; };
	void setValuesToSet(std::map<ChannelType, DmxValue> values) { m_valuesToSet=values; };
	void setFade(std::chrono::milliseconds fadeTime) { m_fadeTime=fadeTime; };
private:
	std::list<int> m_fixturesToChange;
	std::map<ChannelType, DmxValue> m_valuesToSet;
	std::chrono::milliseconds m_fadeTime;
};

}//namespace Command

#endif
