#ifndef COMMAND_COMMANDQUEUE_H
#define COMMAND_COMMANDQUEUE_H

#include <memory>
#include <queue>
#include <functional>

namespace Command
{

class CommandBase;

class CommandQueue
{
public:
	struct NotifierRegistration
	{
		std::function<void(int)> publishNotifier;
	   	std::function<void(int)> removeNotifier;
	};
	static CommandQueue& instance()
	{
		static CommandQueue instance;
		return instance;
	}

	void pushCommand(std::shared_ptr<Command::CommandBase> command);
//	std::shared_ptr<Command::CommandBase> popCommandGui();
//	std::shared_ptr<Command::CommandBase> popCommandBackend();

private:
//	std::queue<std::shared_ptr<Command::CommandBase>> m_commandQueueGui;
//	std::queue<std::shared_ptr<Command::CommandBase>> m_commandQueueBackend;

////////////////////////////
////// Internal ////////////
////////////////////////////
public:
	CommandQueue(const CommandQueue&) = delete;
	CommandQueue(CommandQueue&&) = delete;
	CommandQueue& operator=(const CommandQueue&) = delete;
	CommandQueue& operator=(CommandQueue&&) = delete;
protected:
	CommandQueue();
	virtual ~CommandQueue();
};

}

#endif
