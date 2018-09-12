#ifndef AIDA_DECAY_EVENT_STORE_H
#define AIDA_DECAY_EVENT_STORE_H

#include <iostream>
#include <string>

#include "AIDA_Data_Types.h"

typedef unsigned long long ULong64_t;

class AIDA_Decay_Event_Store{
private:

    int max_len;

    ULong64_t* Time;
    
    double* Energy;
    int* z_strip;
    bool* x_or_y;
    int* x_coord;
    

    int amount;


public:
    AIDA_Decay_Event_Store(int);
    ~AIDA_Decay_Event_Store();
    
    void Store_Decay_Event(ADCDataItem*);
    void Reset();

    //getters
    int get_amount_of_events();

    double* get_Energy();
    bool* get_x_or_y();
    ULong64_t* get_Time();
    int* get_z_strip();
    int* get_x_coord();

};

#endif
