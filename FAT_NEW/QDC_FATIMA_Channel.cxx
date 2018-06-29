#include "QDC_FATIMA_Channel.h"

using namespace std;

//---------------------------------------------------------------

QDC_FATIMA_Channel::QDC_FATIMA_Channel(int det_id,int board_ID,int Ch_Num){
    this->det_id = det_id;
    this->board_ID = board_ID;
    this->Ch_Num = Ch_Num;

    data_stream = new double*[100];
    for(int i = 0;i < 100;++i) data_stream[i] = new double[10];
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

void QDC_FATIMA_Channel::set_QDC_Fine_Time(double QDC_Fine_Time_tmp){
    QDC_Fine_Time[iterator] = QDC_Fine_Time_tmp;
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
    
}

//---------------------------------------------------------------

void QDC_FATIMA_Channel::Calibrate(FATIMA_Energy_Calibration* FAT_E,FATIMA_Time_Calibration* FAT_T){
    Energy[iterator] = FAT_E->Calibrate(QLong[iterator],det_id);
    QDC_Time[iterator] = FAT_T->Calibrate_QDC(QDC_Time[iterator],det_id);
    iterator++;
}

//---------------------------------------------------------------

double** QDC_FATIMA_Channel::get_Data(){
    //cout << "-----------------------------" << endl;
    for(int i = 0;i < iterator;++i){
        data_stream[i][0] = Energy[i];
        data_stream[i][1] = QDC_Time[i];
        data_stream[i][2] = iterator;
        data_stream[i][3] = QDC_Fine_Time[i];
        //for(int j = 0;j < 4;++j) cout << dec << data_stream[i][j] << " ";
       // cout <<" <- "<< i  <<endl;
    } 
    first_call = true;

   // cout << "-----------------------------" << endl;

    return data_stream;
}

//---------------------------------------------------------------

void QDC_FATIMA_Channel::reset(){
    iterator = 0;
    first_call = true;
}

//---------------------------------------------------------------