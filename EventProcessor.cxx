#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "FRS_DataStruct.h"
#include "AIDA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"
#include "FATIMA_DataStruct.h"
#include "GALILEO_DataStruct.h"


class EventProcessor{

public:
    virtual ~EventProcessor(){};

    virtual void PassEvent_FRS(FRS_DataStruct*) = 0;
    virtual void PassEvent_AIDA(AIDA_DataStruct*) = 0;
    virtual void PassEvent_PLASTIC_VME(PLASTIC_VME_DataStruct*) = 0;
    virtual void PassEvent_PLASTIC(PLASTIC_DataStruct*) = 0;
    virtual void PassEvent_FATIMA(FATIMA_DataStruct*) = 0;
    virtual void PassEvent_GALILEO(GALILEO_DataStruct*) = 0;

    virtual bool AllPassed() = 0;
};

#endif