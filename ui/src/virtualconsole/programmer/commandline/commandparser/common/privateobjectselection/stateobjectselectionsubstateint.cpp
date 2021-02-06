#include "stateobjectselectionsubstateint.h"

#include "virtualconsole/programmer/commandline/commandparser/commandtext.h"

using namespace CommandParser;

StateObjectSelectionSubstateInt::StateObjectSelectionSubstateInt(bool singleSelect)
	: StateObjectSelectionSubstate(singleSelect)
{
}

StateObjectSelectionSubstateInt::~StateObjectSelectionSubstateInt()
{
}


void StateObjectSelectionSubstateInt::clear()
{
	m_buffer.clear();
	m_objectList.clear();
}

void StateObjectSelectionSubstateInt::finish(CommandText& formattedCommandText)
{
	m_objectList.clear();
	bool isGroup=false;
	if (m_buffer.size()>0 && m_buffer[0]=='0' && !isSingleSelect())
	{
		formattedCommandText.appendRawCommandPart(m_buffer[0]);
		m_buffer.erase(0,1);
		formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"saddleBrown\">Group</font>"), ">");
		isGroup=true;
	}
	for(char itChar : m_buffer )
	{
		formattedCommandText.appendRawCommandPart(itChar);
		formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"saddleBrown\">")+itChar+"</font>", ">");
	}
	if (isGroup)
	{
		//TODO
	}
	else
	{
		try
		{
			m_objectList.push_back(std::stoi(m_buffer));
		}
		catch (...)
		{
			formattedCommandText.addErrorMessage("Internal error: Not a valid Object ID: "+m_buffer);
		}
	}
}

bool StateObjectSelectionSubstateInt::parseChar(char newChar, CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
	if (newChar>='0' && newChar <='9')
	{
		m_buffer+=newChar;
		return true;
	}
	return false;
}

std::shared_ptr<State> StateObjectSelectionSubstateInt::getFollowingParserState()
{
	return nullptr;
}

void StateObjectSelectionSubstateInt::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	std::ignore=hintMessage;
	helpMessage=">";
}
