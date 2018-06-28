#ifndef FATIMA_DETECTOR_H
#define FATIMA_DETECTOR_H

#include <iostream>

class FATIMA_DETECTOR{

private:
    
    int det_id;
    int am_of_hits;

    double Energy[100];
    double Time[100];
    double QDC_Time[100];
    double QDC_Fine_Time[100];

    bool mismatch;

    double** data_stream;

public:
    FATIMA_DETECTOR(int);
    ~FATIMA_DETECTOR();

    void set_Data(double**,double**);

    int get_amount_of_hits();
    double** get_Data();

    double get_E(int);
    double get_T(int);
    double get_QDC_T(int);
    double get_QDC_FT(int);


    bool get_mismatch();

};

#endif
