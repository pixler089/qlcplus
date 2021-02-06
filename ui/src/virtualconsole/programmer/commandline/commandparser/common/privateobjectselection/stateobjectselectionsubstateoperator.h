#ifndef COMMANDPARSER_STATEOBJECTSELECTIONSUBSTATEOPERATOR_H
#define COMMANDPARSER_STATEOBJECTSELECTIONSUBSTATEOPERATOR_H

#include "stateobjectselectionsubstate.h"

namespace CommandParser
{

class StateObjectSelectionSubstateOperator : public StateObjectSelectionSubstate
{
public:
	///@param singleSelect if true, only one object can be selected
	StateObjectSelectionSubstateOperator(bool isSingleSelect);
	virtual ~StateObjectSelectionSubstateOperator();

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
	virtual std::list<int> getObjects() const { return m_objectList; };
	virtual bool wantsBeStacked() const override { return true; };
	virtual void returnAfterStacked(CommandText& commandText) override;
	///@brief sets the (already parsed) objects on the left hand side of the Operator
	virtual void setObjectListLhs(const std::list<int>& objectsLhs) override { m_objectListLhs=objectsLhs; };
	static int getOperatorPrecedence(char operatorChar);
private:
	std::list<int> m_objectList;
	std::list<int> m_objectListLhs;
	std::list<int> m_objectListRhs;
	int m_operatorRepeatCount=0;
	char m_operator=0;
	int m_charCount=0;
	std::shared_ptr<StateObjectSelectionSubstate> m_nextOperator;
};

}

#endif
