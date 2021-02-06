#include "channeltype.h"

ChannelType getChannelTypeFromText(const std::string& channelType)
{
	if (channelType=="brightness")
		return ChannelType::Brightness;
	if (channelType=="red")
		return ChannelType::Red;
	if (channelType=="green")
		return ChannelType::Green;
	if (channelType=="blue")
		return ChannelType::Blue;
	if (channelType=="white")
		return ChannelType::White;
	if (channelType=="amber")
		return ChannelType::Amber;
	if (channelType=="uv")
		return ChannelType::UV;
	if (channelType=="strobe")
		return ChannelType::Strobe;
	if (channelType=="zoom")
		return ChannelType::Zoom;
	return ChannelType::Unknown;
}

std::string getReadableChannelDescr(ChannelType channelType)
{
	switch (channelType)
	{
		case(ChannelType::Unknown):
			return "<unknown>";
		case(ChannelType::Brightness):
			return "Brightness";
		case(ChannelType::Red):
			return "Red";
		case(ChannelType::Green):
			return "Green";
		case(ChannelType::Blue):
			return "Blue";
		case(ChannelType::White):
			return "White";
		case(ChannelType::Amber):
			return "Amber";
		case(ChannelType::UV):
			return "Uv";
		case(ChannelType::Strobe):
			return "Strobe";
		case(ChannelType::Zoom):
			return "Zoom";
		default: 
			return "<TODO>";
	}
}
