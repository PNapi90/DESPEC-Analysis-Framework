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
typedef unsigned long long ULong64_t;

private:
	int max_am_dets;
	
	int board_id;
	int num_channels;
	
	

	int* pdata;

	ULong64_t tmp_Sum_Time;
	int tmp_Pileup;
	int tmp_Hit_Pattern;
	
	ULong64_t* Sum_Time;
	int* Pileup;
	int* Hit_Pattern;
	
	ULong64_t* Chan_Time;
	double* Chan_Energy;

	int* det_ids;

	std::map<std::pair<int,int>, int> GALILEO_map;
	
	int num_channels_fired = 0;

	int pileup_flags[16];
	int Ge_channels[16];
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
	
	//void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};
	void Process_FRS(TGo4MbsSubEvent* psubevt){};

	void Process_AIDA(TGo4MbsSubEvent* psubevt){};

	void Process_MBS(int*);
	void get_Event_data(Raw_Event*);
	int* get_pdata();
	
	void read_config_variables(std::string){};


	unsigned long** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;}
    void write(){return;};
    void set_Gain_Match_Filename(std::string){return;};


};



#endif
