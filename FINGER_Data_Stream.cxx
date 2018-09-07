#include "FINGER_Data_Stream.h"

using namespace std;


//---------------------------------------------------------------

FINGER_Data_Stream::FINGER_Data_Stream(){}

//---------------------------------------------------------------

FINGER_Data_Stream::~FINGER_Data_Stream(){}

//---------------------------------------------------------------

void FINGER_Data_Stream::reset(){
    mismatch = false;
}

//---------------------------------------------------------------

void FINGER_Data_Stream::set_event_data(ULong* coarse_T,ULong* fine_T,UInt* ch_ID,ULong coarse_T_Trigger,ULong fine_T_Trigger){
    for(int i = 0;i < amount;++i){
        FINGER_Coarse_T[i] = coarse_T[i];
        FINGER_Fine_T[i] = fine_T[i];
        FINGER_ch_ID[i] = ch_ID[i];
    }
    FINGER_Coarse_T_Trigger = coarse_T_Trigger;
    FINGER_Fine_T_Trigger = fine_T_Trigger;
}

//---------------------------------------------------------------

void FINGER_Data_Stream::set_amount_of_Events(int amount){this->amount = amount;}

//---------------------------------------------------------------

void FINGER_Data_Stream::set_mismatch(){mismatch = true;}

//---------------------------------------------------------------

bool FINGER_Data_Stream::get_mismatch(){return mismatch;}

//---------------------------------------------------------------

int FINGER_Data_Stream::get_amount_Hits(){return amount;}

//---------------------------------------------------------------

double FINGER_Data_Stream::get_E(int i){return 0;}

//---------------------------------------------------------------
