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
    
    /*DATA.AmountFired = RAW->get_FAT_det_fired();
    
    for(int i = 0;i < DATA.AmountFired;++i){
		DATA.Energies[i] = RAW->get_FAT_E(i);
		DATA.IDs[i] = RAW->get_FAT_id(i);
		DATA.Times[i] = RAW->get_FAT_t(i);
	}
    */
}

//---------------------------------------------------------------

double FATIMA_Event::get_energy(){return 0;}

//---------------------------------------------------------------

void FATIMA_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------
