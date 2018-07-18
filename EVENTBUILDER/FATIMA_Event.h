#ifndef FATIMA_EVENT_H
#define FATIMA_EVENT_H

#include "Events.cxx"

#include "Raw_Event.h"

class FATIMA_Event : public Events{

private:
    int** Match_IDs;

    inline void set_DATA(Raw_Event*);


public:
    FATIMA_Event(int,Raw_Event*);
    ~FATIMA_Event();

    void set_Match_ID_address(int*);
    void set_Match_ID_address_to_NULL_ALL();
    
    bool Permission(int);

    int* get_Match_ID_adress();
};

#endif