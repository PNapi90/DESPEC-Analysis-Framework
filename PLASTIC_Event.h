#ifndef PLASTIC_EVENT_H
#define PLASTIC_EVENT_H

#include <iostream>
#include <cstdlib>
#include <vector>

#include "Events.h"
#include "Raw_Event.h"


class PLASTIC_Event : public Events{

private:

	//PLASTIC_Data DATA;

	
    inline void set_DATA(Raw_Event*);


public:
    PLASTIC_Event(int*,int,Raw_Event*);
    virtual ~PLASTIC_Event();
    
    double get_energy(){return 0;};
    void Write_Event(Tree_Creator*);
};

#endif
