#ifndef PLASTIC_DETECTOR_SYSTEM_H
#define PLASTIC_DETECTOR_SYSTEM_H

#include <iostream>

#include "Detector_System.cxx"
#include "Data_Stream.h"
#include "PLASTIC_Calibrator.h"

#include "TAMEX.h"



typedef unsigned long long ULong64_t;


class PLASTIC_Detector_System : public Detector_System{


private:

    PLASTIC_Calibrator* PLASTIC_Calibration; 

    bool tamex_end;

    bool no_edges[100];

    int* pdata;

    int increase;
    int add;
    int aa;
    int six_eight,six_f;
    int error_code;
    int tamex_identifier;

    unsigned long trailer_code;

    int am_fired[100];
    int sfp_id[100];
    int trigger_type[100];
    int iterator[100];
    int tamex_id[100];

    int tamex_iter;

    unsigned long Pre_Trigger_Window;
    unsigned long Post_Trigger_Window;

    unsigned long*** coarse_T;
    unsigned long*** fine_T;
    unsigned int*** ch_ID;

    unsigned long* coarse_T_Trigger;
    unsigned long* fine_T_Trigger;

    void check_error();
    void check_trailer();
    void get_edges();
    void get_trigger();
    void skip_padding();
    void Process_TAMEX();
    void calibrate();

public:
    PLASTIC_Detector_System();
    ~PLASTIC_Detector_System();

    //functions from abstract class Detector_System
    void Process_MBS(int*);

    void get_Event_data(Data_Stream*);

    int* get_pdata();

};



#endif
