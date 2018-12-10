#ifndef FATIMA_EVENT_H
#define FATIMA_EVENT_H

#include <iostream>
#include <cstdlib>

//#include "FATIMA_Data_Struct.h"

#include "Events.h"
#include "Raw_Event.h"

class FATIMA_Event : public Events{

private:

    //FATIMA_Data DATA;

    inline void set_DATA(Raw_Event*);


public:
    FATIMA_Event(int*,int,Raw_Event*);
    virtual ~FATIMA_Event();
    
    double get_energy();
    void Write_Event(Tree_Creator* Tree);
};

#endif
