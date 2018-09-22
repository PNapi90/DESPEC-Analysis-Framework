#ifndef GALILEO_EVENT_H
#define GALILEO_EVENT_H

#include <iostream>
#include <cstdlib>

#include "Events.h"
#include "Raw_Event.h"

typedef unsigned long ULong;

class GALILEO_Event : public Events{

private:
    
    int am_fired;
    int det_id[3];
    double energy_tot;
    double energies[3];
    ULong ch_time[3];

    inline void set_DATA(Raw_Event*);


public:
    GALILEO_Event(int*,int,Raw_Event*);
    virtual ~GALILEO_Event();
    
    double get_energy();
    double get_energies(int);
    ULong get_time(int);
};

#endif
