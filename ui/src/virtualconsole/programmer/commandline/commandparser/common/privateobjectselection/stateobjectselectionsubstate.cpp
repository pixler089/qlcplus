#include "stateobjectselectionsubstate.h"

#include "stateobjectselectionsubstateint.h"
#include "stateobjectselectionsubstateoperator.h"

using namespace CommandParser;

StateObjectSelectionSubstate::StateObjectSelectionSubstate(bool singleSelect)
	: m_singleSelect(singleSelect)
{
}

StateObjectSelectionSubstate::~StateObjectSelectionSubstate()
{
}

std::shared_ptr<StateObjectSelectionSubstate> StateObjectSelectionSubstate::getStateForChar(char aChar, bool isSingleSelect)
{
	if (aChar>='0' && aChar<='9')
	{
		std::shared_ptr<StateObjectSelectionSubstate> newState(new StateObjectSelectionSubstateInt(isSingleSelect));
		return newState;
	}
	switch (aChar)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		{
			std::shared_ptr<StateObjectSelectionSubstateOperator> newState(new StateObjectSelectionSubstateOperator(isSingleSelect));
			return newState;
		}
		default:
			return nullptr;
	}
	return nullptr;
}
