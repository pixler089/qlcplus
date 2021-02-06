#include "commandtext.h"

#include <iostream>

using namespace CommandParser;

CommandText::CommandText()
{
}

CommandText::~CommandText()
{
}

void CommandText::clear()
{
	m_rawCommand.clear();
	m_htmlCommands.clear();
	m_errorMessage.clear();
}

void CommandText::appendRawCommandPart(char newChar)
{
	m_rawCommand+=newChar;
}

void CommandText::appendRawCommandPart(std::string rawCommand)
{
	m_rawCommand+=rawCommand;
}


void CommandText::appendHtmlCommandPart(HtmlString subPart, std::string helpMessage)
{
	m_htmlCommands.push_back(CommandTextPart(subPart, helpMessage));
}


std::string CommandText::getRawCommand() const
{
	return m_rawCommand;
}

std::list<CommandText::CommandTextPart> CommandText::getHtmlCommandParts() const
{
	return m_htmlCommands;
}

void CommandText::addErrorMessage(std::string errorMessage)
{
	if (m_errorMessage.size())
	{
		m_errorMessage+="\n";
	}
	m_errorMessage+=errorMessage;
}

void CommandText::appendHtmlSpace()
{
	if (m_htmlCommands.size()==0)
		return;
	std::list<CommandTextPart>::iterator it=m_htmlCommands.end();
	--it;
	(*it).m_htmlString+=' ';	
}

void CommandText::merge(const CommandText& rhs)
{
	m_rawCommand+=rhs.m_rawCommand;
	for (auto htmlPart : rhs.m_htmlCommands)
	{
		m_htmlCommands.push_back(htmlPart);
	}
	m_errorMessage+=rhs.m_errorMessage;
	m_helpMessageLastCommand=rhs.m_helpMessageLastCommand;
	m_hintMessageLastCommand=rhs.m_hintMessageLastCommand;
}

void CommandText::replaceHelpGreaterSymbolWithText(std::string newHelpText)
{
	for (auto& htmlPart : m_htmlCommands)
	{
		if (htmlPart.m_helpMessage==">")
			htmlPart.m_helpMessage=newHelpText;
	}
}
