#ifndef OBJECTMANAGEROBJLIST_H
#define OBJECTMANAGEROBJLIST_H

#include "../objectmanagernotifierregistration.h"

#include <map>
#include <memory>
#include <vector>
#include <tinyxml.h>

template <typename T>
class ObjectManagerObjList
{
public:
	ObjectManagerObjList()
	{
	}
	virtual ~ObjectManagerObjList()
	{
	}

	std::weak_ptr<T> getObjectByInternalId(int internalId) const
	{
		typename std::map<int, std::shared_ptr<T> >::const_iterator searchIt=m_objectsByInternalId.find(internalId);
		if (searchIt==m_objectsByInternalId.end())
			return std::weak_ptr<T>();
		return searchIt->second;
	}

	std::weak_ptr<T> getObjectByExternalId(int externalId) const
	{
		typename std::map<int, std::shared_ptr<T> >::const_iterator searchIt=m_objectsByExternalId.find(externalId);
		if (searchIt==m_objectsByExternalId.end())
			return std::weak_ptr<T>();
		return searchIt->second;
	}

	bool publishObject(std::shared_ptr<T> object)
	{
		if (object->getInternalId()>=0)
		{
		   	if (m_objectsByInternalId.find(object->getInternalId())!=m_objectsByInternalId.end())
				return false;
			m_internalIdCounter=std::max(m_internalIdCounter, object->getInternalId());
		}
		if (object->getInternalId()==-1)
			object->setInternalId(++m_internalIdCounter);
		m_objectsByInternalId[object->getInternalId()]=object;
		if (object->getExternalId()==-1)
		{
			auto itLast=m_objectsByExternalId.end();
			if (itLast==m_objectsByExternalId.begin())
				object->setExternalId(1);
			else
			{
				--itLast;
				object->setExternalId(itLast->first+1);
			}
		}
		m_objectsByExternalId[object->getExternalId()]=object;
		for(ObjectManagerNotifierRegistration registration : m_notifierList)
		{
			if (registration.publishNotifier)
				registration.publishNotifier(object->getInternalId());
		}
		return true;
	}

	void removeObject(int internalId)
	{
		m_objectsByInternalId.erase(internalId);
		rebuildExternalMap();
		for(ObjectManagerNotifierRegistration registration : m_notifierList)
		{
			if (registration.removeNotifier)
				registration.removeNotifier(internalId);
		}
	}

	void objectHasUpdated(int internalId)
	{
		rebuildExternalMap();
		for(ObjectManagerNotifierRegistration registration : m_notifierList)
		{
			if (registration.stateChangeNotifier)
				registration.stateChangeNotifier(internalId);
		}
	}

	//TODO eigentlich wollen wir nur weak_ptr rausgeben...
	std::map<int, std::shared_ptr<T> > getMapByInternalId() const { return m_objectsByInternalId; }; 
	std::map<int, std::shared_ptr<T> > getMapByExternalId() const { return m_objectsByExternalId; }; 

	void registerNotification(ObjectManagerNotifierRegistration registration)
	{
		m_notifierList.push_back(registration);
	}

	bool loadObjectsFromXml(const TiXmlElement* elementsToLoad, const std::string& elementName, std::string& errorMessage)
	{
		bool hasError=false;
		const TiXmlElement* elementIterator=elementsToLoad->FirstChildElement(elementName);
		while(elementIterator)
		{
			std::shared_ptr<T> newObject(new T);
			std::string tmpError;
			if (newObject->loadObjectFromXml(elementIterator, tmpError))
			{
				publishObject(newObject);
			}
			else
			{
				errorMessage+=tmpError;
				hasError=true;
			}
			elementIterator=elementIterator->NextSiblingElement(elementName);
		}
		return !hasError;
	}

	void saveObjectsAsXml(TiXmlElement* elementToSaveTo, const std::string& elementName) const
	{
		for(const std::pair<int, std::shared_ptr<T> >& object : m_objectsByInternalId)
		{
			TiXmlElement* element=new TiXmlElement(elementName);
			object.second->saveObjectAsXml(element);
			elementToSaveTo->LinkEndChild(element);
		}
	}

private:
	void rebuildExternalMap()
	{
		m_objectsByExternalId.clear();
		for(std::pair<const int, std::shared_ptr<T> >& object : m_objectsByInternalId)
		{
			m_objectsByExternalId[object.second->getExternalId()]=object.second;
		}
	}
	int m_internalIdCounter=0;
	std::map<int, std::shared_ptr<T> > m_objectsByInternalId;
	std::map<int, std::shared_ptr<T> > m_objectsByExternalId;
	std::vector<ObjectManagerNotifierRegistration> m_notifierList;
};

#endif
