#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <cstdlib>

#include <TFile.h>

#include "Tree_Creator.h"
#include "Raw_Event.h"

#include "PLASTIC_DataStruct.h"
#include "PLASTIC_VME_DataStruct.h"
#include "FATIMA_DataStruct.h"

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
};

#endif
