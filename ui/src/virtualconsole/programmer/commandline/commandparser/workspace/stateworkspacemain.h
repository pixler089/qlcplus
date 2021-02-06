#ifndef COMMANDPARSER_STATEWORKSPACEMAIN_H
#define COMMANDPARSER_STATEWORKSPACEMAIN_H

#include <string>
#include <list>
#include <memory>

#include "../state.h"
#include "../common/stateobjectselection.h"

namespace CommandParser
{

class StateWorkspaceMain : public State
{
public:
	StateWorkspaceMain();
	virtual ~StateWorkspaceMain();
	///@brief resets the parser
	virtual void clear() override;
	///@brief will be called by the parser if the parser decides not to give more chars to the state (perhaps because there is no more input)
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual void finish(CommandText& formattedCommandText) override;
	///@brief parses one char. Returns true, if char has been parsed by this ParserState, otherwise false. A suggestion for the next ParserState will be given using getFollowingParserState.  
	///@param newChar the char to parse
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual bool parseChar(char newChar, CommandText& formattedCommandText) override;
	///@brief returns the ParserState that should be used to parse the next char. 0, if no ParserState available for this char.
	virtual std::shared_ptr<State> getFollowingParserState() override;
	virtual bool wantsBeStacked() const override { return true; };
	///Returns the command parsed. Returns 0 in case of an error.
	virtual std::shared_ptr<Command::CommandBase> getResultingCommand() const override;
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const override;
private:
	enum class CommandType
	{
		NoCommand,
		New,
		Select
	} m_commandType=CommandType::NoCommand;
	std::shared_ptr<State> m_followingParserState;
	std::shared_ptr<StateObjectSelection> m_stateSelection;
	char selectedSubcommand=0;
};

};

#endif
