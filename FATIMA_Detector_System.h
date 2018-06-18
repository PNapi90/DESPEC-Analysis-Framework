#ifndef FATIMA_DET_H
#define FATIMA_DET_H

#include "Detector_System.cxx"
#include "White_Rabbit.h"
#include "QDC_TDC_FATIMA.h"

class FATIMA_Detector_System : public Detector_System{

private:

    unsigned long long WR_T;
    int am_QDCs,am_TDCs,fired_amount;

    White_Rabbit* WR;
    QDC_TDC_FATIMA* QDC_TDC;

    double** data_stream;    

public:
    FATIMA_Detector_System();
    ~FATIMA_Detector_System();

    void QDC_Processing(int*);
    
    double** get_Event_data();
    
};


#endif