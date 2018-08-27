#ifndef AIDA_PROCESSOR_H
#define AIDA_PROCESSOR_H

#include <thread>
#include <iostream>


#include "TX_Matrix.h"
#include "XY_Matrix.h"
#include "Raw_Event.h"
#include "AIDA_Data_Streamer.h"


typedef unsigned long long ULong64_t;

class AIDA_Processor{
private:

    int amount_z_strips,amount_thr;

    AIDA_Data_Streamer* Stream;

    TX_Matrix** TX;
    XY_Matrix** XY;


public:
    AIDA_Processor(int);
    ~AIDA_Processor();
    
    void PROCESSING(bool*,int*,ULong64_t*,double*,int*,int);
    void get_DATA(Raw_Event*);
};

#endif