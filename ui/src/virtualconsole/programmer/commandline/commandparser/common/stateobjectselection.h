#ifndef COMMANDPARSER_STATEOBJECTSELECTION_H
#define COMMANDPARSER_STATEOBJECTSELECTION_H

#include "../state.h"

namespace CommandParser
{

class StateObjectSelectionSubstate;

class StateObjectSelection : public State
{
public:
	///@param singleSelect if true, only one object can be selected
	StateObjectSelection(bool singleSelect=false);
	virtual ~StateObjectSelection();

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
	virtual void getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const override;
	virtual void returnAfterStacked(CommandText& commandText) override;
	virtual std::list<int> getObjects() const { return m_objectList; };
	bool charIsStartcharForSelection(char charToCheck) const;
private:
	std::string m_commandBuffer;
	std::list<int> m_objectList;
	bool m_singleSelect=false;
	std::shared_ptr<StateObjectSelectionSubstate> m_activeSubstate;
	//std::list<int> m_objectListLhs;
};

};

#endif
