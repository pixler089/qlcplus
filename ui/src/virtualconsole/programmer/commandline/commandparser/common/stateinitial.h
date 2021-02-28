#ifndef COMMANDPARSER_STATEINITIAL_H
#define COMMANDPARSER_STATEINITIAL_H

#include "../state.h"
#include "virtualconsole/programmer/commandline/objects/object.h"

namespace CommandParser
{

class StateInitial : public State
{
public:
	StateInitial();
	virtual ~StateInitial();

	///The pointer will be valid until end of program	
	static const std::string getHelpMessage();
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
	virtual Command::CommandBase::List getResultingCommand() const override;
	///@brief returns the type of the objeect the command is related to
	Object::Type getObjectType() const { return m_objectType; };
private:
	Object::Type m_objectType = Object::Type::Unknown;
	std::shared_ptr<State> m_commandParserCommandGroup;
	std::shared_ptr<State> m_followingParserState;
	std::shared_ptr<Command::CommandBase> m_ownCommand;
};

};

#endif
