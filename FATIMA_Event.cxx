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
    //FAT_FIRED = RAW->get_FATIMA_am_Fired();
    am_fired = RAW->get_FAT_det_fired();
    energy_tot = 0;
    for(int i = 0;i < am_fired;++i) energy_tot += RAW->get_FAT_E(i);
    //cout << "FATIMA EVENT WITH " << energy_tot << endl;

}

//---------------------------------------------------------------

double FATIMA_Event::get_energy(){return (am_fired > 0) ? energy_tot : -1;}
