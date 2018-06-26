#include "FATIMA_Data_Stream.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Data_Stream::FATIMA_Data_Stream(){}

//---------------------------------------------------------------

FATIMA_Data_Stream::~FATIMA_Data_Stream(){}

//---------------------------------------------------------------

void FATIMA_Data_Stream::set_event_data(double E,ULong T,ULong QDC_T,int total_hits,int hits_det,int det_ID){
    FATIMA_Det_ID[fat_iter] = det_ID;
    FATIMA_E[fat_iter] = E;
    FATIMA_T[fat_iter] = T;
    FATIMA_QDC_T[fat_iter] = QDC_T;
    FATIMA_hits_det[fat_iter] = hits_det;
    fat_iter++;

    FATIMA_Event_Length = fat_iter;

}

//---------------------------------------------------------------

void FATIMA_Data_Stream::reset(){
    fat_iter = 0;
    mismatch = false;
}

//---------------------------------------------------------------

void FATIMA_Data_Stream::set_mismatch(){mismatch = true;}

//---------------------------------------------------------------

bool FATIMA_Data_Stream::get_mismatch(){return mismatch;}

//---------------------------------------------------------------

int FATIMA_Data_Stream::get_amount_Hits(){return fat_iter;}

//---------------------------------------------------------------

double FATIMA_Data_Stream::get_E(int i){return FATIMA_E[i];}

//---------------------------------------------------------------