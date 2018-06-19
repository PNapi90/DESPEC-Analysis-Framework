#ifndef PLASTIC_DETECTOR_SYSTEM_H
#define PLASTIC_DETECTOR_SYSTEM_H

#include <iostream>

#include "Detector_System.cxx"
#include "TAMEX.h"


typedef unsigned long long ULong64_t;


class PLASTIC_Detector_System : public Detector_System{


private:

    const int add = 2781;
    const int aa = 170;

    double** data_stream;

    TAMEX* tamex;



public:
    PLASTIC_Detector_System();
    ~PLASTIC_Detector_System();

    //functions from abstract class Detector_System
    void Process_MBS(int*);
    double** get_Event_data();
};



#endif