#include "FRS_Event.h"

//---------------------------------------------------------------

FRS_Event::FRS_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

FRS_Event::~FRS_Event() {

}

//---------------------------------------------------------------

inline void FRS_Event::set_DATA(Raw_Event* RAW){
    //something
}

//---------------------------------------------------------------

double FRS_Event::get_energy(){
	return 0;
}

//---------------------------------------------------------------

void FRS_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------
