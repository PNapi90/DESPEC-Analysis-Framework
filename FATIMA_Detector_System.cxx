#include "FATIMA_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Detector_System::FATIMA_Detector_System(){

    //set amount of QDCs and TDCs
    am_QDCs = 20;
    am_TDCs = 20;

    //initialize White Rabbit and QDC_TDC objects
    QDC_TDC = new QDC_TDC_FATIMA(am_QDCs,am_TDCs); 
}

//---------------------------------------------------------------

FATIMA_Detector_System::~FATIMA_Detector_System(){
    delete QDC_TDC;

}

//---------------------------------------------------------------

void FATIMA_Detector_System::get_Event_data(Data_Stream* data_stream){

    //return important information of event (energies,triggered detectors,multiplicity,...)
    QDC_TDC->get_Detector_Data((FATIMA_Data_Stream*) data_stream);
}


//---------------------------------------------------------------

void FATIMA_Detector_System::get_Event_data(Raw_Event* RAW){
    QDC_TDC->get_Detector_Data(RAW);
    //return important information of event (energies,triggered detectors,multiplicity,...)
    //QDC_TDC->get_Detector_Data((FATIMA_Data_Stream*) data_stream);
}

//---------------------------------------------------------------

void FATIMA_Detector_System::Process_MBS(int* pdata){

    this->pdata = pdata;
    //extract QDC and TDC informations from MBS and assign to detectors
    QDC_TDC->Run_FATIMA_MBS(pdata);

    //get amount of fired detectors
    fired_amount = QDC_TDC->get_amount_of_fired_Dets();
}
//---------------------------------------------------------------

int* FATIMA_Detector_System::get_pdata(){
    pdata = QDC_TDC->get_pdata();
    return pdata;
}

//---------------------------------------------------------------