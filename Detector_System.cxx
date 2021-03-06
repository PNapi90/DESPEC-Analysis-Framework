
#ifndef DETECTOR_SYSTEM_H
#define DETECTOR_SYSTEM_H 

#include "Raw_Event.h"

#include "Data_Stream.cxx"

#include <string>

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"

#include "TSCNParameter.h"



class Detector_System{

public:

    virtual ~Detector_System(){};

    virtual void Process_MBS(int*) = 0;
    
    virtual void Process_FRS(TGo4MbsSubEvent* psubevt) = 0;

    virtual void Process_AIDA(TGo4MbsSubEvent* psubevt) = 0;

    virtual void get_Event_data(Raw_Event*) = 0;

//    virtual void get_Event_data(Data_Stream*) = 0;

    virtual unsigned long** tmp_get_coarse_T() = 0;

    virtual int tmp_get_am_hits() = 0;

    virtual int* get_pdata() = 0;

    virtual unsigned int** tmp_get_chID() = 0;

	virtual int* tmp_get_iterator() = 0;

	virtual bool calibration_done() = 0;

	virtual void write() = 0;
	
	//virtual bool do_gain_matching() = 0;
	virtual void set_Gain_Match_Filename(std::string) = 0;
};

#endif
