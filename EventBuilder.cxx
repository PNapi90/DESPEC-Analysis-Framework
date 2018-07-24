#ifndef EVENTBUILDER_H
#define EVENTBUILDER_H

#include "Raw_Event.h"

class EventBuilder{

public:
	
	virtual ~EventBuilder() = 0;

	virtual void set_Event(Raw_Event*) = 0;

};

#endif