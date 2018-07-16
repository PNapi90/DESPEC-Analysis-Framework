
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

	int* pdata;


	ULong64_t tmp_AIDA_t0_0_15;
	ULong64_t tmp_AIDA_t0_16_35;
	ULong64_t tmp_AIDA_t0_36_63;
	ULong64_t AIDA_t0;
	ULong64_t AIDA_t0_base;
	
	ULong64_t tmp_ADC_Timestamp_end;
	ULong64_t ADC_Timestamp_full;

	ULong64_t tmp_AIDA_Discriminator_end;
	ULong64_t AIDA_Discriminator_full;
	
	bool End_of_AIDA;

	void Check_AIDA_t0_DATA(AIDA_t0_Header*);
	void Check_AIDA_ADC_DATA();
	void Check_AIDA_Disc_DATA();

public:

    AIDA_Detector_System();
	~AIDA_Detector_System();
	
	
	//void Process_FRS(TFRSSortEvent*){};

	
	void Process_MBS(int*);
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
