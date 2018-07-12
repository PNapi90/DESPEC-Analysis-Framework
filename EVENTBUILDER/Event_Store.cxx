#ifndef EVENT_STORE_H
#define EVENT_STORE_H


typedef unsigned long long ULong64_t;

class Event_Store{

public:
	
	virtual ~Event_Store() = 0;

	virtual void store(Raw_Event*) = 0;

	virtual void get(int) = 0;

	virtual void purge(int) = 0;

	virtual void Time_Comparison(ULong64_t);
	
};


#endif