#ifndef CALIB_H
#define CALIB_H

class Calibration{

public:
    Calibration();
    virtual ~Energy_Calibration();
    
    virtual double Calibrate(double,int);

};

#endif