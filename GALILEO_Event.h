#ifndef GALILEO_EVENT_H
#define GALILEO_EVENT_H

#include <iostream>
#include <cstdlib>

#include "Events.h"
#include "Raw_Event.h"

class GALILEO_Event : public Events{

private:
    
    int am_fired;
    double energy_tot;

    inline void set_DATA(Raw_Event*);


public:
    GALILEO_Event(int*,int,Raw_Event*);
    virtual ~GALILEO_Event();
    
    double get_energy();
    void Write_Event(Tree_Creator*);
};

#endif
