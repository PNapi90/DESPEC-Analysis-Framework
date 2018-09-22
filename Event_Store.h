#ifndef EVENT_STORE_H
#define EVENT_STORE_H

#include <iostream>
#include <cstdlib>
#include <cmath>

#include "Raw_Event.h"

#include "Events.h"

//#include "FRS_Event.h"
//#include "AIDA_Event.h"
#include "PLASTIC_Event.h"
#include "FATIMA_Event.h"
#include "GALILEO_Event.h"
//#include "FINGER_Event.h"


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

	Events*** Event;
	ULong64_t** Event_WR;
	int*** Event_position;
    
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

	inline bool in_time_windows(double);

    //-------------------
    TH1D* T_DIFF;
    TH1D* T_DIFF_Fine;
    TH2D *Emat,*Egal_o_t;
    TH1D *Efat,*Egal,*Efat_C,*Fine_T2;
    TH2D** det_vs_t;
    TH2D*** e_over_t;
    TFile* TFILE;
    ULong64_t tgal,tfat;
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
    void Write_Energies(int,int);
    
    void reset_Iter();

    int get_Match_ID(int,int,int);
    int Time_Comparison(int,ULong64_t);

	int* get_position(int);

};


#endif
