#ifndef AIDA_DETECTOR_SYSTEM_H
#define AIDA_DETECTOR_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <stdlib.h>
#include <cstdlib>

#include <TFile.h>
#include <TH1.h>

#include "AIDA_Headers.h"
#include "AIDA_Data_Types.h"

//#include "AIDA_Decay_Event_Store.h"
//#include "AIDA_Processor.h"

#include "Detector_System.cxx"

typedef unsigned long long ULong64_t;

class AIDA_Detector_System : public Detector_System{

private:

	const int num_FEE64s = 24;		//Not just the number in use but the highest number ID that is used
	const int num_DSSSDs = 6;		//Total number of DSSD you are implanting in
	const int num_channels = 64;		//Will remain fixed. Number of channels per FEE
	const int masterFEE64 = 6;
	const double adcZero = 32768; 	//2**15
	
	
	Int_t* pdata;
	Int_t* pdata_start;
    
	Int_t lwords;
                
	Int_t sub_evt_length;
	
	int AIDA_counter;
	
	int tmp_x, tmp_y, tmp_z;
	int tmp_stopping_layer;
	bool x_check, y_check;
	
	int tmp_FEE64ID;      //FEE64 ID

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
	
	ADCDataItem decayItem;
	ADCDataItem implantItem;
	
	int* feeChannelOrder; /*= {62, 63, 59, 60, 61, 56, 57, 58, 52, 53, 54, 55, 49, 50, 51, 45,
			  46, 47, 48, 42, 43, 44, 38, 39, 40, 41, 35, 36, 37, 31, 32, 33,
			  34, 28, 29, 30, 24, 25, 26, 27, 21, 22, 23, 17, 18, 19, 20, 14,
			  15, 16, 10, 11, 12,  7,  3,  0,  8,  4,  1,  9,  5,  2, 13,  6};*/
			  
	int* FEE_polarity_map;
	double** channel_offsets_map;
	
	double** ADCLowEnergyGain;
	double** ADCHighEnergyGain;
	
	int** ADCItemCounts;
	ULong64_t** ADCLastTimestamp;
	
	int itemADC;
	int itemFEE;
	ULong64_t itemTimestamp;
	
	void load_polarity_file();
	void load_offsets_file();
	void load_config_file();
	void load_channel_order();

	void get_position_data(ADCDataItem&);
	
	void Pause_Timestamp(AIDA_Time_First*);
	void Resume_Timestamp(AIDA_Time_First*);
	void Set_AIDA_Timestamp();
	void Set_AIDA_Implantation(AIDA_ADC_1*);
	void Unpack_AIDA_Decay_DATA(AIDA_ADC_1*);
	void Check_AIDA_Disc_DATA();
	
	void CorrectMultiplexer(ADCDataItem&);

	//Decay Event Building
	//AIDA_Processor* Processor;
	//AIDA_Decay_Event_Store* AIDA_Store;
	
	//void load_max_len_file(int*);



public:

    AIDA_Detector_System();
	~AIDA_Detector_System();
	
	
	//void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};
	void Process_MBS(TGo4MbsSubEvent* psubevt);

	
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
