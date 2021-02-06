#ifndef GUI_ICOMMANDGUI_H
#define GUI_ICOMMANDGUI_H

namespace Command
{

class ICommandGui
{
public:
	virtual void setCurrentWorkspace(int workspaceId)=0;
};

}

#endif
