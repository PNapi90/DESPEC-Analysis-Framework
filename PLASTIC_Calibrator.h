#ifndef PLASIC_CALIBRATOR_H
#define PLASIC_CALIBRATOR_H

typedef unsigned long ULong;
typedef unsigned int UInt;

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>

class PLASTIC_Calibrator{

private:

	int am_fired;

	ULong cal_arr_c[100][100][2];
	ULong cal_arr_f[100][100][2];

	void load_Calibration_File();



public:
	PLASTIC_Calibrator();
	~PLASTIC_Calibrator();
	
	void calibrate(ULong**,ULong**,UInt**,int,int);
	void calibrate(ULong,ULong,ULong*,int);
};

#endif
