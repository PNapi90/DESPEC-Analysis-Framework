#ifndef FATIMA_DET_H
#define FATIMA_DET_H

#include "Detector_System.cxx"
#include "QDC_TDC_FATIMA.h"

class FATIMA_Detector_System : public Detector_System{

private:

    int am_QDCs,am_TDCs,fired_amount;

    QDC_TDC_FATIMA* QDC_TDC;

    double** data_stream;    

public:
    FATIMA_Detector_System();
    ~FATIMA_Detector_System();

    //functions from abstract class Detector_System
    void Process_MBS(int*);
    double** get_Event_data();
    
};


#endif