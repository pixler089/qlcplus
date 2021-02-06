#include "objectmanager.h"

#include "workspace/workspace.h"


ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

bool ObjectManager::workspacePublish(std::shared_ptr<Workspace> workspace)
{
	return m_workspaces.publishObject(workspace);
}

void ObjectManager::workspaceRemove(int internalId)
{
	m_workspaces.removeObject(internalId);
}

std::weak_ptr<Workspace> ObjectManager::workspaceGetByInternalId(int id) const
{
	return m_workspaces.getObjectByInternalId(id);
}

std::weak_ptr<Workspace> ObjectManager::workspaceGetByExternalId(int id) const
{
	return m_workspaces.getObjectByExternalId(id);
}


//TODO cases schoener machen (auch obere) - nur noch eine switch-case-Funktion
void ObjectManager::registerNotification(Object::Type type, ObjectManagerNotifierRegistration registration)
{
	switch(type)
	{
		case Object::Type::Workspace:
			m_workspaces.registerNotification(registration);
			break;
		default: 
			break;
	}
}

