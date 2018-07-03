#ifndef GALILEO_DETECTOR_SYSTEM_H
#define GALILEO_DETECTOR_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>


#include "GALILEO_Energy_Calibration.h"
#include "GALILEO_Time_Calibration.h"

#include "FEBEX.h"

#include "Detector_System.cxx"


class GALILEO_Detector_System : public Detector_System{

private:
	int max_am_dets;
	
	int board_id;
	int num_channels;
	
	

	int* pdata;

	ULong tmp_Sum_Time;
	int tmp_Pileup;
	int tmp_Hit_Pattern;
	
	ULong* Sum_Time;
	int* Pileup;
	int* Hit_Pattern;
	
	ULong* Chan_Time;
	double* Chan_Energy;

	int* det_ids;

	std::map<std::pair<int,int>, int> GALILEO_map;

	int Fired_FEBEX_Channels[100][2];
	int fired_FEBEX_amount;

	void load_board_channel_file();
	void reset_fired_channels();
	void Calibrate_FEBEX(int);

	bool wired_FEBEX(int,int);

	GALILEO_Time_Calibration* GALILEO_T_CALIB;
	GALILEO_Energy_Calibration* GALILEO_E_CALIB;


public:
	GALILEO_Detector_System();
	~GALILEO_Detector_System();
	
	void Process_MBS(int*);
	void get_Event_data(Raw_Event*);
	int* get_pdata();

	unsigned long*** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int*** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;}


};



#endif