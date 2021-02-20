#ifndef COMMANDPARSER_PARSER_H
#define COMMANDPARSER_PARSER_H

#include <string>
#include <list>
#include <memory>

#include "virtualconsole/programmer/commandline/command/command.h"

namespace CommandParser
{

class CommandText;
class State;

class Parser
{
public:
	Parser();
	virtual ~Parser();
	///@brief parses a command string. The result can be retrieved using getPommandParts.
	///@param formattedCommandText Output of the command text in pretty formatted expanded variant. This will be showed to the user.
	///@return true on success, false in case of an error.
	bool parseCommand(std::string command, CommandText& formattedCommandText);
	///@brief returns a human readable error message if the last command wasn't parsed successful.
	std::string getErrorMessage() const;
	///@brief returns a copy of the current command
	Command::CommandBase::List getCommands() const { return m_commands; };

	///@brief deletes the data of the last parsed command
	void clear(); 

private:
	Command::CommandBase::List m_commands;
	std::list<std::shared_ptr<State>> m_stateStack;
};

};

#endif
