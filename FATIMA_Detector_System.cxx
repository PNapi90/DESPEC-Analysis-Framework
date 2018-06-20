#include "FATIMA_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Detector_System::FATIMA_Detector_System(){

    //set amount of QDCs and TDCs
    am_QDCs = 5;
    am_TDCs = 20;

    //initialize White Rabbit and QDC_TDC objects
    QDC_TDC = new QDC_TDC_FATIMA(am_QDCs,am_TDCs,shared_WR); 

    data_stream = nullptr;
}

//---------------------------------------------------------------

FATIMA_Detector_System::~FATIMA_Detector_System(){
    delete QDC_TDC;

}

//---------------------------------------------------------------

void FATIMA_Detector_System::get_Event_data(Data_Stream* data_stream){
    //return important information of event (energies,triggered detectors,multiplicity,...)
    data_stream->set_ID("FATIMA");
    data_stream->set_amount_of_Events(fired_amount);
    data_stream->set_event_data(raw_data_stream);

}

//---------------------------------------------------------------

void FATIMA_Detector_System::Process_MBS(int* pdata){

    //extract QDC and TDC informations from MBS and assign to detectors
    QDC_TDC->Run_FATIMA_MBS(pdata);

    //get amount of fired detectors
    fired_amount = QDC_TDC->get_amount_of_fired_Dets();
    //assign outgoing data stream (Energies, time and QDC time)
    raw_data_stream = QDC_TDC->get_Detector_Data();
}

//---------------------------------------------------------------