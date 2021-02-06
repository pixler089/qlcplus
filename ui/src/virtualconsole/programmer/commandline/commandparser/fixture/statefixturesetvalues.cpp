#include "statefixturesetvalues.h"

#include "../commandtext.h"

using namespace CommandParser;

StateFixtureSetValues::StateFixtureSetValues()
{
}

StateFixtureSetValues::~StateFixtureSetValues()
{
}

std::string StateFixtureSetValues::getColorString(StateFixtureSetValues::ColorState colorState) const
{
	switch (colorState)
	{
		case ColorState::Red:
			return "Red";
		case ColorState::Green:
			return "Green";
		case ColorState::Blue:
			return "Blue";
		case ColorState::White:
			return "White";
		case ColorState::Amber:
			return "Amber";
		case ColorState::UV:
			return "UV";
		default:
			return "";
	}
}

void StateFixtureSetValues::clear()
{
	m_internalState=InternalState::Start;
	m_brightnessString.clear();
	m_colorPartState=ColorState::Red;
}

void StateFixtureSetValues::finish(CommandText& formattedCommandText)
{
	std::ignore=formattedCommandText;
}

bool StateFixtureSetValues::parseChar(char newChar, CommandText& formattedCommandText)
{
	if (newChar==',')
	{
		if (m_internalState==InternalState::Start)
		{
			m_internalState=InternalState::Brightness;
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
			return true;
		}
		if (m_internalState==InternalState::Brightness)
		{
			m_internalState=InternalState::Color;
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
			return true;
		}
		if (m_internalState==InternalState::Color)
		{
			m_internalState=InternalState::Flags;
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
			return true;
		}
	}
	if (newChar>='0' && newChar<='9')
	{
		if (m_internalState==InternalState::Brightness)
		{
			std::string prefix;
			if (m_brightnessString.empty())
				prefix="Brightness=";
			else if (stoi(m_brightnessString+newChar)>100)
				return false;
			m_brightnessString+=newChar;
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+prefix+newChar+"</font>");
			return true;
		}
		if (m_internalState==InternalState::Color)
		{
			std::string colorName=getColorString(m_colorPartState);
			if (colorName.empty())
				return false;
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+colorName+"="+newChar+"</font> ");
			m_colorPartState=static_cast<ColorState>(static_cast<int>(m_colorPartState)+1); // TODO dirty hack...
			return true;
		}
	}
	if ( (newChar>='a' && newChar <= 'z') 
			|| (newChar>='A' && newChar<='Z')
	   )
	{
		m_internalState=InternalState::Flags;
		formattedCommandText.appendRawCommandPart(newChar);
		formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
		return true;
	}
	return false;
}

std::shared_ptr<State> StateFixtureSetValues::getFollowingParserState()
{
	return nullptr;
}

void StateFixtureSetValues::returnAfterStacked(CommandText& commandText)
{
	std::ignore=commandText;
}

void StateFixtureSetValues::getHelpHintMessages(std::string& helpMessage, std::string& hintMessage) const
{
	std::ignore=helpMessage;
	switch (m_internalState)
	{
		case (InternalState::Start):
			hintMessage="[&lt;Brightness&gt;][,&lt;Color&gt;][,&lt;Flag&gt;]";
			break;
		case (InternalState::Brightness):
			if (m_brightnessString.empty())
			{
				hintMessage="<font color=\"black\">Brightness/Flag</font>";
			}
			hintMessage+="[,&lt;Color&gt;][,&lt;Flag&gt;]";
			break;
		case (InternalState::Color):
			{
				std::string colorName=getColorString(m_colorPartState);
				if (!colorName.empty())
				{
					hintMessage="<font color=\"black\">"+colorName;
					if (m_colorPartState==ColorState::Red)
					{
						hintMessage+="/Flag";
					}
					hintMessage+="</font>";
				}
				hintMessage+="[,&lt;Flag&gt;]";
				break;
			}
		default:
			hintMessage="[,&lt;Flag&gt;]";
			break;
	}
}

std::shared_ptr<Command::CommandBase> StateFixtureSetValues::getResultingCommand() const
{
	return nullptr;
}

