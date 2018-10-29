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
    am_fired = RAW->get_GALILEO_am_Fired();
    energy_tot = 0;
    for(int i = 0;i < am_fired;++i) energy_tot += RAW->get_GALILEO_Chan_E(i);
    //cout << "GALILEO EVENT WITH " << energy_tot << endl;
    
}

//---------------------------------------------------------------

double GALILEO_Event::get_energy(){return (am_fired > 0) ? energy_tot : -1;}



void GALILEO_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
}

//---------------------------------------------------------------
