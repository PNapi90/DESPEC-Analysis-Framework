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
    for(int i = 0;i < 3;++i){
		ch_time[i] = 1337;
		energies[i] = -1337;
		det_id[i] = -1;
	}
    for(int i = 0;i < am_fired;++i){
		det_id[i] = RAW->get_GALILEO_det_id(i);
		energy_tot += RAW->get_GALILEO_Chan_E(i);
		energies[det_id[i]] = RAW->get_GALILEO_Chan_E(det_id[i]);
		ch_time[det_id[i]] = RAW->get_GALILEO_Chan_T(det_id[i]);
	}
    
    //cout << "GALILEO EVENT WITH " << energy_tot << endl;
    
}

//---------------------------------------------------------------

double GALILEO_Event::get_energy(){return (am_fired > 0) ? energy_tot : -1;}


ULong GALILEO_Event::get_time(int i){return ch_time[i];}

double GALILEO_Event::get_energies(int i){
	return energies[i];
}
