#ifndef PL_FAT_EVENTPROCESSOR_H
#define PL_FAT_EVENTPROCESSOR_H 

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <string>

#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"
#include "FATIMA_DataStruct.h"

#include "EventProcessor.cxx"



class PL_FAT_EventProcessor : public EventProcessor{

private:

    const int MAX_FAT = 40;
    const int MAX_PL = 40;

    const bool PLASTIC_VME = true;

    PLASTIC_DataStruct* DATA_P;
    PLASTIC_VME_DataStruct* DATA_VME_P;

    FATIMA_DataStruct* DATA_F;

    bool PLASTIC_Passed;
    bool FATIMA_Passed;

    TFile* FILE;
    TH1F*** TDiff;
    TH2F*** EvE;
    void ProcessEvent();


public:
    PL_FAT_EventProcessor();
    ~PL_FAT_EventProcessor();


    void PassEvent_PLASTIC_VME(PLASTIC_VME_DataStruct* DATA_VME_P);
    void PassEvent_PLASTIC(PLASTIC_DataStruct* DATA_VME_P);
    void PassEvent_FATIMA(FATIMA_DataStruct* DATA_F);


    bool AllPassed();

    //not used here
    void PassEvent_FRS(FRS_DataStruct*){return;};
    void PassEvent_AIDA(AIDA_DataStruct*){return;};

    void PassEvent_GALILEO(GALILEO_DataStruct*){return;};
    void PassEvent_FINGER(FINGER_DataStruct*){return;};



};


#endif
