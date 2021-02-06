#include "object.h"

#include <tuple>

#include <iostream>

Object::Object(Object::Type type)
{
	std::ignore=type;
}

Object::~Object()
{
}

bool Object::updateObject(const Object& newDataObject)
{
	//The internal ID must not change -> do not update!
	bool hasChanges=false;
	if (newDataObject.m_bExternalIdSet)
	{
		m_externalId=newDataObject.m_externalId;
		hasChanges=true;
	}
	if (newDataObject.m_bTitleSet)
	{
		m_title=newDataObject.m_title;
		hasChanges=true;
	}
	return hasChanges;
}
