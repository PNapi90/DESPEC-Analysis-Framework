
#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

#include "Data_Stream.h"

class Detector_System{

public:

	virtual ~Detector_System() = 0;

    virtual void Process_MBS(int*) = 0;
    
    virtual void get_Event_data(Data_Stream*) = 0;

    virtual int* get_pdata() = 0;

};

#endif
