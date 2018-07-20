#ifndef EVENT_STORE_H
#define EVENT_STORE_H

#include <iostream>
#include <cstdlib>

#include "Raw_Event.h"

#include "Events.cxx"

//#include "FRS_Event.h"
//#include "AIDA_Event.h"
#include "PLASTIC_Event.h"
#include "FATIMA_Event.h"
//#include "GALILEO_Event.h"
//#include "FINGER_Event.h"


typedef unsigned long long ULong64_t;



class Event_Store{

private:

	const int MEMORY_LIMIT = 200000;

	Events*** Event;

	double** Event_WR;
	int** Event_position;

    int amount_interests;
    int* length_array;
    int** interest_array

	int event_counter[6];
    
    int iter[6];
    int* sys_interest;

	void purge(int,int);
	void create_Event(int,Raw_Event*);
    void set_Det_Sys_specific_coincidences();

	inline bool in_time_windows(double);

public:
	Event_Store(int,int*,int**);
	~Event_Store();
	

	void set_permission(int,int*,int);
	void Full_Permission(int,int*);

	bool compare_match_ID(int,int*,int*);

	void store(Raw_Event*);
	void Time_Comparison(int,ULong64_t);
	void set_Match_ID_address(int,int*,int*);

	int* get_position(int);




};


#endif