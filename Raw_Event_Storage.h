#ifndef RAW_EVENT_STORAGE_H
#define RAW_EVENT_STORAGE_H

#include "Raw_Event.h"

typedef unsigned long long ULong64_t;

class Raw_Event_Storage{

private:

	int capacity;

	int* sub_event_type;
	ULong64_t* White_Rabbit_Storage;


public:
	Raw_Event_Storage();
	~Raw_Event_Storage();
	
};



#endif