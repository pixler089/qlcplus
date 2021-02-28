#include "stateclearmain.h"

#include "../commandtext.h"
#include "../common/stateobjectselection.h"
#include "../../command/icommandgui.h"

#include <iostream>
#include <memory>

using namespace CommandParser;

static const std::string helpCommands="\
	<table> \
		<tr><td><font color=\"red\">0</font>: <font color=\"red\">a</font>ll</td></tr> \
		<tr><td><font color=\"red\">1</font>: <font color=\"red\">s</font>elected</td></tr> \
	</table> \
";

StateClearMain::StateClearMain()
{
}

StateClearMain::~StateClearMain()
{
}

void StateClearMain::clear()
{
	m_commandType=CommandType::NoCommand;
}

void StateClearMain::finish(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
}

bool StateClearMain::parseChar(char newChar, CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
	std::string longText;
	bool hasError=false;
	switch (newChar)
	{
		case 'a':
		case '0':
			longText="all";
			m_commandType=CommandType::All;
			break;
		case 's':
		case '1':
			longText="selected";
			m_commandType=CommandType::Selected;
			break;
		default:
			hasError=true;
			longText=newChar;
			formattedCommandText.addErrorMessage(std::string("Error: Could not find command group ")+longText);
	}
	std::string shortCommand;
	shortCommand=newChar;
	std::string textColor;
	if (hasError)
		textColor="red";
	else
		textColor="blue";
	formattedCommandText.appendRawCommandPart(shortCommand);
	formattedCommandText.appendHtmlCommandPart(std::string("<font color=\""+textColor+"\">"+longText+"</font> "), helpCommands);
	return !hasError;
}

std::shared_ptr<State> StateClearMain::getFollowingParserState()
{
	return nullptr;
}

Command::CommandBase::List StateClearMain::getResultingCommand() const
{
	//First we handle commands that our State has to create (there don't exist substates)
	if (m_commandType==CommandType::All)
	{
		std::shared_ptr<Command::CommandBase> commandReturn(new Command::CommandBase(Command::CommandBase::EExecuteType::GUI, Object::Type::Fixture));	
		auto func=std::function<void(Command::ICommandGui*)>(&Command::ICommandGui::commandClearAll);
		commandReturn->setCommand(func);
		return Command::CommandBase::List{commandReturn};
	}
	if (m_commandType==CommandType::Selected)
	{
		std::shared_ptr<Command::CommandBase> commandReturn(new Command::CommandBase(Command::CommandBase::EExecuteType::GUI, Object::Type::Fixture));	
		auto func=std::function<void(Command::ICommandGui*)>(&Command::ICommandGui::commandClearSelected);
		commandReturn->setCommand(func);
		return Command::CommandBase::List{commandReturn};
	}
	return Command::CommandBase::List();
}

void StateClearMain::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	helpMessage=helpCommands;
	hintMessage="&lt;Subcommand&gt;";
}
