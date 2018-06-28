#ifndef FATIMA_DET_H
#define FATIMA_DET_H

#include "Data_Stream.cxx"
#include "FATIMA_Data_Stream.h"
#include "Detector_System.cxx"
#include "QDC_TDC_FATIMA.h"
#include "Raw_Event.h"

class FATIMA_Detector_System : public Detector_System{

private:

	int* pdata;

	int increase;
    int am_QDCs,am_TDCs,fired_amount;

    QDC_TDC_FATIMA* QDC_TDC;

public:
    FATIMA_Detector_System();
    ~FATIMA_Detector_System();

    //functions from abstract class Detector_System
    void Process_MBS(int*);
    void get_Event_data(Raw_Event*);
    void get_Event_data(Data_Stream*);


    int* get_pdata();
    
    unsigned long*** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int*** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;}
};


#endif
