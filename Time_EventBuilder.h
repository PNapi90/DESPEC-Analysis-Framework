#ifndef TIME_EVENTBUILDER_H
#define TIME_EVENTBUILDER_H

#include <cstdlib> 
#include <iostream>
#include <fstream>

#include "Raw_Event.h"
#include "EventBuilder.cxx"

#include "Event_Store.h"
#include "Match.h"


class Time_EventBuilder : public EventBuilder{

private:

    const ULong64_t ULONG64_MAX = 0xffffffffffffffff;

	const int MEMORY_LIMIT = 1000000;

    int match_amount[100];
    int found_matches;
    int iter[6];
    int expired_counter;

    int max_calls[6];
    bool Used_Systems[6];
    bool relevance_system[6];

    bool first_event;

    bool** relevance_array;

	int amount_interest;
    int* length_interest;
	int** interest_array;
	
    Match*** Matches;
	Event_Store* Event_Storage;

    ULong64_t WR_old;

    void create_relevance_array();
    void get_used_Systems();
    void get_DELETE_Permission(int,int);

public:
	Time_EventBuilder(int,int*,int**);
	~Time_EventBuilder();
	
	void set_Event(Raw_Event*);
};

#endif