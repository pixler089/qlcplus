#ifndef COMMANDPARSER_STATEFIXTUREMAIN_H
#define COMMANDPARSER_STATEFIXTUREMAIN_H

#include <string>
#include <list>
#include <memory>

#include "../state.h"

namespace CommandParser
{

class StateObjectSelection;

class StateFixtureMain : public State
{
public:
	StateFixtureMain();
	virtual ~StateFixtureMain();
	///@brief resets the parser
	virtual void clear() override;
	///@brief will be called by the parser if the parser decides not to give more chars to the state (perhaps because there is no more input)
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual void finish(CommandText& formattedCommandText) override;
	///@brief parses one char. Returns true, if char has been parsed by this ParserState, otherwise false. A suggestion for the next ParserState will be given using getFollowingParserState.  
	///@param newChar the char to parse
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual bool parseChar(char newChar, CommandText& formattedCommandText) override;
	///if true, the command parser will push this state to a stack if getFollowingParserState has been != nullptr. If any of the followed commands don't like the parsed char and has no followingState, the
	///parser will go back to the newest stacked state.
	virtual bool wantsBeStacked() const override { return true; };
	///will be called if the substate returns so the stacked state will be reactivated.
	virtual void returnAfterStacked(CommandText& commandText) override;
	///@brief returns the ParserState that should be used to parse the next char. 0, if no ParserState available for this char.
	virtual std::shared_ptr<State> getFollowingParserState() override;
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const override;
	virtual Command::CommandBase::List getResultingCommand() const override;
private:
	enum class InternalState
	{
		Start,
		Selection,
		Subcommand
	};
	InternalState m_state=InternalState::Start;
	enum class SubCommand
	{
		NoCommand,
		SetValues
	};
	SubCommand m_subcommand=SubCommand::NoCommand;
	std::shared_ptr<StateObjectSelection> m_stateSelection;
	std::shared_ptr<State> m_stateFollowing;
};

};

#endif
