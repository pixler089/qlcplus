#include "statefixturemain.h"

#include "../common/stateobjectselection.h"
#include "../../command/icommandgui.h"
#include "statefixturesetvalues.h"
#include <iterator>

using namespace CommandParser;

static const std::string helpCommands="\
	<table> \
		<tr><td><font color=\"red\">-</font> noch nicht definiert</td></tr> \
	</table> \
";

StateFixtureMain::StateFixtureMain()
{
	m_stateSelection.reset(new StateObjectSelection());
}

StateFixtureMain::~StateFixtureMain()
{
}

void StateFixtureMain::clear()
{
	m_state=InternalState::Start;
	m_stateSelection->clear();
	m_stateFollowing.reset();
}

void StateFixtureMain::finish(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
}

bool StateFixtureMain::parseChar(char newChar, CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
	if (m_state!=InternalState::Start)
	{
		return false;
	}

	if (m_stateSelection->charIsStartcharForSelection(newChar))
	{
		m_stateFollowing=m_stateSelection;
		m_subcommand=SubCommand::SetValues;
		m_state=InternalState::Selection;
		return false;
	}
	return false;
}

std::shared_ptr<State> StateFixtureMain::getFollowingParserState()
{
	return m_stateFollowing;
}

void StateFixtureMain::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	std::string hintMessageSelection;
	std::string helpMessageSelection;
	m_stateSelection->getHelpHintMessages(helpMessageSelection, hintMessageSelection);
	hintMessage=hintMessageSelection+"/&lt;Subcommand&gt;";
	helpMessage+=helpCommands+"<br><br>oder Fixtureauswahl:"+helpMessageSelection;
}

void StateFixtureMain::returnAfterStacked(CommandText& commandText)
{
	std::ignore=commandText;
	if (m_state!=InternalState::Selection)
	{
		m_stateFollowing.reset();
		return;
	}
	if (m_subcommand==SubCommand::SetValues)
	{
		m_stateFollowing.reset(new StateFixtureSetValues());
	}
}

Command::CommandBase::List StateFixtureMain::getResultingCommand() const
{
	m_stateSelection->setRawCommand(std::function<void(Command::ICommandGui* commandGui,  VcProgrammerSelectedObjects objects)>(&Command::ICommandGui::commandSetSelectedFixtures));
	Command::CommandBase::List cmdList=m_stateSelection->getResultingCommand();
	if (m_stateFollowing)
	{
		auto cmdFollowing=m_stateFollowing->getResultingCommand();
		std::move(cmdFollowing.begin(), cmdFollowing.end(), std::back_inserter(cmdList));
	}
	return cmdList;
}
