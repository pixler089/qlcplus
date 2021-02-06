#ifndef CHANNELTYPE_H
#define CHANNELTYPE_H

#include <string>

enum class ChannelType
{
	Unknown=0,
	Brightness=1,
	Red=2,
	Green=3,
	Blue=4,
	White=5,
	Amber=6,
	UV=7,
	Strobe=101,
	Zoom=202
};

ChannelType getChannelTypeFromText(const std::string& channelType);
std::string getReadableChannelDescr(ChannelType channelType);

#endif
