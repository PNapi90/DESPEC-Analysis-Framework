#include "PLASTIC_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Detector_System::PLASTIC_Detector_System(){
    data_stream = nullptr;

    tamex = new TAMEX();
}

//---------------------------------------------------------------

PLASTIC_Detector_System::~PLASTIC_Detector_System(){
    delete tamex;
}

//---------------------------------------------------------------

double** PLASTIC_Detector_System::get_Event_data(){
    //return important information of event
    return data_stream;
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::Process_MBS(int* pdata){
    PLASTIC_Header* PLHead = (PLASTIC_Header*) pdata;
    if(PLHead->a == 0){
        
    }
}

//---------------------------------------------------------------

