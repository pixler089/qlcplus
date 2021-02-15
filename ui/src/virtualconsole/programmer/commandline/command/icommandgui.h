#ifndef GUI_ICOMMANDGUI_H
#define GUI_ICOMMANDGUI_H

#include "virtualconsole/programmer/vcprogrammerselectedobjects.h"

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
	virtual void commandSetBrightness(uint8_t brightnessDmxValue)
	{
		std::ignore=brightnessDmxValue;
	}
};

}

#endif
