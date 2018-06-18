#include "FATIMA_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Detector_System::FATIMA_Detector_System(){

    //set amount of QDCs and TDCs
    am_QDCs = 5;
    am_TDCs = 20;


    //initialize White Rabbit and QDC_TDC objects
    WR = new White_Rabbit();
    QDC_TDC = new QDC_TDC_FATIMA(am_QDCs,am_TDCs); 

    data_stream = nullptr;
}

//---------------------------------------------------------------

FATIMA_Detector_System::~FATIMA_Detector_System(){
    delete WR;
    delete QDC_TDC;

}

//---------------------------------------------------------------

double** FATIMA_Detector_System::get_Event_data(){
    //return important information of event (energies,triggered detectors,multiplicity,...)
    return data_stream;
}

//---------------------------------------------------------------

void FATIMA_Detector_System::QDC_Processing(int* pdata){
    //reset data_stream pointer
    data_stream = nullptr;

    //extract White Rabbit from MBS stream
    WR_T = WR->get_White_Rabbit(pdata);
    //extract QDC and TDC informations from MBS and assign to detectors
    QDC_TDC->Run_FATIMA_MBS(pdata);

    //get amount of fired detectors
    fired_amount = QDC_TDC->get_amount_of_fired_Dets();
    //assign outgoing data stream (Energies, time and QDC time)
    data_stream = QDC_TDC->get_Detector_Data();
}