#include "stateobjectselectionsubstateoperator.h"

#include "virtualconsole/programmer/commandline/commandparser/commandtext.h"

#include <iostream>
#include <sstream>

using namespace CommandParser;

StateObjectSelectionSubstateOperator::StateObjectSelectionSubstateOperator(bool isSingleSelect)
	: StateObjectSelectionSubstate(isSingleSelect)
{
}

StateObjectSelectionSubstateOperator::~StateObjectSelectionSubstateOperator()
{
}


void StateObjectSelectionSubstateOperator::clear()
{
	m_objectList.clear();
	m_objectListLhs.clear();
	m_objectListRhs.clear();
	m_operator=0;
	m_operatorRepeatCount=0;
	m_charCount=0;
	m_nextOperator.reset();
}

void StateObjectSelectionSubstateOperator::finish(CommandText& formattedCommandText)
{
	if (m_operator=='-')
	{
		m_objectList=m_objectListLhs-m_objectListRhs;
	}
	else if (m_operator=='+')
	{
		m_objectList=m_objectListLhs+m_objectListRhs;
	}
	else if (m_operator=='*')
	{
		if (m_objectListLhs.size()==1 && m_objectListRhs.size()==1)
		{
			//TOOD unschoene Loesung hier aus der ObjectList wieder int zu machen.....
			m_objectList=VcProgrammerSelectedObjects::createRange(m_objectListLhs.front().m_objectID, m_objectListRhs.front().m_objectID);
		}
		else
		{
			if (m_objectListLhs.size()!=1)
			{
				std::ostringstream os;
				os<<"Operator *: Falsche Zahl Operanden links des Operators ("<<m_objectListLhs.size()<<")";
				formattedCommandText.addErrorMessage(os.str());
			}
			if (m_objectListRhs.size()>1)
			{
				std::ostringstream os;
				os<<"Operator *: Falsche Zahl Operanden rechts des Operators ("<<m_objectListRhs.size()<<")";
				formattedCommandText.addErrorMessage(os.str());
			}
		}
	}
	else if (m_operator=='/')
	{
		if (m_objectListRhs.size()==1)
		{
			//TOOD unschoene Loesung hier aus der ObjectList wieder int zu machen.....
			int divisor=m_objectListRhs.front().m_objectID;
			int counterStart=divisor-m_operatorRepeatCount+1;
			m_objectList.setRotation(counterStart, divisor);
		}
		else
		{
			if (m_objectListRhs.size()>1)
			{
				std::ostringstream os;
				os<<"Operator /: Falsche Zahl Operanden rechts des Operators ("<<m_objectListRhs.size()<<")";
				formattedCommandText.addErrorMessage(os.str());
			}
		}
	}
	else
	{
		formattedCommandText.addErrorMessage(std::string("Unknown Operator ")+m_operator+"!");
	}
	if (m_objectList.empty())
		m_objectList=m_objectListLhs;
}

bool StateObjectSelectionSubstateOperator::parseChar(char newChar, CommandText& formattedCommandText)
{
	m_charCount++;
	if (m_charCount==(m_operatorRepeatCount+1) && (m_operator==0 || newChar==m_operator))
	{
		m_operator=newChar;
		formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"saddleBrown\">")+newChar+"</font>", ">");
		formattedCommandText.appendRawCommandPart(newChar);
		m_operatorRepeatCount++;
		return true;
	}
	if (getOperatorPrecedence(m_operator)>=getOperatorPrecedence(newChar))
	{
		m_nextOperator.reset();
		finish(formattedCommandText);
		return false;
	}
	m_nextOperator=StateObjectSelectionSubstate::getStateForChar(newChar, isSingleSelect());
	m_nextOperator->setObjectListLhs(m_objectListRhs);
	return false;
}

std::shared_ptr<State> StateObjectSelectionSubstateOperator::getFollowingParserState()
{
	return m_nextOperator;
}

void StateObjectSelectionSubstateOperator::returnAfterStacked(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
	if (m_nextOperator)
		m_objectListRhs=m_nextOperator->getObjects();
	m_nextOperator.reset();
}

void StateObjectSelectionSubstateOperator::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	std::ignore=hintMessage;
	helpMessage=">";
}

int StateObjectSelectionSubstateOperator::getOperatorPrecedence(char operatorChar)
{
	if (operatorChar >= '0' && operatorChar <= '9')
		return 999;
	//same as C++
	switch (operatorChar)
	{
		case '*':
		case '/':
			return 5;
		case '+':
		case '-':
			return 3;
		case '(':
			return 99;
		default:
			return -1;
	}
}
