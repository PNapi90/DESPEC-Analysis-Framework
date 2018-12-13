#ifndef FATIMA_DETECTOR_SYSTEM_H
#define FATIMA_DETECTOR_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>
#include <vector>

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
	
	int unknown_header_counter;

	int* pdata;
	
	//These are indexed by fired_QDC_amount
	int fired_QDC_amount;
	std::vector<int> det_ids_QDC;
	std::vector<double> QLong;
	std::vector<double> QLong_Raw;
	std::vector<double> QShort_Raw;
	std::vector<ULong64_t> QDC_Time_Coarse;
	std::vector<double> QDC_Time_Fine;
	
	//These are indexed by fired_TDC_amount
	int fired_TDC_amount;
	std::vector<int> det_ids_TDC;
	std::vector<ULong64_t> TDC_Time_raw;		//This is the raw TDC time
	std::vector<double> TDC_Time_ns;		//This is the raw TDC time


	//These are used for mapping mod,chn to detector
	std::vector<std::vector<int> > det_ID_QDC;
	std::vector<std::vector<int> > det_ID_TDC;
	std::vector<std::vector<double> > FAT_positions;
	
	std::vector<double> source_position_correction;

	bool exiter;
	bool no_data;
	bool QDC_DATA;
	std::string gain_match_filename;
	
	bool gain_match_used; //Set in the constructor
	bool dist_corr_used;
	int num_TDC_modules = 0;  //Set in the constructor
	int num_TDC_modules_fixed = 0;  //Set in the constructor
    
	    //This is used during individual module unpacking
	std::vector<std::vector<int> > Fired_QDC_Channels;
    

	void load_board_channel_file();
	void load_det_angles();
	void load_pos_correction();

	void reset_fired_channels();
	void Check_QDC_DATA(QDC_Header*);
	void Check_TDC_DATA();
	void Calibrate_QDC(int);
	void Gain_Match_QDC(int);
	void Calibrate_TDC(int);
	
	void read_config_variables(std::string);
	
	
	double distance_between_detectors(double, double, double, double, double, double);


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
	void Process_MBS(TGo4MbsSubEvent* psubevt){};
	
	void Process_MBS(int*);
	void get_Event_data(Raw_Event*);
	int* get_pdata();
		

    //bool do_gain_matching(){return gain_match_used;};

    void write();
    bool calibration_done(){return false;}

    void set_Gain_Match_Filename(std::string);


};



#endif
