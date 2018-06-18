#ifndef EVENT_H
#define EVENT_H 

#include "Detector_System.h"
#include "White_Rabbit.h"


class Event{

private:

    long long int amount_Raw_Events;

public:
    Event();
    ~Event();
    

    void OFFLINE_ANALYSIS();
};








#endif