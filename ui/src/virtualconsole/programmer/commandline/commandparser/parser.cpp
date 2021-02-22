#include "parser.h"

//TODO relativen Pfad beseitigen
#include "commandtext.h"
#include "state.h"
#include "common/stateinitial.h"
#include <iostream>

namespace CommandParser
{

Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::clear() 
{ 
	m_commands.clear();
	m_stateStack.clear();
}

bool Parser::parseCommand(std::string command, CommandText& formattedCommandText)
{
	bool hasError=false;
	clear();
	std::shared_ptr<StateInitial> parserStateInitial(new StateInitial);
	std::shared_ptr<State> parserState=parserStateInitial;
	for (char commandChar : command)
	{
		bool hasParsed=false;
		//To avoid infinite loops we limit the next loop
		int commandParserChangeLimitation=50;
		while (commandParserChangeLimitation)
		{
			--commandParserChangeLimitation;
			if (!parserState)
			{
				if (m_stateStack.empty())
					break;
				parserState=m_stateStack.back();
				m_stateStack.pop_back();
				parserState->returnAfterStacked(formattedCommandText);
				continue;
			}

			if (parserState->parseChar(commandChar, formattedCommandText))
			{
				hasParsed=true;
				break;
			}
			if (parserState->getFollowingParserState())//&&parserState->wantsBeStacked())
				m_stateStack.push_back(parserState);
			else
				parserState->finish(formattedCommandText);
			parserState=parserState->getFollowingParserState();
		}
		if (!hasParsed)
		{
			hasError=true;
			std::string commandCharString;
			commandCharString=commandChar;
			formattedCommandText.appendRawCommandPart(commandCharString);
			formattedCommandText.appendHtmlCommandPart(HtmlString("<font color=\"red\">"+commandCharString+"</font>"));
		}

		if (parserState && parserState->getFollowingParserState())
		{
			if (true)//parserState->wantsBeStacked())
				m_stateStack.push_back(parserState);
			else
				parserState->finish(formattedCommandText);
			parserState=parserState->getFollowingParserState();
		}
	}

	if (parserState)
	{
		parserState->finish(formattedCommandText);
		std::string helpMessage;
		std::string hintMessage;
		parserState->getHelpHintMessages(helpMessage, hintMessage);
		formattedCommandText.setHelpMessageLastCommand(helpMessage);
		if (!hasError)
			formattedCommandText.setHintMessageLastCommand(hintMessage);
		else
			formattedCommandText.setHintMessageLastCommand("");
	}
	while (!m_stateStack.empty())
	{
		auto parserState=m_stateStack.back();
		m_stateStack.pop_back();
		parserState->returnAfterStacked(formattedCommandText);
		parserState->finish(formattedCommandText);
		if (formattedCommandText.getHelpMessageLastCommand()==">")
		{
			std::string helpMessage;
			std::string hintMessage;
			parserState->getHelpHintMessages(helpMessage, hintMessage);
			formattedCommandText.setHelpMessageLastCommand(helpMessage);
		}
	}
	if (!hasError)
	{
		auto cmd=parserStateInitial->getResultingCommand();
		if (cmd.size())
		{
			std::move(cmd.begin(), cmd.end(), std::back_inserter(m_commands));
			m_commands.splice(m_commands.end(), cmd);
		}
	}
	return true;
}

std::string Parser::getErrorMessage() const
{
	return std::string();
}

}

