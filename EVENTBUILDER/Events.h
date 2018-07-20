#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <cstdlib>

#include "Raw_Event.h"

class Events{

protected:

    int length,iterator;
    int* positions;

    int** Match_IDs;

public:
    Events();
    ~Events();

    void set_Match_ID_address(int*);
    void set_Match_ID_address_to_NULL_ALL();
    
    bool Permission(int);

    int get_iterator();

    int* get_Match_ID_adress(int);
};

#endif