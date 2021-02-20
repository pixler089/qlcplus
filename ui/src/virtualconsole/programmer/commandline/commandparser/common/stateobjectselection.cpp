#include "stateobjectselection.h"

#include "privateobjectselection/stateobjectselectionsubstate.h"
#include "virtualconsole/programmer/commandline/commandparser/commandtext.h"

#include <iostream>
#include <memory>
#include <sstream>

using namespace CommandParser;

static const std::string helpCommandsMultiselect="\
	<ul> \
		<li>Zahlen: Objektnummern </li>\
		<li>Zahlen startend mit 0: Gruppen</li>\
		<li>+/-: Hinzufügen/Löschen von Objekten aus der Liste</li>\
		<li>*: Bereich, Beispiel: 1*5 ergibt die Objekte 1, 2, 3, 4, 5</li>\
		<li>/: Teilung, 1*9/3 ergibt die Objekte 1 und 4 und 7, 1*9//3 die Objekte 2, 5 und 8</li>\
		<li>(): Klammerung möglich, z.B. (1*3+7*9)/3</li>\
	</ul> \
";

static const std::string helpCommandsSingleselect="\
		Auswahl <b>eines</b> Objektes als Zahl \
";

StateObjectSelection::StateObjectSelection(bool singleSelect)
	: m_singleSelect(singleSelect)
{
}

StateObjectSelection::~StateObjectSelection()
{
}

void StateObjectSelection::finish(CommandText& formattedCommandText)
{
	std::string helpMessage;
	std::string hintMessage;
	getHelpHintMessages(helpMessage, hintMessage);
	formattedCommandText.replaceHelpGreaterSymbolWithText(helpMessage);
	if (m_commandBuffer.empty())
		return;
	formattedCommandText.appendHtmlSpace();
}

void StateObjectSelection::clear()
{
	m_commandBuffer.clear();
	m_objectList.clear();
	//m_objectListLhs.clear();
	m_activeSubstate.reset();
}

bool StateObjectSelection::parseChar(char newChar, CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
	if (m_activeSubstate)
	{
		m_activeSubstate.reset();
		if (m_singleSelect)
			return false;
	}
	m_activeSubstate=StateObjectSelectionSubstate::getStateForChar(newChar, m_singleSelect);
	if (m_activeSubstate)
		m_activeSubstate->setObjectListLhs(m_objectList);
	return false;
}

std::shared_ptr<State> StateObjectSelection::getFollowingParserState()
{
	return m_activeSubstate;
}

void StateObjectSelection::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	std::ostringstream stream;
	if (m_singleSelect)
		stream<<helpCommandsSingleselect;
	else
		stream<<helpCommandsMultiselect;
	stream<<std::endl<<"\nSelected Objects:\n"<<std::endl;
	bool isFirstObject=true;
	for (int object : m_objectList.getCurrentObjects())
	{
		if (isFirstObject)
		{
			isFirstObject=false;
		}
		else
		{
			stream<<", ";
		}
		stream<<object;
	}
	helpMessage=stream.str();
	hintMessage="&lt;Selection&gt;";
}

void StateObjectSelection::returnAfterStacked(CommandText& commandText)
{
	std::ignore=commandText;
	m_objectList=m_activeSubstate->getObjects();
//	for (int object : m_activeSubstate->getObjects())
//		m_objectList.push_back(object);
}

bool StateObjectSelection::charIsStartcharForSelection(char charToCheck) const
{
	return StateObjectSelectionSubstate::getStateForChar(charToCheck, false)!=nullptr;
}

Command::CommandBase::List StateObjectSelection::getResultingCommand() const
{
	/*
	//First we handle commands that our State has to create (there don't exist substates)
	if (m_commandType==CommandType::Select)
	{
		std::shared_ptr<Command::CommandWorkspaceSelect> commandReturn(new Command::CommandWorkspaceSelect());	
		if (!m_stateSelection || m_stateSelection->getObjects().size()!=1)
		{
			return nullptr;
		}
		commandReturn->setWorkspaceToSelect(m_stateSelection->getObjects().front());

		return commandReturn;
	}
	//No we handle commands that will be created by a substate
	if (!m_followingParserState)
		return nullptr;
	return m_followingParserState->getResultingCommand();
	*/
	return Command::CommandBase::List();
}

