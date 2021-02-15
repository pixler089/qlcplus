#ifndef COMMANDTEXTEDIT_H
#define COMMANDTEXTEDIT_H

#include "virtualconsole/programmer/commandline/command/command.h"
#include "virtualconsole/programmer/commandline/commandparser/commandtext.h"
#include "virtualconsole/programmer/commandline/commandparser/parser.h"
#include <QTextEdit>
#include <memory>
#include <functional>

namespace Command
{
	class ICommandGui;
}

class CommandTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	struct CursorPosition
	{
		size_t positionRaw=0;
		size_t positionHtmlRounded=0;
	};

	CommandTextEdit(QWidget* parent = nullptr);
	virtual ~CommandTextEdit();

	void setCommandParser(const CommandParser::Parser& parser);
	void clear();
	CursorPosition getCursorPositionRaw() const;
	size_t cursorPositionHtml(size_t cursorPositionRaw) const;
	virtual void keyPressEvent(QKeyEvent* event) override;
	void setCurrentWorkspace(int currentWorkspace) {m_currentWorkspace=currentWorkspace;};
	void setCommandGuiInterface(Command::ICommandGui* commandGuiInterface) { m_commandGuiInterface=commandGuiInterface;};
public slots:
signals:
	void helpMessage(std::string message);
protected:
	virtual void focusInEvent(QFocusEvent* event) override;
private:
	void analyzeCommand(const std::string& command);
	bool eventFilter(QObject* object, QEvent* event);
	std::list<std::shared_ptr<Command::CommandBase>> m_commands;
	CommandParser::CommandText m_commandText;
	CommandParser::Parser m_commandParser;
	std::string m_inputCommand;
	int m_currentWorkspace=-1;
	Command::ICommandGui* m_commandGuiInterface=0;
private slots:
	void cursorPositionChangeEvent();
};

#endif
