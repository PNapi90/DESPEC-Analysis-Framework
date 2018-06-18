#include "Event.h"

using namespace std;

//---------------------------------------------------------------

Event::Event(){
    
}

//---------------------------------------------------------------

Event::~Event(){

}

//---------------------------------------------------------------

void Event::OFFLINE_ANALYSIS(){

    //create White_Rabbit object
    White_Rabbit WR;

    //get amount of raw events
    amount_Raw_Events = WR.get_Raw_Event_Number();

    //create DESPEC Detector system
    Detector_System DESPEC;

    bool true_event = false;

    //do offline analysis 
    for(size_t i = 0;i < amount_Raw_Events;++i){
        //check if event is triggered
        true_event = DESPEC.get_next_Event(WR.next_TimeStamp());
        //if triggered -> save event in output
        if(true_event) write_event(DESPEC.get_Event_data(),DESPEC.get_Event_type());
    }
}
//---------------------------------------------------------------
void Event::write_event(double** EVENT_TMP_ARRAY,bool type){
    //think about output format (e.g. for go4)

}
//---------------------------------------------------------------
