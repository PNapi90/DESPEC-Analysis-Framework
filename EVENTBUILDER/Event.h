#ifndef EVENT_H
#define EVENT_H

#include "Raw_Event.h"

typedef unsigned long long ULong64_t;

class Event{

private:

    int AIDA_X,AIDA_Y,AIDA_Z;
    double A_over_Q;

    ULong64_t AIDA_TimeStamp;


public:
    Event(int*);
    ~Event();

    void set_FRS_DATA(Raw_Event*);
    void write_Event();
};



#endif