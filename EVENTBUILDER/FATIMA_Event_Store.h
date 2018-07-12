#ifndef FATIMA_EVENT_STORE_H
#define FATIMA_EVENT_STORE_H

#include <iostream>
#include <cstdlib>

#include "Event_Store.cxx"
#include "Raw_Event.h"
#include "FATIMA_Event.h"

typedef unsigned long long ULong64_t;



class FATIMA_Event_Store : public Event_Store{

private:

	FATIMA_Event** Event;

	double** Event_WR;

	int event_counter;
	int MEMORY_LIMIT;



public:
	FATIMA_Event_Store();
	~FATIMA_Event_Store();
	

	void store(Raw_Event*);
	void get(int);
	void purge(int);
	void Time_Comparison(ULong64_t);


};


#endif