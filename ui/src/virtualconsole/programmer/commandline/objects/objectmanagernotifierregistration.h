#ifndef OBJECTMANAGERNOTIFIERREGISTRATION_H
#define OBJECTMANAGERNOTIFIERREGISTRATION_H

#include <functional>

	struct ObjectManagerNotifierRegistration
	{
		void* registrator=0;
		///@brief will be called if a new object is registered
		///@param the id of the new object
		std::function<void(int)> publishNotifier;
		///@brief will be called if a object is deletedr
		///@param the id of the deleted object
	   	std::function<void(int)> removeNotifier;
		///@brief will be called if a object  state is changed
		///@param the id of the changed object
	   	std::function<void(int)> stateChangeNotifier;
		///@brief will be called if the id of an object changes
		///@param the old id of the object
		///@param the new id of the objects
	   	std::function<void(int, int)> idChangeNotifier;
	};

#endif
