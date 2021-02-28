#include "stateinitial.h"

#include "../workspace/stateworkspacemain.h"
#include "../fixture/statefixturemain.h"
#include "../clear/stateclearmain.h"
#include "../commandtext.h"
#include "../../command/icommandgui.h"

#include <iostream>

using namespace CommandParser;

static const std::string helpCommands="\
	<table> \
		<tr><td><font color=\"red\">0</font>: <font color=\"red\">f</font>ixture</td></tr> \
		<tr><td><font color=\"red\">1</font>: <font color=\"red\">c</font>lear programmer</td></tr> \
		<tr><td><font color=\"red\">/</font>: <font color=\"red\">r</font>otate selection</td></tr> \
		<!--<tr><td><font color=\"red\">2</font>: <font color=\"red\">w</font>orkspace</td></tr>--> \
	</table> \
";

StateInitial::StateInitial()
{
}

StateInitial::~StateInitial()
{
}

const std::string StateInitial::getHelpMessage()
{
	return helpCommands;
}


void StateInitial::clear()
{
	m_commandParserCommandGroup.reset();
	m_followingParserState.reset();
	m_objectType=Object::Type::Unknown;
}

void StateInitial::finish(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
}

bool StateInitial::parseChar(char newChar, CommandText& formattedCommandText)
{
	std::string longText;
	bool hasError=false;
	switch (newChar)
	{
//		case 'w':
//		case '2':
//			longText="Workspace";
//			m_objectType=Object::Type::Workspace;
//			m_commandParserCommandGroup.reset(new StateWorkspaceMain());
//			break;
		case 'f':
		case '0':	
			longText="Fixture";
			m_objectType=Object::Type::Fixture;
			m_commandParserCommandGroup.reset(new StateFixtureMain());
			break;
		case 'c':
		case '1':	
			longText="Clear";
			m_objectType=Object::Type::Fixture;
			m_commandParserCommandGroup.reset(new StateClearMain());
			break;
		case '/':
		case 'r':	
			{
				longText="Rotate selection";
				m_objectType=Object::Type::Fixture;
				m_ownCommand.reset(new Command::CommandBase(Command::CommandBase::EExecuteType::GUI, Object::Type::Fixture));	
				auto func=std::function<void(Command::ICommandGui*)>(&Command::ICommandGui::commandRotateSelection);
				m_ownCommand->setCommand(func);
				break;
			}
		default:
			hasError=true;
			longText=newChar;
			formattedCommandText.addErrorMessage("Error: Could not find command group "+longText);
			break;
	}
	std::string shortCommand;
	shortCommand=newChar;
	std::string textColor;
	if (hasError)
		textColor="red";
	else
		textColor="blue";
	formattedCommandText.appendRawCommandPart(shortCommand);
	formattedCommandText.appendHtmlCommandPart(std::string("<font color=\""+textColor+"\">"+longText+"</font> "));
	m_followingParserState=m_commandParserCommandGroup;
	return true;
}

std::shared_ptr<State> StateInitial::getFollowingParserState()
{
	return m_followingParserState;
}

Command::CommandBase::List StateInitial::getResultingCommand() const
{
	if (!m_followingParserState)
	{
		if (m_ownCommand)
			return Command::CommandBase::List{m_ownCommand};
		else
			return Command::CommandBase::List();
	}
	return m_followingParserState->getResultingCommand();
}
