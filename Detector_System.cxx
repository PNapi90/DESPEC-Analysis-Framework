
#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

#include "Data_Stream.cxx"

class Detector_System{

public:

	virtual ~Detector_System() = 0;

    virtual void Process_MBS(int*) = 0;
    
    virtual void get_Event_data(Data_Stream*) = 0;

    virtual unsigned long*** tmp_get_coarse_T() = 0;

    virtual int tmp_get_am_hits() = 0;

    virtual int* get_pdata() = 0;

    virtual unsigned int*** tmp_get_chID() = 0;

	virtual int* tmp_get_iterator() = 0;

	virtual bool calibration_done() = 0;
};

#endif
