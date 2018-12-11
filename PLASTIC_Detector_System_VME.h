#ifndef PLASTIC_VME_DETECTOR_SYSTEM_H
#define PLASTIC_VME_DETECTOR_SYSTEM_H

#include <iostream>
#include <string>
#include <vector>

#include "Detector_System.cxx"
#include "Raw_Event.h"


typedef unsigned long long ULong64_t;
typedef unsigned long ULong;

class PLASTIC_VME_Detector_System : public Detector_System{


private:

    // ================= THE SUDIPTA VME VARIABLES ================== //
    
    
    int fthr; //100
    
    int *pl_data,diff,first_e_sample;

    char exit_flg;

    long raw_data, energy_data, first_lw;

    long card, chan, rchan, samples, energy, trg_cnt, ft_cnt, trailer;

    long hit_pattern, nof_hits, i;

    long raw_data_old;

    unsigned short *pl_data16, first_sample;

    char index_pat;
  
    long base_sum, base2_sum, top_sum;

    char pi_flg, rt_flg;
  
    int value, value_t, chNo,TDC_iterator;
    
    // ===== THE ONE'S I ADDED ======== //
    
    std::vector<double> VME_QDC_Data;
    std::vector<double> VME_QDC_Channels;
    std::vector<double> VME_TDC_Data;
    std::vector<double> VME_TDC_Channels;


public:
    PLASTIC_VME_Detector_System();
    ~PLASTIC_VME_Detector_System();


    void Process_MBS(TGo4MbsSubEvent* psubevt);

    //functions from abstract class Detector_System
    void Process_MBS(int*){return};

    void get_Event_data(Raw_Event*);

    int* get_pdata(){return nullptr;};
    
    bool calibration_done(){return false;}

    void write(){return;};
    void set_Gain_Match_Filename(std::string){return;};

};



#endif

