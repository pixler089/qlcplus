#ifndef COMMANDPARSER_STATE_H
#define COMMANDPARSER_STATE_H

#include <string>
#include <list>
#include <memory>

namespace Command
{
	class CommandBase;
}

namespace CommandParser
{

class CommandText;

class State
{
public:
	State();
	virtual ~State();
	///@brief resets the parser
	virtual void clear() = 0;
	///@brief will be called by the parser if the parser decides not to give more chars to the state (perhaps because there is no more input)
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual void finish(CommandText& formattedCommandText) 
	{
		std::ignore=formattedCommandText;
	};
	///@brief parses one char. Returns true, if char has been parsed by this ParserState, otherwise false. A suggestion for the next ParserState will be given using getFollowingParserState.  
	///@param newChar the char to parse
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual bool parseChar(char newChar, CommandText& formattedCommandText) = 0;
	///@brief returns the ParserState that should be used to parse the next char. 0, if no ParserState available for this char.
	virtual std::shared_ptr<State> getFollowingParserState() = 0;
	///if true, the command parser will push this state to a stack if getFollowingParserState has been != nullptr. If any of the followed commands don't like the parsed char and has no followingState, the
	///parser will go back to the newest stacked state.
	virtual bool wantsBeStacked() const { return false; };
	///will be called if the substate returns so the stacked state will be reactivated.
	virtual void returnAfterStacked(CommandText& commandText)
	{
		std::ignore=commandText;
	};
	///Method to get the help and hint-Messages of the State
	///TODO ggf. auf prue virtuell setzen
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const 
	{
		std::ignore=helpMessage;
		std::ignore=hintMessage;
	};
	///Returns the command parsed. Returns 0 in case of an error.
	virtual std::shared_ptr<Command::CommandBase> getResultingCommand() const {return nullptr; };
};

};

#endif
