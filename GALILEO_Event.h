#ifndef GALILEO_EVENT_H
#define GALILEO_EVENT_H

#include <iostream>
#include <cstdlib>

#include "FATIMA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"

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

    PLASTIC_DataStruct* GET_PLASTIC(){return nullptr;};
    PLASTIC_VME_DataStruct* GET_PLASTIC_VME(){return nullptr;};

    FATIMA_DataStruct* GET_FATIMA(){return nullptr;};
    FRS_DataStruct* GET_FRS(){return nullptr;};
    AIDA_DataStruct* GET_AIDA(){return nullptr;};
    GALILEO_DataStruct* GET_GALILEO(){return nullptr;};
    FINGER_DataStruct* GET_FINGER(){return nullptr;};
};

#endif
