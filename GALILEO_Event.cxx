#include "GALILEO_Event.h"

using namespace std;

//---------------------------------------------------------------

GALILEO_Event::GALILEO_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

GALILEO_Event::~GALILEO_Event(){}

//---------------------------------------------------------------

inline void GALILEO_Event::set_DATA(Raw_Event* RAW){
    //something
    //FAT_FIRED = RAW->get_GALILEO_am_Fired();

    return;
}

//---------------------------------------------------------------