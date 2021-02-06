#include "commandtextedit.h"

#include "commandparser/parser.h"
#include "commandparser/common/stateinitial.h"
#include "helper/htmlstring.h"
#include "command/commandqueue.h"

#include <iostream>
#include <QKeyEvent>

void appendHtmlCommandParts(std::string& stringToAppend, const std::list<CommandParser::CommandText::CommandTextPart>& commandPartList)
{
		for ( CommandParser::CommandText::CommandTextPart formattedPart : commandPartList )
		{
			stringToAppend+=formattedPart.m_htmlString;
		}
}

CommandTextEdit::CommandTextEdit(QWidget* parent) 
	: QTextEdit(parent)
{
	connect(this, &QTextEdit::cursorPositionChanged, this, &CommandTextEdit::cursorPositionChangeEvent);
	installEventFilter(this);
}

CommandTextEdit::~CommandTextEdit()
{
}

void CommandTextEdit::clear()
{
	m_commands.clear();
	m_commandText.clear();
	m_commandParser.clear();
	m_inputCommand.clear();
	QTextEdit::clear();
}
	
void CommandTextEdit::keyPressEvent(QKeyEvent* event)
{
	size_t oldCursorPosition=getCursorPositionRaw().positionRaw;
	size_t newCursorPosition=oldCursorPosition;

	if (event->key()==Qt::Key_Backspace)
	{
		if (oldCursorPosition<1)
			return;
		newCursorPosition=oldCursorPosition-1;
		m_inputCommand.erase(newCursorPosition,1);
	}
	else if (event->key()==Qt::Key_Delete)
	{
		if (oldCursorPosition>m_inputCommand.size()-1)
			return;
		m_inputCommand.erase(newCursorPosition,1);
	}
	else if (event->key()==Qt::Key_Escape)
	{
		clear();
	}
	else if (event->key()==Qt::Key_Left)
	{
		if (oldCursorPosition>0)
			--newCursorPosition;
	}
	else if (event->key()==Qt::Key_Right)
	{
		if (oldCursorPosition<m_inputCommand.size())
			++newCursorPosition;
	}
	else if (event->key()==Qt::Key_Home ||
				event->key()==Qt::Key_End )
	{
		QTextEdit::keyPressEvent(event);
		return;
	}
	else
	{
		if (event->text().size()<1)
			return;
		QChar key=event->text().at(0);
		if (key.isPrint()&&key.unicode()<128)
		{
			m_inputCommand.insert(oldCursorPosition, 1, static_cast<char>(key.unicode()));
			++newCursorPosition;
		}
	}
	analyzeCommand(m_inputCommand);
	QTextCursor curs=textCursor();
	curs.setPosition(static_cast<int>(cursorPositionHtml(newCursorPosition)));
	setTextCursor(curs);
}

bool CommandTextEdit::eventFilter(QObject* object, QEvent* event)
{
	std::ignore=object;
	if (event->type()!=QEvent::KeyPress)
		return false;
	QKeyEvent* keyEvent=static_cast<QKeyEvent*>(event);
	if (keyEvent->key()==Qt::Key_Enter || keyEvent->key()==Qt::Key_Return)
	{
		for (std::shared_ptr<Command::CommandBase> cmd : m_commands)
		{
			cmd->setWorkspaceId(m_currentWorkspace);
			if (cmd->getExecuteType()==Command::CommandBase::EExecuteType::GUI)
			{
				cmd->setCommandGuiInterface(m_commandGuiInterface);
				cmd->execute();
			}
			else
			{
				Command::CommandQueue::instance().pushCommand(cmd);
			}
		}
		clear();
		return true;
	}
	return false;
}

void CommandTextEdit::cursorPositionChangeEvent()
{
	CursorPosition pos=getCursorPositionRaw();
	QTextCursor curs=textCursor();
	curs.setPosition(static_cast<int>(pos.positionHtmlRounded));
	setTextCursor(curs);
	std::list<CommandParser::CommandText::CommandTextPart> commandParts=m_commandText.getHtmlCommandParts();
	std::string errorMessage=m_commandText.getErrorMessage();
	if (!errorMessage.empty())
		errorMessage=std::string("<br><br><font color=\"red\">Fehler:<br>")+errorMessage+"</font>";
	if (pos.positionRaw==0)
	{
		emit helpMessage(CommandParser::StateInitial::getHelpMessage()+errorMessage);
	}
	else if (pos.positionRaw<commandParts.size())
	{
		auto itPart=commandParts.begin();
		std::advance(itPart, pos.positionRaw);
		emit helpMessage(itPart->m_helpMessage+errorMessage);
	}
	else
	{
		emit helpMessage(m_commandText.getHelpMessageLastCommand()+errorMessage);
	}
}

CommandTextEdit::CursorPosition CommandTextEdit::getCursorPositionRaw() const
{
	CursorPosition pos;
	pos.positionRaw=0;
	pos.positionHtmlRounded=0;

	size_t cursorPositionHtml=static_cast<size_t>(std::max(0,textCursor().position()));

	for ( CommandParser::CommandText::CommandTextPart formattedPart : m_commandText.getHtmlCommandParts() )
	{
		size_t stringLength=formattedPart.m_htmlString.visibleCharCount();
		if ((stringLength+pos.positionHtmlRounded)>cursorPositionHtml)
		{
			if ((stringLength/2+pos.positionHtmlRounded)<cursorPositionHtml)
			{
				++pos.positionRaw;
				pos.positionHtmlRounded+=stringLength;
			}
			return pos;
		}
		++pos.positionRaw;
		pos.positionHtmlRounded+=stringLength;
	}
	return pos;
}

size_t CommandTextEdit::cursorPositionHtml(size_t cursorPositionRaw) const
{
	size_t posHtml=0;
	size_t posRaw=0;
	for ( CommandParser::CommandText::CommandTextPart formattedPart : m_commandText.getHtmlCommandParts() )
	{
		if (posRaw==cursorPositionRaw)
			return posHtml;
		posHtml+=formattedPart.m_htmlString.visibleCharCount();
		++posRaw;
	}
	return posHtml;
}
void CommandTextEdit::analyzeCommand(const std::string& rawCommand)
{
	std::ignore=rawCommand;
	m_commandText.clear();
	m_commandParser.parseCommand(m_inputCommand, m_commandText);
	m_commands=m_commandParser.getCommands();

	std::string formattedCommand;

	appendHtmlCommandParts(formattedCommand, m_commandText.getHtmlCommandParts() );
	formattedCommand+="<font color=\"gray\">"+m_commandText.getHintMessageLastCommand()+"</font>";
	setText(QString::fromStdString(formattedCommand));
}
void CommandTextEdit::focusInEvent(QFocusEvent* event)
{
	cursorPositionChangeEvent();//TODO bisschen brachial das immer zu machen...
	QTextEdit::focusInEvent(event);
}
