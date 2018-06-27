#ifndef PLASTIC_DETECTOR_SYSTEM_H
#define PLASTIC_DETECTOR_SYSTEM_H

#include <iostream>

#include "Detector_System.cxx"
#include "PLASTIC_Data_Stream.h"
#include "PLASTIC_Calibrator.h"
#include "Data_Stream.cxx"

#include "TAMEX.h"



typedef unsigned long long ULong64_t;
typedef unsigned long ULong;

class PLASTIC_Detector_System : public Detector_System{


private:

    PLASTIC_Calibrator* PLASTIC_Calibration; 

    bool tamex_end;

    bool no_edges[100];

    bool written;

    bool CALIBRATE,Calibration_Done;
    
    int cal_count;

    int* pdata;

    int increase;
    int add;
    int aa;
    int six_eight,six_f;
    int error_code;
    int tamex_identifier;

    ULong trailer_code;

    int am_fired[100];
    int sfp_id[100];
    int trigger_type[100];
    int* iterator;
    int tamex_id[100];

    int tamex_iter;

    ULong Pre_Trigger_Window;
    ULong Post_Trigger_Window;

    ULong*** edge_coarse;
    ULong*** edge_fine;
    unsigned int*** ch_ID_edge;

    ULong* coarse_T;
    ULong* fine_T;
    unsigned int* ch_ID;

    void check_error();
    void check_trailer();
    void get_edges();
    void get_trigger();
    void skip_padding();
    void Process_TAMEX();
    void calibrate_ONLINE();
    void calibrate_OFFLINE();

    bool no_error_reached();
    

public:
    PLASTIC_Detector_System();
    ~PLASTIC_Detector_System();

    //functions from abstract class Detector_System
    void Process_MBS(int*);

    void get_Event_data(Data_Stream*);

    int* get_pdata();

    ULong*** tmp_get_coarse_T();
    int tmp_get_am_hits();

    unsigned int*** tmp_get_chID();

    int* tmp_get_iterator();

    bool calibration_done();

};



#endif
