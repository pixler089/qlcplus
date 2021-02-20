#ifndef COMMANDPARSER_STATEOBJECTSELECTIONSUBSTATEINT_H
#define COMMANDPARSER_STATEOBJECTSELECTIONSUBSTATEINT_H

#include "stateobjectselectionsubstate.h"
#include "../../../..//vcprogrammerselectedobjects.h"

namespace CommandParser
{

class StateObjectSelectionSubstateInt : public StateObjectSelectionSubstate
{
public:
	///@param singleSelect if true, only one object can be selected
	StateObjectSelectionSubstateInt(bool singleSelect);
	virtual ~StateObjectSelectionSubstateInt();

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
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const override;
	virtual VcProgrammerSelectedObjects getObjects() const { return m_objectList; };
private:
	std::string m_buffer;
	VcProgrammerSelectedObjects m_objectList;
};

}

#endif
