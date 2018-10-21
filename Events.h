#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <cstdlib>

#include <TFile.h>

#include "Raw_Event.h"

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
    
    virtual double get_energy() = 0;
    virtual void Write_Event(TFile* File) = 0;
};

#endif
