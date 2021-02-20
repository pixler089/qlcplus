#include "stateworkspacemain.h"

#include "stateworkspacenew.h"
#include "../commandtext.h"
#include "../common/stateobjectselection.h"

#include "virtualconsole/programmer/commandline/command/workspace/commandworkspaceselect.h"

#include <iostream>
#include <memory>

using namespace CommandParser;

static const std::string helpCommands="\
	<table> \
		<tr><td><font color=\"red\">0</font>: <font color=\"red\">c</font>lear</td></tr> \
		<tr><td><font color=\"red\">1</font>: <font color=\"red\">e</font>nable</td></tr> \
		<tr><td><font color=\"red\">2</font>: <font color=\"red\">d</font>isable</td></tr> \
		<tr><td><font color=\"red\">3</font>: s<font color=\"red\">w</font>itch</td></tr> \
		<tr><td><font color=\"red\">4</font>: <font color=\"red\">n</font>ew</td></tr> \
		<tr><td><font color=\"red\">5</font>: <font color=\"red\">r</font>emove</td></tr> \
		<tr><td><font color=\"red\">6</font>: <font color=\"red\">s</font>elect (also if no subcommand)</td></tr> \
	</table> \
";

StateWorkspaceMain::StateWorkspaceMain()
{
}

StateWorkspaceMain::~StateWorkspaceMain()
{
}

void StateWorkspaceMain::clear()
{
	m_followingParserState.reset();
	m_stateSelection.reset();
	m_commandType=CommandType::NoCommand;
}

void StateWorkspaceMain::finish(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
}

bool StateWorkspaceMain::parseChar(char newChar, CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
	if (selectedSubcommand)
		return false;
	std::string longText;
	bool hasError=false;
	switch (newChar)
	{
		case 'n':
		case '4':
			longText="new";
			m_followingParserState.reset(new StateWorkspaceNew());
			m_commandType=CommandType::New;
			break;
		case 'e':
		case '1':
			longText="enable";
			m_stateSelection.reset(new StateObjectSelection(false));
			m_followingParserState=m_stateSelection;
			break;
		case 'd':
		case '2':
			longText="disable";
			m_stateSelection.reset(new StateObjectSelection(false));
			m_followingParserState=m_stateSelection;
			break;
		case 'w':
		case '3':
			longText="switch";
//			m_followingParserState.reset(new WorkspaceParserGeneric());
			break;
		case 'c':
		case '0':
			longText="clear";
			m_stateSelection.reset(new StateObjectSelection(false));
			m_followingParserState=m_stateSelection;
			break;
		case 's':
		case '6':
			longText="select";
			m_stateSelection.reset(new StateObjectSelection(true));
			m_followingParserState=m_stateSelection;
			m_commandType=CommandType::Select;
			break;
		case 'r':
		case '5':
			longText="remove";
			m_stateSelection.reset(new StateObjectSelection(false));
			m_followingParserState=m_stateSelection;
			break;
		default:
			hasError=true;
			longText=newChar;
			formattedCommandText.addErrorMessage(std::string("Error: Could not find command group ")+longText);
			m_followingParserState.reset();
	}
	if (!hasError)
		selectedSubcommand=newChar;
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

std::shared_ptr<State> StateWorkspaceMain::getFollowingParserState()
{
	return m_followingParserState;
}

std::shared_ptr<Command::CommandBase> StateWorkspaceMain::getResultingCommand() const
{
	//First we handle commands that our State has to create (there don't exist substates)
	if (m_commandType==CommandType::Select)
	{
		std::shared_ptr<Command::CommandWorkspaceSelect> commandReturn(new Command::CommandWorkspaceSelect());	
		if (!m_stateSelection || m_stateSelection->getObjects().size()!=1)
		{
			return nullptr;
		}
		commandReturn->setWorkspaceToSelect(m_stateSelection->getObjects().front().m_objectID);

		return commandReturn;
	}
	//No we handle commands that will be created by a substate
	if (!m_followingParserState)
		return nullptr;
	return m_followingParserState->getResultingCommand();
}

void StateWorkspaceMain::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	helpMessage=helpCommands;
	hintMessage="&lt;Subcommand&gt;";
}
