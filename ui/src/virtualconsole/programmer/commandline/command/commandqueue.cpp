#include "commandqueue.h"

#include "virtualconsole/programmer/commandline/command/command.h"

using namespace Command;

CommandQueue::CommandQueue()
{
}

CommandQueue::~CommandQueue()
{
}

void CommandQueue::pushCommand(std::shared_ptr<Command::CommandBase> command)
{
	command->execute();
}
