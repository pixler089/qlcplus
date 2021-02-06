#include "commandfixturesetvalue.h"

using namespace Command;

CommandFixtureSetValue::CommandFixtureSetValue()
	: CommandBase(CommandBase::EExecuteType::Backend, Object::Type::Fixture)
{
}

CommandFixtureSetValue::~CommandFixtureSetValue()
{
}

void CommandFixtureSetValue::execute()
{
}
