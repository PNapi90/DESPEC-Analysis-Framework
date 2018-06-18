#ifndef FATIMA_TIME_CALIB_H
#define FATIMA_TIME_CALIB_H

#include <string>
#include <fstream>
#include <iostream>

class FATIMA_Time_Calibration{

private:

    const char* format = "%d %lf";

    double calib_coeffs[36];

    std::string filename;

    void load_Calibration_File();


public:
    FATIMA_Time_Calibration(std::string);
    ~FATIMA_Time_Calibration();
    
    double Calibrate(double,int);
};



#endif