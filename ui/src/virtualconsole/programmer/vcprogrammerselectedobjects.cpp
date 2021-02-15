#include "vcprogrammerselectedobjects.h"


VcProgrammerSelectedObjects::VcProgrammerSelectedObjects()
{
}

VcProgrammerSelectedObjects::~VcProgrammerSelectedObjects()
{
}

void VcProgrammerSelectedObjects::addObject(int objectID, int rotationPosition, int rotationSize)
{
	m_objects.push_back(ObjectData{objectID, rotationPosition, rotationSize});
}

std::list<int> VcProgrammerSelectedObjects::getCurrentObjects() const
{
	std::list<int> currentObjects;
	for(const ObjectData& obj : m_objects)
	{
		if (obj.m_rotationPosition==0)
		{
			currentObjects.push_back(obj.m_objectID);
		}
	}
	return currentObjects;
}

void VcProgrammerSelectedObjects::stepRight()
{
	for(ObjectData& obj : m_objects)
	{
		if (obj.m_rotationSize==0)
		{
			continue;
		}
		obj.m_rotationPosition=(obj.m_rotationSize+1)%obj.m_rotationSize;
	}
}

void VcProgrammerSelectedObjects::stepLeft()
{
	for(ObjectData& obj : m_objects)
	{
		if (obj.m_rotationSize==0)
		{
			continue;
		}
		obj.m_rotationPosition=(obj.m_rotationSize-1)%obj.m_rotationSize;
	}
}

VcProgrammerSelectedObjects& VcProgrammerSelectedObjects::operator+=(const VcProgrammerSelectedObjects& rhs)
{
	std::ignore=rhs;
	return *this;
}

VcProgrammerSelectedObjects& VcProgrammerSelectedObjects::operator-=(const VcProgrammerSelectedObjects& rhs)
{
	std::ignore=rhs;
	return *this;
}
