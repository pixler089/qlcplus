#ifndef COMMANDPARSER_STATEOBJECTSELECTIONSUBSTATE_H
#define COMMANDPARSER_STATEOBJECTSELECTIONSUBSTATE_H

#include "../../state.h"

namespace CommandParser
{

class StateObjectSelectionSubstate : public State
{
public:
	///@param singleSelect if true, only one object can be selected
	StateObjectSelectionSubstate(bool singleSelect);
	virtual ~StateObjectSelectionSubstate();

	/////////////////////////////
	// State specific ///////////
	
	///@brief resets the parser
	virtual void clear() = 0;
	///@brief will be called by the parser if the parser decides not to give more chars to the state (perhaps because there is no more input)
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual void finish(CommandText& formattedCommandText) = 0; 
	///@brief parses one char. Returns true, if char has been parsed by this ParserState, otherwise false. A suggestion for the next ParserState will be given using getFollowingParserState.  
	///@param newChar the char to parse
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	virtual bool parseChar(char newChar, CommandText& formattedCommandText) = 0;
	///@brief returns the ParserState that should be used to parse the next char. 0, if no ParserState available for this char.
	virtual std::shared_ptr<State> getFollowingParserState() = 0;
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const = 0;
	/////////////////////////
	// Substate specific ////

	virtual void setObjectListLhs(const std::list<int>& objectsLhs)
	{
		std::ignore=objectsLhs;
	};
	virtual std::list<int> getObjects() const = 0;
	static std::shared_ptr<StateObjectSelectionSubstate> getStateForChar(char aChar, bool isSingleSelect);
	bool isSingleSelect() const { return m_singleSelect;};
private:
	bool m_singleSelect=false;
};

}

#endif
