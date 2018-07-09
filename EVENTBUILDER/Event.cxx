#include "Event.h"

using namespace std;

//---------------------------------------------------------------

Event::Event(int* AIDA_XYZ){
    //set AIDA coordinates
    AIDA_Z = AIDA_XYZ[2];
    AIDA_X = AIDA_XYZ[0];
    AIDA_Y = AIDA_XYZ[1];

}

//---------------------------------------------------------------

Event::~Event(){

}

//---------------------------------------------------------------

void Event::set_FRS_DATA(Raw_Event* RAW){
    //set FRS data of Event
    A_over_Q = RAW->get_A_over_Q();

}

//---------------------------------------------------------------



//---------------------------------------------------------------

void Event::write_Event(){
    //write Event in histogram (for example)

}

//---------------------------------------------------------------