#include "stateworkspacenew.h"

#include "../commandtext.h"
#include "virtualconsole/programmer/commandline/command/command.h"
#include "virtualconsole/programmer/commandline/command/workspace/commandworkspacenew.h"

using namespace CommandParser;

static const std::string helpCommands="Bitte eine freie ID sowie einen menschenlesbaren Namen eingeben.";

StateWorkspaceNew::StateWorkspaceNew()
{
}

StateWorkspaceNew::~StateWorkspaceNew()
{
}

void StateWorkspaceNew::clear()
{
	m_strID.clear();
	m_strName.clear();
}

void StateWorkspaceNew::finish(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
}

bool StateWorkspaceNew::parseChar(char newChar, CommandText& formattedCommandText)
{
	bool isOk=false;
	std::string shortCommand;
	shortCommand=newChar;
	std::string longText;
	if (m_internalState==EInternalState::ID)
	{
		if (newChar>='0' && newChar <= '9')
		{
			m_strID+=newChar;
			longText=newChar;
			isOk=true;
		}
		else if (newChar==',')
		{
			m_internalState=EInternalState::Name;
			longText=newChar;
			isOk=true;
		}
	}
	else if (m_internalState==EInternalState::Name)
	{
		if ( (newChar>='0' && newChar <= '9') ||
		     (newChar>='A' && newChar <= 'Z') ||
		     (newChar>='a' && newChar <= 'z')
		   )
		{
			m_strName+=newChar;
			longText=newChar;
			isOk=true;
		}
	}
	if (isOk)
	{
		formattedCommandText.appendRawCommandPart(shortCommand);
		formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">"+longText+"</font>"), helpCommands);
	}

	return isOk;
}

std::shared_ptr<State> StateWorkspaceNew::getFollowingParserState()
{
	return 0;
}

Command::CommandBase::List StateWorkspaceNew::getResultingCommand() const
{
	return Command::CommandBase::List{std::make_shared<Command::CommandWorkspaceNew>(Command::CommandWorkspaceNew(getId(), m_strName))};
}

void StateWorkspaceNew::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	helpMessage=helpCommands;
	if (m_internalState==EInternalState::ID)
	{
		if (m_strID.empty())
			hintMessage="&lt;ID>";
		hintMessage+=",&lt;Name>";
	}
	else
	{
		if (m_strName.empty())
			hintMessage="&lt;Name>";
	}

}

int StateWorkspaceNew::getId() const
{
	try 
	{
		return stoi(m_strID);
	}
	catch(...)
	{
		return 0;
	}
}

