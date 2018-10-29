#include "FATIMA_Event.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Event::FATIMA_Event(int* positions_tmp,int length,Raw_Event* RAW) : Events(positions_tmp,length){
    for(int i = 0;i < 50;++i) DATA.Energy[i] = 0;
    DATA.amountHits = 0;

    set_DATA(RAW);
}

//---------------------------------------------------------------

FATIMA_Event::~FATIMA_Event(){}

//---------------------------------------------------------------

inline void FATIMA_Event::set_DATA(Raw_Event* RAW){
    //something
    //FAT_FIRED = RAW->get_FATIMA_am_Fired();
    int am_fired = RAW->get_FAT_det_fired();
    double energy_tot = 0;
    for(int i = 0;i < am_fired;++i) energy_tot += RAW->get_FAT_E(i);
    
    DATA.amountHits = am_fired;
    for(int i = 0;i < am_fired;++i) DATA.Energy[i] = RAW->get_FAT_E(i);

    //cout << "FATIMA EVENT WITH " << energy_tot << endl;

}

//---------------------------------------------------------------

double FATIMA_Event::get_energy(){return (am_fired > 0) ? energy_tot : -1;}

//---------------------------------------------------------------

void FATIMA_Event::Write_Event(Tree_Creator* Tree){
	//User defined what variables should be written
    Tree->SetFATIMA(&DATA);
}

//---------------------------------------------------------------
