#include "PLASTIC_Data_Stream.h"

using namespace std;


//---------------------------------------------------------------

PLASTIC_Data_Stream::PLASTIC_Data_Stream(){}

//---------------------------------------------------------------

PLASTIC_Data_Stream::~PLASTIC_Data_Stream(){}

//---------------------------------------------------------------

void PLASTIC_Data_Stream::reset(){
    mismatch = false;
}

//---------------------------------------------------------------

void PLASTIC_Data_Stream::set_event_data(ULong* coarse_T,ULong* fine_T,UInt* ch_ID,ULong coarse_T_Trigger,ULong fine_T_Trigger){
    for(int i = 0;i < amount;++i){
        PLASTIC_Coarse_T[i] = coarse_T[i];
        PLASTIC_Fine_T[i] = fine_T[i];
        PLASTIC_ch_ID[i] = ch_ID[i];
    }
    PLASTIC_Coarse_T_Trigger = coarse_T_Trigger;
    PLASTIC_Fine_T_Trigger = fine_T_Trigger;
}

//---------------------------------------------------------------

void PLASTIC_Data_Stream::set_amount_of_Events(int amount){this->amount = amount;}

//---------------------------------------------------------------

void PLASTIC_Data_Stream::set_mismatch(){mismatch = true;}

//---------------------------------------------------------------

bool PLASTIC_Data_Stream::get_mismatch(){return mismatch;}

//---------------------------------------------------------------

int PLASTIC_Data_Stream::get_amount_Hits(){return amount;}

//---------------------------------------------------------------

double PLASTIC_Data_Stream::get_E(int i){return 0;}

//---------------------------------------------------------------