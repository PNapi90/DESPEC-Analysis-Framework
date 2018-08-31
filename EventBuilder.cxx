#ifndef EVENTBUILDER_H
#define EVENTBUILDER_H

#include "Raw_Event.h"

class EventBuilder{

public:
	
	virtual ~EventBuilder(){};

	virtual void set_Event(Raw_Event*) = 0;

};

#endif
