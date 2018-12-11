#ifndef PLASTIC_EVENT_H
#define PLASTIC_EVENT_H

#include <iostream>
#include <cstdlib>
#include <vector>

#include "Events.h"
#include "Raw_Event.h"

#include "FATIMA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"


class PLASTIC_Event : public Events{

private:

	PLASTIC_DataStruct DATA;
    PLASTIC_VME_DataStruct DATA_VME;
    bool VME_Event;

	
    inline void set_DATA(Raw_Event*);


public:
    PLASTIC_Event(int*,int,Raw_Event*);
    virtual ~PLASTIC_Event();
    
    double get_energy(){return 0;};
    void Write_Event(Tree_Creator*);

    PLASTIC_DataStruct* GET_PLASTIC();
    PLASTIC_VME_DataStruct* GET_PLASTIC_VME();

    FATIMA_DataStruct* GET_FATIMA(){return nullptr;};


};

#endif
