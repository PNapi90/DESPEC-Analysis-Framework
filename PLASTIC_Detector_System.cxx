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
    //check for trigger window (beginning of TAMEX MBS)
    TRIGGER_WINDOW* window = (TRIGGER_WINDOW*) pdata;
    Pre_Trigger_Window = window->PRE_TRIGG;
    Post_Trigger_Window = window->POST_TRIGG;

    //move to next word
    pdata++;
    //skip padding in stream
    skip_padding();

    //get tamex_id, sfp_id and trigger type
    TAMEX_CHANNEL_HEADER* head = (TAMEX_CHANNEL_HEADER*) pdata;
    sfp_id = head->sfp_id;
    trigger_type = head->trigger_t;

    //next word
    pdata++;

    //get amount of fired tdcs (without last trailing words)
    TAMEX_FIRED* fire = (TAMEX_FIRED*) pdata;
    am_fired = fire->am_fired - 2;

    //next word
    pdata++;

    //begin of data header
    TAMEX_BEGIN* begin = (TAMEX_BEGIN*) pdata;
    if(begin->aa != aa){
        cerr << "error in TAMEX format! aa not found after fired amount!" << endl;
        exit(0);
    }

    //next word
    pdata++;

    //get trigger 
    get_trigger();

    //move on to leading and trailing edges
    get_edges();

    //check errors
    check_error();




}

//---------------------------------------------------------------

void PLASTIC_Detector_System::skip_padding(){
    //skip padding by checking words for add
    bool still_padding = true;
    while(still_padding){
        PADDING* padding = (PADDING*) pdata;
        if(padding->add == add) pdata++;
        else still_padding = false;
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_trigger(){
    //check place holder in stream
    PLACE_HOLDER* hold = (PLACE_HOLDER*) pdata;
    if(hold->six_eight != six_eight){
        cerr << "PLACE_HOLDER error in TAMEX!" << endl;
        exit(0);
    }

    //next word 
    pdata++;

    //extract data
    TAMEX_DATA* data = (TAMEX_DATA*) pdata;
    coarse_T_trigger = data->coarse_T;
    fine_T_trigger = data->fine_T;
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_edges(){
    //set iterator of edges to 0
    iterator = 0;

    //lead = 0 -> leading edge, lead = 1 -> trailing edge
    int lead = 0;
    //loop over remaining words (getting leading and trailing edge data)
    for(int i = 0;i < am_fired-2;++i){
        //check place holder in stream
        PLACE_HOLDER* hold = (PLACE_HOLDER*) pdata;
        if(hold->six_eight != six_eight){
            cerr << "PLACE_HOLDER error in TAMEX!" << endl;
            exit(0);
        }

        //next word 
        pdata++;

        //extract data
        TAMEX_DATA* data = (TAMEX_DATA*) pdata;

        coarse_T[iterator][lead] = data->coarse_T;
        fine_T[iterator][lead] = data->fine_T;
        ch_ID[iterator][lead] = data->ch_ID;

        lead++;
        if(lead == 2){
            //move to next pair of edges
            lead = 0;
            iterator++;
        }

        //next word
        pdata++;
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::check_error
