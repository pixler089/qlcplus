#include "statefixturesetvalues.h"

#include "../commandtext.h"

#include "../../command/icommandgui.h"

using namespace CommandParser;

StateFixtureSetValues::StateFixtureSetValues()
{
}

StateFixtureSetValues::~StateFixtureSetValues()
{
}

std::string StateFixtureSetValues::getColorString(StateFixtureSetValues::ColorState colorState)
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

QLCChannel::Preset StateFixtureSetValues::getChannelType(ColorState internalColorState)
{
	switch (internalColorState)
	{
		case ColorState::Red:
			return QLCChannel::IntensityRed;
		case ColorState::Green:
			return QLCChannel::IntensityGreen;
		case ColorState::Blue:
			return QLCChannel::IntensityBlue;
		case ColorState::White:
			return QLCChannel::IntensityWhite;
		case ColorState::Amber:
			return QLCChannel::IntensityAmber;
		case ColorState::UV:
			return QLCChannel::IntensityUV;
		default:
			return QLCChannel::NoFunction;
	}
}

void StateFixtureSetValues::clear()
{
	m_internalState=InternalState::Start;
	m_brightnessString.clear();
	m_colorPartState=ColorState::Red;
	m_commands.clear();
	m_errorState=false;
}

void StateFixtureSetValues::finish(CommandText& formattedCommandText)
{
	if (!m_errorState && m_brightnessString.size()>0)
		addCommand(QLCChannel::Preset::IntensityDimmer, stoi(m_brightnessString)*255/100);
	std::ignore=formattedCommandText;
}

void StateFixtureSetValues::addCommand(QLCChannel::Preset type, uint8_t dmxValue)
{
	auto func=std::function<void(Command::ICommandGui*,
	                        QLCChannel::Preset channelType, 
							uint8_t dmxValue, 
							uint8_t dmxValueFine)>
							(&Command::ICommandGui::commandSetChannel);
	auto commandObject=std::make_shared<Command::CommandBase>(Command::CommandBase::EExecuteType::GUI, Object::Type::Fixture);
	commandObject->setCommand(std::bind(func, std::placeholders::_1, type, dmxValue, 0));
	m_commands.push_back(commandObject);
}

bool StateFixtureSetValues::parseChar(char newChar, CommandText& formattedCommandText)
{
	if (newChar==',')
	{
		if (m_internalState==InternalState::Start)
		{
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
			m_internalState=InternalState::Brightness;
			return true;
		}
		if (m_internalState==InternalState::Brightness)
		{
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
			//Command will be created in finish()
			m_internalState=InternalState::Color;
			return true;
		}
		if (m_internalState==InternalState::Color)
		{
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+newChar+"</font>");
			m_internalState=InternalState::Flags;
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
			{
				m_errorState=true;
				return false;
			}
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
			int intensityPercent=(newChar-'0')*11;
			if (intensityPercent==99)
				intensityPercent=100;
			formattedCommandText.appendRawCommandPart(newChar);
			formattedCommandText.appendHtmlCommandPart(std::string("<font color=\"green\">")+colorName+"="+std::to_string(intensityPercent)+"%</font> ");
			auto qlcChannelType=getChannelType(m_colorPartState);
			if (!m_errorState)
				addCommand(qlcChannelType, intensityPercent*255/100);
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

Command::CommandBase::List StateFixtureSetValues::getResultingCommand() const
{
	return m_commands;
}

