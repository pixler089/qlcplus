#include "vcprogrammerselectedobjects.h"


VcProgrammerSelectedObjects::VcProgrammerSelectedObjects()
{
}

VcProgrammerSelectedObjects::~VcProgrammerSelectedObjects()
{
}

VcProgrammerSelectedObjects VcProgrammerSelectedObjects::createRange(int startID, int endID)
{
	VcProgrammerSelectedObjects objects;
	if (startID<=endID)
	{
		for (int object=startID;object<=endID;object++)
		{
			objects.addObject(object);
		}
	}
	else
	{
		for (int object=startID;object>=endID;object--)
		{
			objects.addObject(object);
		}
	}
	return objects;
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

void VcProgrammerSelectedObjects::setRotation(int rotationStart, int rotationSize)
{
	int increment=1;
	if (rotationSize<0)
	{
		rotationSize=-rotationSize;
		increment=-1;
	}
	int currentRotationPosition=rotationStart%rotationSize;
	for(ObjectData& obj : m_objects)
	{
		obj.m_rotationPosition=currentRotationPosition;
		obj.m_rotationSize=rotationSize;
		currentRotationPosition=(currentRotationPosition+increment)%rotationStart;
	}
}

VcProgrammerSelectedObjects& VcProgrammerSelectedObjects::operator+=(const VcProgrammerSelectedObjects& rhs)
{
	for ( const auto& object : rhs)
	{
		m_objects.push_back(object);
	}
	std::ignore=rhs;
	return *this;
}

VcProgrammerSelectedObjects& VcProgrammerSelectedObjects::operator-=(const VcProgrammerSelectedObjects& rhs)
{

	for ( const auto& object : rhs)
	{
		m_objects.remove(object);
	}
	std::ignore=rhs;
	return *this;
}
