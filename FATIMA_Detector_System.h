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
	int FAT_evt;

	int* pdata;
	
	//These are indexed by fired_QDC_amount
	int fired_QDC_amount;
	int* det_ids_QDC;
	double* QLong;
	double* QLong_Raw;
	double* QShort_Raw;
	ULong64_t* QDC_Time_Coarse;
	double* QDC_Time_Fine;
	
	//These are indexed by fired_TDC_amount
	int fired_TDC_amount;
	int* det_ids_TDC;
	ULong64_t* TDC_Time_raw;		//This is the raw TDC time
	double* TDC_Time_ns;		//This is the raw TDC time


	//These are used for mapping mod,chn to detector
	int** det_ID_QDC;
	int** det_ID_TDC;

	bool exiter;
	bool no_data;
    bool QDC_DATA;
    std::string gain_match_filename;
    
    bool gain_match_used; //Set in the constructor
    int num_TDC_modules;  //Set in the constructor

	//This is used during individual module unpacking
    int Fired_QDC_Channels[100][2];
    

	void load_board_channel_file();
	void reset_fired_channels();
	void Check_QDC_DATA(QDC_Header*);
	void Check_TDC_DATA();
	void Calibrate_QDC(int);
	void Gain_Match_QDC(int);
	void Calibrate_TDC(int);

	//This could be used to activate deactivate individual
	//detectors
	bool wired_QDC(int,int);
	bool wired_TDC(int,int);

	//??
    bool check_additional(int);

    FATIMA_Time_Calibration* FATIMA_T_CALIB;
    FATIMA_Energy_Calibration* FATIMA_E_CALIB;
    FATIMA_Gain_Match* FATIMA_GAIN_MATCH;

    //tmp root things
    TH1D** Det_Hist;

public:
	FATIMA_Detector_System();
	~FATIMA_Detector_System();
	
	
	//void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};
	void Process_FRS(TGo4MbsSubEvent* psubevt){};

	void Process_AIDA(TGo4MbsSubEvent* psubevt){};


	
	
	void Process_MBS(int*);
	void get_Event_data(Raw_Event*);
	int* get_pdata();

	unsigned long** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;};
    //bool do_gain_matching(){return gain_match_used;};

    void write();
    
    void set_Gain_Match_Filename(std::string);


};



#endif
