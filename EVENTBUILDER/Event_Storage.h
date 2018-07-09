#ifndef EVENT_STORAGE_H
#define EVENT_STORAGE_H

#include <iostream>
#include <cstdlib>

#include "Raw_Event.h"
#include "Event.h"

typedef unsigned long long ULong64_t;

class Event_Storage{

private:

    int type,N_X,N_Y;

    int*** AIDA_iterator;

    ULong64_t Time_Array[5][];

    Event***** Events;

    void create_Event(int,int*);

public:
    Event_Storage();
    ~Event_Storage();

    save_Raw_Event(Raw_Event*,int);
    save_Raw_Event(Raw_Event*,int,ULong64_t);
    
};

#endif