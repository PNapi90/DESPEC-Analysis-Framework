#include "AIDA_Event.h"

//---------------------------------------------------------------

AIDA_Event::AIDA_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

AIDA_Event::~AIDA_Event() {

}

//---------------------------------------------------------------

inline void AIDA_Event::set_DATA(Raw_Event* RAW){
    //something
}

//---------------------------------------------------------------

double AIDA_Event::get_energy(){
	return 0;
}

//---------------------------------------------------------------

void AIDA_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------
