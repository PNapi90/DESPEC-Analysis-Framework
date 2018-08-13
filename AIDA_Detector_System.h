#ifndef AIDA_DETECTOR_SYSTEM_H
#define AIDA_DETECTOR_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <TFile.h>
#include <TH1.h>

#include "AIDA_Headers.h"

#include "Detector_System.cxx"

typedef unsigned long long ULong64_t;

class AIDA_Detector_System : public Detector_System{

private:
	
	Int_t* pdata;
	Int_t* pdata_start;
    
	Int_t lwords;
                
	Int_t sub_evt_length;
	
	double tmp_x, tmp_y, tmp_z;
	double tmp_stopping_layer;
	bool x_check, y_check;
	
	int** FEE_allocation;

	bool* check_FEE64_timestamp;

	ULong64_t tmp_AIDA_t0_0_15;
	ULong64_t tmp_AIDA_t0_16_35;
	ULong64_t tmp_AIDA_t0_36_63;
	ULong64_t AIDA_t0;
	ULong64_t AIDA_t0_base;
	
	ULong64_t tmp_ADC_Timestamp_end;
	ULong64_t ADC_Timestamp_full;

	ULong64_t tmp_AIDA_Discriminator_end;
	ULong64_t AIDA_Discriminator_full;
	
	/*int feeChannelOrder[64]={62, 63, 59, 60, 61, 56, 57, 58, 52, 53, 54, 55, 49, 50, 51, 45,
				 46, 47, 48, 42, 43, 44, 38, 39, 40, 41, 35, 36, 37, 31, 32, 33,
				 34, 28, 29, 30, 24, 25, 26, 27, 21, 22, 23, 17, 18, 19, 20, 14,
				 15, 16, 10, 11, 12,  7,  3,  0,  8,  4,  1,  9,  5,  2, 13,  6};*/
	
	
	
	void load_config_file();
	
	void get_coordinate(int, int, AIDA_ADC_1*);
	void Implant_List_Analyser();
	
	void Pause_Timestamp(AIDA_Time_First*);
	void Resume_Timestamp(AIDA_Time_First*);
	void Set_AIDA_Timestamp(AIDA_Time_First*);
	void Set_AIDA_Implantation(AIDA_ADC_1*);
	void Check_AIDA_ADC_DATA(AIDA_ADC_1*);
	void Check_AIDA_Disc_DATA();

public:

    AIDA_Detector_System();
	~AIDA_Detector_System();
	
	
	//void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};
	void Process_FRS(TGo4MbsSubEvent* psubevt){};


	void Process_AIDA(TGo4MbsSubEvent* psubevt);


	
	void Process_MBS(int*){};
	void get_Event_data(Raw_Event*);
	int* get_pdata();

	unsigned long** tmp_get_coarse_T(){return NULL;};

	int tmp_get_am_hits(){return 0;};

	unsigned int** tmp_get_chID(){return NULL;};

	int* tmp_get_iterator(){return NULL;};

	bool calibration_done(){return false;};

	void write(){return;};
    
	void set_Gain_Match_Filename(std::string){return;};

};



#endif
