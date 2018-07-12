#ifndef TIME_EVENTBUILDER_H
#define TIME_EVENTBUILDER_H

#include <cstdlib> 

#include "Raw_Event.h"
#include "EventBuilder.cxx"

class Time_EventBuilder : public EventBuilder{

private:

	int type_of_interest;


	double*** Time_Difference_Array;


public:
	Time_EventBuilder(int);
	~Time_EventBuilder();
	
	void set_Event(Raw_Event*);
};

#endif