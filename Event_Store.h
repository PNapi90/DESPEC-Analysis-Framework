#ifndef EVENT_STORE_H
#define EVENT_STORE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "Raw_Event.h"

#include "Events.h"
#include "Match.h"

#include "FRS_Event.h"
#include "AIDA_Event.h"
#include "PLASTIC_Event.h"
#include "FATIMA_Event.h"
#include "GALILEO_Event.h"
#include "FINGER_Event.h"

#include "FRS_DataStruct.h"
#include "AIDA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"
#include "FATIMA_DataStruct.h"
#include "GALILEO_DataStruct.h"
#include "FINGER_DataStruct.h"

#include "ProcessDef.h"

#include "Tree_Creator.h"

#include "EventProcessor.cxx"
#include "PL_FAT_EventProcessor.h"


#include <TH1.h>
#include <TH2.h>
#include <TFile.h>


typedef unsigned long long ULong64_t;



class Event_Store{

private:

    const int MEMORY_LIMIT = 200000;

    int* Address_arr;
    int* Max_Fill;

    int** Fill_at;
    int* Fill_am;
    
    int iter_tmp,internal_iter;
    double E_save[2];
    double e_gali,fat_e;
    
    int ProcessAmount;

    bool VME_or_TAMEX;

    Events*** Event;
    ULong64_t** Event_WR;
    int*** Event_position;
    

    EventProcessor** PROCESSORS;
    ProcessDef** Process_Type;
    
    int amount_interest,ev_pos;
    int* tmp_pos;
    int* length_array;
    int* length_interest;
    int** interest_array;

    int event_counter[6];
    
    int iter[6];
    int** sys_interest;

    void purge(int,int);
    void create_Event(int,Raw_Event*);
    void set_Det_Sys_specific_coincidences();

    void FILL_PROCESSOR(int,int,int);

    inline bool in_time_windows(double);   
    
    void GetCoincidenceMap();
    void SetProcessors();
    void ProcessCreator(int);
    
    int getProcessorType(int,int*);

    //-------------------
    void Write_Energies(int,int);

    TH1D* T_DIFF;
    TH1D* T_DIFF_Fine;
    TH2D* Emat;
    TH1D *Efat,*Egal;
    TFile* TFILE;
    bool Verbose_Write;
    //-------------------

public:
    Event_Store(int,int*,int**);
    ~Event_Store();
    

    void set_permission(int,int*,int);
    void Full_Permission(int,int);

    bool compare_match_ID(int,int*,int);

    void store(Raw_Event*);
    void set_Match_ID_address(int,int*,int*);
    void show_all_addresses(int,int);
    void show_positions(int);

    void Write(Match*,Tree_Creator*);

    int get_Match_ID(int,int,int);
    int Time_Comparison(int,ULong64_t);

    int* get_position(int);

};


#endif
