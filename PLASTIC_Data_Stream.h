#ifndef PLASTIC_DATA_STREAM_H
#define PLASTIC_DATA_STREAM_H

#include "Detector_System_Data_Stream.cxx"

typedef unsigned long long ULong64_t;

class PLASTIC_Data_Stream : public Detector_System_Data_Stream{

private:
    
    ULong64_t WR_T;

    double E[100];
    double T[100];

    int hitID[100];
    int am_hits;

public:

    PLASTIC_Data_Stream();
    ~PLASTIC_Data_Stream();

    void set_data_stream(double**,ULong64_t);
    void set_Event();

};



#endif