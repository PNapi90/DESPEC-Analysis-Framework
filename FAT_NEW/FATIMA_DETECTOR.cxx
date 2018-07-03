#include "FATIMA_DETECTOR.h"

#include <cstdlib>

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
        mismatch = true;
        return;
        cerr << "FATIMA_DETECTOR " << det_id << " -> mismatch of ";
        cerr << "hits in TDC and QDC ! " << endl;
        cout << "QDC " << am_of_hits << "   TDC " << am_of_hits_TDC << endl;  
        exit(0);
    }
    mismatch = false;

    //if correct -> write Energies and Times in detector
    for(int i = 0;i < am_of_hits;++i){
        Energy[i] = data_QDC[i][0];
        QDC_Time[i] = data_QDC[i][1];
        QDC_Fine_Time[i] = data_QDC[i][3];
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
	data_stream[i][3] = QDC_Fine_Time[i];

    }
    return data_stream;
}

//---------------------------------------------------------------

double FATIMA_DETECTOR::get_E(int i){return Energy[i];}

//---------------------------------------------------------------

double FATIMA_DETECTOR::get_T(int i){return Time[i];}

//---------------------------------------------------------------

double FATIMA_DETECTOR::get_QDC_T(int i){return QDC_Time[i];}

//---------------------------------------------------------------

double FATIMA_DETECTOR::get_QDC_FT(int i){return QDC_Fine_Time[i];}

//---------------------------------------------------------------

bool FATIMA_DETECTOR::get_mismatch(){return mismatch;}
