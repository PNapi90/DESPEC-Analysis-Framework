#ifndef PLASTIC_EVENT_H
#define PLASTIC_EVENT_H

#include <iostream>
#include <cstdlib>

#include "Events.h"
#include "Raw_Event.h"

class PLASTIC_Event : public Events{

private:

    int FAT_FIRED;

    inline void set_DATA(Raw_Event*);


public:
    PLASTIC_Event(int*,int,Raw_Event*);
    virtual ~PLASTIC_Event();
    
    double get_energy(){return 0;};
    double get_energies(int){return 0;};
    ULong get_time(int){return 0;};
};

#endif
