#ifndef TIME_EVENTBUILDER_H
#define TIME_EVENTBUILDER_H

#include <cstdlib> 
#include <iostream>
#include <fstream>

#include "Raw_Event.h"
#include "EventBuilder.cxx"

#include "Event_Store.h"



class Time_EventBuilder : public EventBuilder{

private:
	

    int match_amount;
    int found_matches;

    int max_calls[6];
    bool Used_Systems[6];
    bool relevance_system[6];

    bool** relevance_array;

	int amount_interests;
	int** interest_array;
	
	Event_Store* Event_Storage;

    void create_relevance_array();

public:
	Time_EventBuilder(int,int*,int**);
	~Time_EventBuilder();
	
	void set_Event(Raw_Event*);
};

#endif