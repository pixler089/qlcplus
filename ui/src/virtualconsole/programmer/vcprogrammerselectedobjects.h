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
	};

	VcProgrammerSelectedObjects();
	virtual ~VcProgrammerSelectedObjects();

	void addObject(int objectID, int rotationPosition, int rotationSize);
	std::list<int> getCurrentObjects() const;

	void stepRight();
	void stepLeft();
	VcProgrammerSelectedObjects& operator+=(const VcProgrammerSelectedObjects& rhs);
	VcProgrammerSelectedObjects& operator-=(const VcProgrammerSelectedObjects& rhs);
private:
	std::list<ObjectData> m_objects;
};

#endif
