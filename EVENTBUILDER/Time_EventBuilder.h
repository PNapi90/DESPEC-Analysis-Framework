#ifndef TIME_EVENTBUILDER_H
#define TIME_EVENTBUILDER_H

#include <cstdlib> 
#include <iostream>
#include <fstream>

#include "Raw_Event.h"
#include "EventBuilder.cxx"

#include "Event_Store.cxx"

#include "FATIMA_Event_Store.h"
#include "PLASTIC_Event_Store.h"

class Time_EventBuilder : public EventBuilder{

private:
	
	bool Used_Systems[6];

	int amount_interests;
	int** interest_array;
	
	Event_Store** Event_Stores;

public:
	Time_EventBuilder(int,int**);
	~Time_EventBuilder();
	
	void set_Event(Raw_Event*);
};

#endif