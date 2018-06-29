#ifndef FATIMA_TDC_H
#define FATIMA_TDC_H

#include "FATIMA_Time_Calibration.h"

class TDC_FATIMA_Channel{

private:

    int det_id,board_ID,Ch_num,iterator;
    bool first_call,fired;

    double TDC_Time[100];
    double Time[100];

    double** data_stream;


public:
    TDC_FATIMA_Channel(int,int,int);
    ~TDC_FATIMA_Channel();
    
    int get_Det_id();

    void set_TDC_Time(double);
    void Calibrate(FATIMA_Time_Calibration*);
    void set_TDC_Fired();

    bool get_TDC_Fired();
    void reset();

    double** get_Data();
};

#endif
