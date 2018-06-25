#ifndef PLASIC_CALIBRATOR_H
#define PLASIC_CALIBRATOR_H

typedef unsigned long ULong;
typedef unsigned int UInt;

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>


#include <TFile.h>
#include <TH1.h>

class PLASTIC_Calibrator{

private:

    bool ONLINE;

	int am_fired,iter;
    int am_bins;

    double min_val,max_val;

    bool** fired;

    double* bins_x_arr;
    double*** Cal_arr;

    TH1*** Fine_Hist;

	void load_Calibration_File();



public:
	PLASTIC_Calibrator(bool);
	~PLASTIC_Calibrator();
	
	void calibrate(ULong**,ULong**,UInt**,int,int);
	void calibrate(ULong,ULong,ULong*,int);

    void ONLINE_CALIBRATION();
    void OFFLINE_CALIBRATION();

};

#endif
