#include "PLASTIC_Event.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Event::PLASTIC_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    set_DATA(RAW);
}

//---------------------------------------------------------------

PLASTIC_Event::~PLASTIC_Event(){}

//---------------------------------------------------------------

inline void PLASTIC_Event::set_DATA(Raw_Event* RAW){
    //something

    return;
}

//---------------------------------------------------------------

void PLASTIC_Event::Write_Event(TFile* File){
	//User defined what variables should be written
}

//---------------------------------------------------------------
