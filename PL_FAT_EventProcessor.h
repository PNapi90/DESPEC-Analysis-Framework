#ifndef PL_FAT_EVENTPROCESSOR_H
#define PL_FAT_EVENTPROCESSOR_H 

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>

#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"
#include "FATIMA_DataStruct.h"

#include "EventProcessor.cxx"



class PL_FAT_EventProcessor : public EventProcessor{

private:

    const int MAX_FAT = 36;
    const int MAX_PL = 50;

    const bool PLASTIC_VME = true;

    PLASTIC_DataStruct* DATA_P;
    PLASTIC_VME_DataStruct* DATA_VME_P;

    FATIMA_DataStruct* DATA_F;

    bool PLASTIC_Passed;
    bool FATIMA_Passed;

    TFile* FILE;
    TH1D*** TDiff;

    void ProcessEvent();


public:
    PL_FAT_EventProcessor();
    ~PL_FAT_EventProcessor();


    void PassEvent(PLASTIC_Event* Event);
    void PassEvent(FATIMA_Event* Event);


    bool AllPassed();

    //not used here
    void PassEvent_FRS(FRS_Event*){};
    void PassEvent_AIDA(AIDA_Event*){};

    void PassEvent_GALILEO(GALILEO_Event*){};



};


#endif