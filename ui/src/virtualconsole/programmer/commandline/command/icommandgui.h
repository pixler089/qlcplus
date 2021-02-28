#ifndef GUI_ICOMMANDGUI_H
#define GUI_ICOMMANDGUI_H

#include "virtualconsole/programmer/vcprogrammerselectedobjects.h"
#include "qlcchannel.h"

#include <memory>

namespace Command
{

class ICommandGui
{
public:
	virtual void commandSetCurrentWorkspace(int workspaceId)
	{
		std::ignore=workspaceId;
	};
	virtual void commandSetSelectedFixtures(VcProgrammerSelectedObjects&& objects)
	{
		std::ignore=objects;
	}
	virtual void commandSetChannel(QLCChannel::Preset channelType, uint8_t dmxValue, uint8_t dmxValueFine=0)
	{
		std::ignore=channelType;
		std::ignore=dmxValue;
		std::ignore=dmxValueFine;
	}
	virtual void commandClearAll()
	{
	}
	virtual void commandClearSelected()
	{
	}
	virtual void commandRotateSelection()
	{
	}
};

}

#endif
