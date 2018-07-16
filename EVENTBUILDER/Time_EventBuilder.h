#ifndef TIME_EVENTBUILDER_H
#define TIME_EVENTBUILDER_H

#include <cstdlib> 
#include <iostream>
#include <fstream>

#include "Raw_Event.h"
#include "EventBuilder.cxx"

#include "Event_Store.cxx"

//#include "FRS_Event_Store.h"
//#include "AIDA_Event_Store.h"
#include "FATIMA_Event_Store.h"
#include "PLASTIC_Event_Store.h"
//#include "GALILEO_Event_Store.h"

class Time_EventBuilder : public EventBuilder{

private:
	

    int match_amount;
    int found_matches;

	bool Used_Systems[6];
    bool relevance_system[6];

    bool** relevance_array;

	int amount_interests;
	int** interest_array;
	
	Event_Store** Event_Stores;

    void create_relevance_array();

public:
	Time_EventBuilder(int,int*,int**);
	~Time_EventBuilder();
	
	void set_Event(Raw_Event*);
};

#endif