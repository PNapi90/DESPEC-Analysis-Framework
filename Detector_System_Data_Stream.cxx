#ifndef  DETECTOR_SYSTEM_DATA_STREAM_H
#define DETECTOR_SYSTEM_DATA_STREAM_H

typedef unsigned long long ULong64_t;

class Detector_System_Data_Stream{

public:
    
    virtual void set_data_stream(double**,int,ULong64_t) = 0;

    virtual void set_Event() = 0;


};


#endif
