#include "QDC_FATIMA_Channel.h"

//---------------------------------------------------------------

QDC_FATIMA_Channel::QDC_FATIMA_Channel(int det_id,int board_ID,int Ch_Num){
    this->det_id = det_id;
    this->board_ID = board_ID;
    this->Ch_Num = Ch_Num;

    data_stream = new double*[100];
    for(int i = 0;i < 100;++i) data_stream[i] = new double[3];
}

//---------------------------------------------------------------


QDC_FATIMA_Channel::~QDC_FATIMA_Channel(){
    if(data_stream){
        for(int i = 0;i < 100;++i) delete[] data_stream[i];
        delete[] data_stream;
    }
}

//---------------------------------------------------------------

void QDC_FATIMA_Channel::set_QDC_Time(double QDC_Time_tmp){
    QDC_Time[iterator] = QDC_Time_tmp;
}

//---------------------------------------------------------------

void QDC_FATIMA_Channel::set_QLong_Raw(double QLong_tmp){
    if(first_call){
        iterator = 0;
        first_call = false;
    }
    QLong[iterator] = QLong_tmp;
}

//---------------------------------------------------------------

void QDC_FATIMA_Channel::set_QShort_Raw(double QShort_tmp){
    QShort[iterator] = QShort_tmp;
    iterator++;
}

//---------------------------------------------------------------

void QDC_FATIMA_Channel::Calibrate(FATIMA_Energy_Calibration* FAT_E){
    Energy[iterator] = FAT_E->Calibrate(QLong[iterator],det_id);
}

//---------------------------------------------------------------

double** QDC_FATIMA_Channel::get_Data(){
    for(int i = 0;i < iterator;++i){
        data_stream[i][0] = Energy[i];
        data_stream[i][1] = QDC_Time[i];
        data_stream[i][2] = iterator;
    }
    first_call = true;

    return data_stream;
}