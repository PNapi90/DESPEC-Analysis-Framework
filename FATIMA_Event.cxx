#include "FATIMA_Event.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Event::FATIMA_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

FATIMA_Event::~FATIMA_Event(){}

//---------------------------------------------------------------

inline void FATIMA_Event::set_DATA(Raw_Event* RAW){
    //something
    FAT_FIRED = RAW->get_FATIMA_am_Fired();

    return;
}

//---------------------------------------------------------------