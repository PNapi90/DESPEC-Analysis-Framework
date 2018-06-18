#include "FATIMA_DETECTOR.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_DETECTOR::FATIMA_DETECTOR(int det_id){
    this->det_id = det_id;

    data_stream = new double*[100];
    for(int i = 0;i < 100;++i) data_stream[i] = new double[3]; 
}

//---------------------------------------------------------------

FATIMA_DETECTOR::~FATIMA_DETECTOR(){
    if(data_stream){
        for(int i = 0;i < 100;++i) delete[] data_stream[i];
        delete[] data_stream;
    }
}

//---------------------------------------------------------------

void FATIMA_DETECTOR::set_Data(double** data_QDC,double** data_TDC){
    
    //get amount of hits
    am_of_hits = (int) data_QDC[0][2];

    //check if TDC and QDC have same amounts of hits
    int am_of_hits_TDC = (int) data_TDC[0][1];

    if(am_of_hits != am_of_hits_TDC){
        cerr << "FATIMA_DETECTOR " << det_id << " -> mismatch of ";
        cerr << "hits in TDC and QDC ! " << endl;
        //exit(0);
    }

    //if correct -> write Energies and Times in detector
    for(int i = 0;i < am_of_hits;++i){
        Energy[i] = data_QDC[i][0];
        QDC_Time[i] = data_QDC[i][1];
        Time[i] = data_TDC[i][0];
    }
}

//---------------------------------------------------------------

int FATIMA_DETECTOR::get_amount_of_hits(){return am_of_hits;}

//---------------------------------------------------------------

double** FATIMA_DETECTOR::get_Data(){
    for(int i = 0;i < am_of_hits;++i){
        data_stream[i][0] = Energy[i];
        data_stream[i][1] = Time[i];
        data_stream[i][2] = QDC_Time[i];
    }
    return data_stream;
}

//---------------------------------------------------------------
