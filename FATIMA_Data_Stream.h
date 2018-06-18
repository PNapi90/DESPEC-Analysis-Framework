#ifndef FATIMA_DATA_STREAM_H
#define FATIMA_DATA_STREAM_H

#include "Detector_System_Data_Stream.cxx"

typedef unsigned long long ULong64_t;

class FATIMA_Data_Stream : public Detector_System_Data_Stream{

private:
    
    ULong64_t WR_T;

    double E[36];
    double T[36];

    int hitID[36];
    int am_hits;

public:

    FATIMA_Data_Stream();
    ~FATIMA_Data_Stream();

    void set_data_stream(double**,int,ULong64_t);
    void set_Event();

};



#endif