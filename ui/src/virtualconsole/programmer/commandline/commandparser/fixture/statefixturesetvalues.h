#ifndef COMMANDPARSER_STATEFIXTURESETVALUES_H
#define COMMANDPARSER_STATEFIXTURESETVALUES_H

#include "../state.h"

namespace CommandParser
{

class StateFixtureSetValues : public State
{
public:
	StateFixtureSetValues();
	virtual ~StateFixtureSetValues();

	///@brief resets the parser
	virtual void clear() override;
	///@brief will be called by the parser if the parser decides not to give more chars to the state (perhaps because there is no more input)
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual void finish(CommandText& formattedCommandText);
	///@brief parses one char. Returns true, if char has been parsed by this ParserState, otherwise false. A suggestion for the next ParserState will be given using getFollowingParserState.  
	///@param newChar the char to parse
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual bool parseChar(char newChar, CommandText& formattedCommandText) override;
	///@brief returns the ParserState that should be used to parse the next char. 0, if no ParserState available for this char.
	virtual std::shared_ptr<State> getFollowingParserState() override;
	///if true, the command parser will push this state to a stack if getFollowingParserState has been != nullptr. If any of the followed commands don't like the parsed char and has no followingState, the
	///parser will go back to the newest stacked state.
	virtual bool wantsBeStacked() const override { return false; };
	///will be called if the substate returns so the stacked state will be reactivated.
	virtual void returnAfterStacked(CommandText& commandText) override;
	///Method to get the help and hint-Messages of the State
	///TODO ggf. auf prue virtuell setzen
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const override;
	///Returns the command parsed. Returns 0 in case of an error.
	virtual Command::CommandBase::List getResultingCommand() const override;
private:
	enum class ColorState
	{
		Red=0,
		Green=1,
		Blue=2,
		White=3,
		Amber=4,
		UV=5
	};
	std::string getColorString(ColorState colorstate) const; 
	enum class InternalState
	{
		Start,
		Brightness,
		Color,
		Flags
	};
	InternalState m_internalState=InternalState::Start;
	std::string m_brightnessString;
	ColorState m_colorPartState=ColorState::Red;

};

}//namespace CommandParser

#endif
