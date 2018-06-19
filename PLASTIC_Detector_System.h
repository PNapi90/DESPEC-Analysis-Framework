#ifndef PLASTIC_DETECTOR_SYSTEM_H
#define PLASTIC_DETECTOR_SYSTEM_H

#include <iostream>

#include "Detector_System.cxx"
#include "TAMEX.h"

#include "PLASTIC_Data_Stream.h"

typedef unsigned long long ULong64_t;


class PLASTIC_Detector_System : public Detector_System{


private:

    const int add = 2781;
    const int aa = 170;
    const int six_eight = 104
    const unsigned long trailer_code = 3138451013;

    int sfp_id;
    int trigger_type;
    int iterator;

    unsigned long Pre_Trigger_Window;
    unsigned long Post_Trigger_Window;

    unsigned long** coarse_T;
    unsigned long** fine_T;
    unsigned int** ch_ID;

    void check_error(int*);
    void check_trailer(int*);
    void get_edges(int*);
    void get_trigger(int*);
    void skip_padding(int*);


public:
    PLASTIC_Detector_System();
    ~PLASTIC_Detector_System();

    //functions from abstract class Detector_System
    void Process_MBS(int*);
    void get_Event_data(PLASTIC_Data_Stream* data_stream);
};



#endif