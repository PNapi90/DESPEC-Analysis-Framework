#include "FINGER_Event.h"

//---------------------------------------------------------------

FINGER_Event::FINGER_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

FINGER_Event::~FINGER_Event() {

}

//---------------------------------------------------------------

inline void FINGER_Event::set_DATA(Raw_Event* RAW){
    //something
}

//---------------------------------------------------------------

double FINGER_Event::get_energy(){
	return 0;
}

//---------------------------------------------------------------

void FINGER_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------
