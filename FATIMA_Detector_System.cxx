#include "FATIMA_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Detector_System::FATIMA_Detector_System(){

    //set amount of QDCs and TDCs
    am_QDCs = 5;
    am_TDCs = 20;

    //check if FATIMA and PLASTIC share White Rabbit
    check_shared();

    //initialize White Rabbit and QDC_TDC objects
    QDC_TDC = new QDC_TDC_FATIMA(am_QDCs,am_TDCs,shared_WR); 

    data_stream = nullptr;
}

//---------------------------------------------------------------

FATIMA_Detector_System::~FATIMA_Detector_System(){
    delete QDC_TDC;

}

//---------------------------------------------------------------

double** FATIMA_Detector_System::get_Event_data(){
    //return important information of event (energies,triggered detectors,multiplicity,...)
    return data_stream;
}

//---------------------------------------------------------------

void FATIMA_Detector_System::Process_MBS(int* pdata){
    //reset data_stream pointer
    data_stream = nullptr;

    //extract QDC and TDC informations from MBS and assign to detectors
    QDC_TDC->Run_FATIMA_MBS(pdata);

    //get amount of fired detectors
    fired_amount = QDC_TDC->get_amount_of_fired_Dets();
    //assign outgoing data stream (Energies, time and QDC time)
    data_stream = QDC_TDC->get_Detector_Data();
}

//---------------------------------------------------------------

void FATIMA_Detector_System::check_shared(){
    //open configuration file for White rabbit sharing
    ifstream shared_file("");

    if(shared_file.fail()){
        cerr << "Could not find White Rabbit share file ! " << endl;
        exit(0);
    }
    
    int share_tmp = 0;
    while(shared_file.good()){
        getline(shared_file,line,'\n');
        if(line[0] == "#") continue;
        sscanf(line.c_str(),"%d",&share_tmp);
    }
    //shared if 1
    shared_WR = (share_tmp == 1);


    if(shared_WR) cout << "*** FATIMA + PLASTIC shared White Rabbit mode ***" << endl;
    else cout << "*** FATIMA + PLASTIC standalone White Rabbit mode ***" << endl;
    

}