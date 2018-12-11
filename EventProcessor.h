#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include "FRS_Event.h"
#include "AIDA_Event.h"
#include "PLASTIC_Event.h"
#include "FATIMA_Event.h"
#include "GALILEO_Event.h"


class EventProcessor{

public:
    virtual ~EventProcessor(){}

    virtual void PassEvent(FRS_Event*) = 0;
    virtual void PassEvent(AIDA_Event*) = 0;
    virtual void PassEvent(PLASTIC_Event*) = 0;
    virtual void PassEvent(FATIMA_Event*) = 0;
    virtual void PassEvent(GALILEO_Event*) = 0;

    virtual bool AllPassed() = 0;
};

#endif