#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <cstdlib>

#include <TFile.h>

#include "Tree_Creator.h"
#include "Raw_Event.h"

#include "FRS_DataStruct.h"
#include "AIDA_DataStruct.h"
#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"
#include "FATIMA_DataStruct.h"
#include "GALILEO_DataStruct.h"
#include "FINGER_DataStruct.h"

class Events{

protected:

    int length,iterator;
    int* positions;

    bool* Permission_array;

    int** Match_IDs;

public:
    Events(int*,int);
    virtual ~Events();

    void set_Match_ID_address(int*);
    void set_Match_ID_address_NULL_ALL();
    
    bool Permission(int);

    int get_iterator();
    int get_Match_ID(int);

    int** get_Match_ID_address();
    
    //System specific functions
    virtual double get_energy() = 0;
    virtual void Write_Event(Tree_Creator* Tree) = 0;
    
    virtual PLASTIC_DataStruct* GET_PLASTIC() = 0;
    virtual PLASTIC_VME_DataStruct* GET_PLASTIC_VME() = 0;
    virtual FATIMA_DataStruct* GET_FATIMA() = 0;
    virtual FRS_DataStruct* GET_FRS() = 0;
    virtual AIDA_DataStruct* GET_AIDA() = 0;
    virtual GALILEO_DataStruct* GET_GALILEO() = 0;
    virtual FINGER_DataStruct* GET_FINGER() = 0;
};

#endif
