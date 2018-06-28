#include "TDC_FATIMA_Channel.h"

using namespace std;

//---------------------------------------------------------------

TDC_FATIMA_Channel::TDC_FATIMA_Channel(int det_id,int board_ID,int Ch_num){
    this->det_id = det_id;
    this->board_ID = board_ID;
    this->Ch_num = Ch_num;

    iterator = 0;
    fired = false;
    data_stream = new double*[100];
    for(int i = 0;i < 100;++i) data_stream[i] = new double[2];
}

//---------------------------------------------------------------

TDC_FATIMA_Channel::~TDC_FATIMA_Channel(){
    if(data_stream){
        for(int i = 0;i < 100;++i) delete[] data_stream[i];
        delete[] data_stream;
    }
}
//---------------------------------------------------------------

bool TDC_FATIMA_Channel::get_TDC_Fired(){return fired;}

//---------------------------------------------------------------

void TDC_FATIMA_Channel::set_TDC_Fired(){fired = true;}

//---------------------------------------------------------------

int TDC_FATIMA_Channel::get_Det_id(){return det_id;}

//---------------------------------------------------------------

void TDC_FATIMA_Channel::set_TDC_Time(double TDC_Time_tmp){
    if(first_call){
        iterator = 0;
        first_call = false;
    }
    //cout << "iti " << iterator << endl;
    TDC_Time[iterator] = TDC_Time_tmp;
}

//---------------------------------------------------------------

void TDC_FATIMA_Channel::Calibrate(FATIMA_Time_Calibration* FAT_T){
    Time[iterator] = FAT_T->Calibrate(TDC_Time[iterator],det_id);
    iterator++;
}

//---------------------------------------------------------------

double** TDC_FATIMA_Channel::get_Data(){
    for(int i = 0;i < iterator;++i){
        data_stream[i][0] = Time[i];
        data_stream[i][1] = iterator;
    }
    first_call = true;
    fired = false;
    return data_stream;
}

//---------------------------------------------------------------

void TDC_FATIMA_Channel::reset(){
    iterator = 0;
    first_call = true;
}

//---------------------------------------------------------------