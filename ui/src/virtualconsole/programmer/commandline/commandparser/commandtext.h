#ifndef COMMANDTEXT_H
#define COMMANDTEXT_H

#include "virtualconsole/programmer/commandline/helper/htmlstring.h"

#include <list>

namespace CommandParser
{

class CommandText
{
public:
	struct CommandTextPart
	{
		CommandTextPart(HtmlString htmlString, std::string helpMessage)
			: m_htmlString(htmlString),
			  m_helpMessage(helpMessage)
		{
		}
		HtmlString m_htmlString;
		/// use "r" to repeat the helpMessage from the prev command, and "" to show no help message
		std::string m_helpMessage;
	};	

	CommandText();
	virtual ~CommandText();

	///@brief clears the command part
	void clear();
	///@brief sets the original input of the command part
	void appendRawCommandPart(std::string rawCommand);
	void appendRawCommandPart(char newChar);
	///@brief appends a html command to the list of subparts
	///@param helpMessage use "r" to repeat the helpMessage from the prev command, and "" to show no help message
	void appendHtmlCommandPart(HtmlString subPart, std::string helpMessage="");
	///@brief inserts a space in html view
	void appendHtmlSpace();
	void setHelpMessageLastCommand(std::string helpMessage) { m_helpMessageLastCommand=helpMessage;};
	void setHintMessageLastCommand(std::string hintMessage) { m_hintMessageLastCommand=hintMessage;};

	void merge(const CommandText& rhs);

	///@brief use this method to replace all ">" error messages before with newHelpText
	void replaceHelpGreaterSymbolWithText(std::string newHelpText);

	///@brief gets the original input of the command part
	std::string getRawCommand() const;
	///@brief gets the HTML formatted expanded version of the command part. Each string represents one char of the raw command
	std::list<CommandTextPart> getHtmlCommandParts() const;
	void addErrorMessage(std::string errorMessage);
	std::string getErrorMessage() const { return m_errorMessage; };
	std::string getHelpMessageLastCommand() const { return m_helpMessageLastCommand; };
	std::string getHintMessageLastCommand() const { return m_hintMessageLastCommand; };
private:
	std::string m_rawCommand;
	std::list<CommandTextPart> m_htmlCommands;
	std::string m_errorMessage;
	std::string m_helpMessageLastCommand;
	std::string m_hintMessageLastCommand;
};

};

#endif
