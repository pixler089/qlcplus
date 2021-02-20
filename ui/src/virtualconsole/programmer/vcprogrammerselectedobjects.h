#ifndef VCPROGRAMMERSELECTEDOBJECTS_H
#define VCPROGRAMMERSELECTEDOBJECTS_H

#include <memory>
#include <list>

class VcProgrammerSelectedObjects
{
public:
	struct ObjectData
	{
		int m_objectID=0;
		//0: Object currently active
		int m_rotationPosition=0;
		//0: No rotation enabled
		int m_rotationSize=0;
		bool operator==(const ObjectData& rhs)
		{
			return (m_objectID==rhs.m_objectID);
		}
	};

	VcProgrammerSelectedObjects();
	virtual ~VcProgrammerSelectedObjects();

	static VcProgrammerSelectedObjects createRange(int startID, int endID);

	void addObject(int objectID, int rotationPosition=0, int rotationSize=0);
	std::list<int> getCurrentObjects() const;

	void stepRight();
	void stepLeft();
	void setRotation(int rotationStart, int rotationSize);
	VcProgrammerSelectedObjects& operator+=(const VcProgrammerSelectedObjects& rhs);
	friend VcProgrammerSelectedObjects operator+(VcProgrammerSelectedObjects lhs, const VcProgrammerSelectedObjects& rhs)
	{
		lhs+=rhs;
		return lhs;
	}
	VcProgrammerSelectedObjects& operator-=(const VcProgrammerSelectedObjects& rhs);
	friend VcProgrammerSelectedObjects operator-(VcProgrammerSelectedObjects lhs, const VcProgrammerSelectedObjects& rhs)
	{
		lhs-=rhs;
		return lhs;
	}

	void clear() { m_objects.clear(); };

	std::list<ObjectData>::iterator begin() { return m_objects.begin(); };
	std::list<ObjectData>::const_iterator begin() const { return m_objects.begin(); };
	std::list<ObjectData>::const_iterator cbegin() const { return m_objects.cbegin(); };
	std::list<ObjectData>::iterator end() { return m_objects.end(); };
	std::list<ObjectData>::const_iterator end() const { return m_objects.end(); };
	std::list<ObjectData>::const_iterator cend() const { return m_objects.cend(); };

	ObjectData& front() { return m_objects.front(); };
	const ObjectData& front() const { return m_objects.front(); };

	ObjectData& back() { return m_objects.back(); };
	const ObjectData& back() const { return m_objects.back(); };

	void push_back (const ObjectData& obj) { m_objects.push_back(obj); };
	void push_back (ObjectData&& obj) { m_objects.push_back(std::move(obj)); };

	size_t size() const { return m_objects.size(); };
	bool empty() const { return m_objects.empty(); };
private:
	std::list<ObjectData> m_objects;
};

#endif
