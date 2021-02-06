#include "command.h"

namespace Command
{

CommandBase::CommandBase(EExecuteType executeType, Object::Type objectType)
	: m_objectType(objectType),
	  m_executeType(executeType)

{
}

CommandBase::~CommandBase()
{
}

}
