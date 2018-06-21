#ifndef AIDA_DATA_STREAM_H
#define AIDA_DATA_STREAM_H

#include "Detector_System_Data_Stream.cxx"

typedef unsigned long long ULong64_t;

class AIDA_Data_Stream : public Detector_System_Data_Stream{

private:
    
    ULong64_t WR_T;

    double E[100];
    double T[100];

    int hit_grid_ID[100];
    int strip_x_ID[100];
    int strip_y_ID[100];

    int am_hits;

public:

    AIDA_Data_Stream();
    ~AIDA_Data_Stream();

    void set_data_stream(double**,ULong64_t);
    void set_Event();

};



#endif