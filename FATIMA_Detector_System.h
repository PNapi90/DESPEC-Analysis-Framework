#ifndef FATIMA_DETECTOR_SYSTEM_H
#define FATIMA_DETECTOR_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include <TFile.h>
#include <TH1.h>

#include "FATIMA_Energy_Calibration.h"
#include "FATIMA_Time_Calibration.h"
#include "FATIMA_Gain_Match.h"

#include "QDC_751.h"
#include "TDC_1290.h"

#include "Detector_System.cxx"

typedef unsigned long long ULong64_t;

class FATIMA_Detector_System : public Detector_System{

private:

    int he_iter;
	int max_am_dets;

	int* pdata;

	double* QLong;
	double* QShort;
	double* QLong_Raw;
	double* QShort_Raw;
	
	ULong64_t* QDC_Time_Coarse;
	ULong64_t* QDC_Time_Fine;
	ULong64_t* TDC_Time;

	int* det_ids_QDC;
    int* det_ids_TDC;

	int** det_ID;
	int** det_ID_TDC;

	bool exiter;
	bool no_data;
    bool QDC_DATA;
    bool gain_match_used;
    std::string gain_match_filename;
    
    int num_TDC_modules = 1;

    int Fired_QDC_Channels[100][2];
    int fired_QDC_amount,fired_TDC_amount;

	void load_board_channel_file();
	void reset_fired_channels();
	void Check_QDC_DATA(QDC_Header*);
	void Check_TDC_DATA();
	void Calibrate_QDC(int);
	void Gain_Match_QDC(int);
	void Calibrate_TDC(int);

	bool wired_QDC(int,int);
	bool wired_TDC(int,int);

    bool check_additional(int);

    FATIMA_Time_Calibration* FATIMA_T_CALIB;
    FATIMA_Energy_Calibration* FATIMA_E_CALIB;
    FATIMA_Gain_Match* FATIMA_GAIN_MATCH;

    //tmp root things
    TH1D** Det_Hist;

public:
	FATIMA_Detector_System();
	~FATIMA_Detector_System();
	
	
	void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};


	
	
	void Process_MBS(int*);
	void get_Event_data(Raw_Event*);
	int* get_pdata();

	unsigned long** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;};

    void write();
    
    void set_Gain_Match_Filename(std::string);


};



#endif
