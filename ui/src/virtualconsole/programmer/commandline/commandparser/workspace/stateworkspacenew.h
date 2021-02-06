#ifndef WORKSPACEPARSERCREATE_H
#define WORKSPACEPARSERCREATE_H

#include <string>
#include <list>
#include <memory>

#include "../state.h"

namespace Command
{
	class CommandBase;
}

namespace CommandParser
{

class StateWorkspaceNew : public State
{
public:
	StateWorkspaceNew();
	virtual ~StateWorkspaceNew();
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
	///Returns the command parsed. Returns 0 in case of an error.
	virtual std::shared_ptr<Command::CommandBase> getResultingCommand() const override;
	///Method to get the help and hint-Messages of the State
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const override;
private:
	int getId() const;
	enum class EInternalState
	{
		ID,
		Name
	};
	EInternalState m_internalState=EInternalState::ID;
	std::string m_strID;
	std::string m_strName;

};

};

#endif
