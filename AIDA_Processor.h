#ifndef AIDA_PROCESSOR_H
#define AIDA_PROCESSOR_H

#include <thread>
#include <iostream>


#include "TX_Matrix.h"
#include "XY_Matrix.h"
#include "Raw_Event.h"
#include "AIDA_Data_Streamer.h"
#include "AIDA_Decay_Event_Store.h"


typedef unsigned long long ULong64_t;

class AIDA_Processor{
private:

    const bool TX_CALC = true;
    const bool XY_CALC = false;

    bool USE_THREADS;

    int amount_z_strips,amount_thr,am_sub_threads;

    AIDA_Data_Streamer* Stream;

    TX_Matrix** TX;
    XY_Matrix** XY;

    std::thread threading(bool,int);
    inline void non_threading(bool,int);

public:
    AIDA_Processor(int);
    ~AIDA_Processor();
    
    void PROCESSING(AIDA_Decay_Event_Store*);
    void Store_and_Purge(bool*,int*,ULong64_t*,double*,int*,int);
    void get_DATA(Raw_Event*);
};

#endif