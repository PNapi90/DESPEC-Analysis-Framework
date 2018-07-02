
#ifndef GALILEO_ENERGY_CALIB_H
#define GALILEO_ENERGY_CALIB_H

#include <string>
#include <fstream>
#include <iostream>

class GALILEO_Energy_Calibration{

private:

    double calib_coeffs[100][5];

    std::string filename;

    void load_Calibration_File();


public:
    GALILEO_Energy_Calibration();
    ~GALILEO_Energy_Calibration();
    
    double Calibrate_FEBEX_E(double,int);
};



#endif
