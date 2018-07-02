#ifndef GALILEO_TIME_CALIB_H
#define GALILEO_TIME_CALIB_H

#include <string>
#include <fstream>
#include <iostream>

typedef unsigned long ULong;


class GALILEO_Time_Calibration{

private:

    double calib_coeffs_sum[36];
    double calib_coeffs_channels[36];

    void load_Calibration_File();


public:
    GALILEO_Time_Calibration();
    ~GALILEO_Time_Calibration();
    
    ULong Calibrate_FEBEX_Sum_T(ULong,int);
    ULong Calibrate_FEBEX_Chan_T(ULong,int);

};



#endif
