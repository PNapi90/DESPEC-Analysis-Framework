#ifndef QDC_FATIMA_CHANNEL_H
#define QDC_FATIMA_CHANNEL_H

#include "FATIMA_Energy_Calibration.h"
#include "FATIMA_Time_Calibration.h"

#include <cstdlib>

class QDC_FATIMA_Channel{

private:

    int det_id,board_ID,Ch_Num;

    double QDC_Time[100];
    double QDC_Fine_Time[100]; // Extras //
    double QLong[100];
    double QShort[100];

    int iterator;
    bool first_call;

    double Energy[100];

    double** data_stream;

public:
    QDC_FATIMA_Channel(int,int,int);
    ~QDC_FATIMA_Channel();

    void set_QLong_Raw(double);
    void set_QShort_Raw(double);
    void set_QDC_Time(double);
    void set_QDC_Fine_Time(double); // Extras //

    void reset();

    void Calibrate(FATIMA_Energy_Calibration*,FATIMA_Time_Calibration*);
    
    double** get_Data();
};

#endif
