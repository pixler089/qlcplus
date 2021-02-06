#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <memory>
#include <map>
#include <list>

#include <iostream>

#include "object.h"
#include "private/objectmanagerobjlist.h"
#include "objectmanagernotifierregistration.h"

class Workspace;
class Universe;

class ObjectManager
{
public:
	static ObjectManager& instance()
	{
		static ObjectManager instance;
		return instance;
	}

	std::weak_ptr<Object> getObjectByInternalId(Object::Type type, int id);	
	std::weak_ptr<Object> getObjectByExternalId(Object::Type type, int id);	

	///@return true on succes, false on failure (for example if workspace-ID already exists)
	bool workspacePublish(std::shared_ptr<Workspace> workspace);
	void workspaceRemove(int internalId);
	std::weak_ptr<Workspace> workspaceGetByInternalId(int id) const;
	std::weak_ptr<Workspace> workspaceGetByExternalId(int id) const;
	std::map<int, std::shared_ptr<Workspace> > workspacesGetAllByInternalId() const { return m_workspaces.getMapByInternalId(); };

	void registerNotification(Object::Type type, ObjectManagerNotifierRegistration registration);
private:
	ObjectManagerObjList<Workspace> m_workspaces;

////////////////////////////
////// Internal ////////////
////////////////////////////
public:
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager(ObjectManager&&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;
	ObjectManager& operator=(ObjectManager&&) = delete;
protected:
	ObjectManager();
	virtual ~ObjectManager();
};

#endif
