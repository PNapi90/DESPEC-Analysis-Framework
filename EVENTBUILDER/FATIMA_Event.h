#ifndef FATIMA_EVENT_H
#define FATIMA_EVENT_H

#include <iostream>
#include <cstdlib>

#include "Events.h"
#include "Raw_Event.h"

class FATIMA_Event : public Events{

private:

    int FAT_FIRED;

    inline void set_DATA(Raw_Event*);


public:
    FATIMA_Event(int*,int,Raw_Event*);
    ~FATIMA_Event();
};

#endif