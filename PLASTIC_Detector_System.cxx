#include "PLASTIC_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Detector_System::PLASTIC_Detector_System(){

    add = 2781;
    aa = 170;
    six_eight = 104
    trailer_code = 3138451013;
    
    coarse_T = new unsigned long*[100];
    fine_T = new unsigned long*[100];
    ch_ID = new unsigned int*[100];
    
    for(int i = 0;i < 100;++i){
        coarse_T[i] = new unsigned long[2];
        fine_T[i] = new unsigned long[2];
        ch_ID[i] = new unsigned int[2];
    }
}

//---------------------------------------------------------------

PLASTIC_Detector_System::~PLASTIC_Detector_System(){
    for(int i = 0;i < 100;++i){
        delete[] coarse_T[i];
        delete[] fine_T[i];
        delete[] ch_ID[i];
    }
    delete[] coarse_T;
    delete[] fine_T;
    delete[] ch_ID;
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_Event_data(Data_Stream* data_stream){
    //return important information of event
    data_stream->set_ID("PLASTIC");
    data_stream->set_amount_of_Events(iterator);
    data_stream->set_event_data(coarse_T,fine_T,ch_ID,coarse_T_trigger,fine_T_trigger);
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::Process_MBS(int* pdata){

    //reset iterator
    iterator = 0;

    //check for trigger window (beginning of TAMEX MBS)
    TRIGGER_WINDOW* window = (TRIGGER_WINDOW*) pdata;
    Pre_Trigger_Window = window->PRE_TRIGG;
    Post_Trigger_Window = window->POST_TRIGG;

    //move to next word
    pdata++;
    //skip padding in stream
    skip_padding(pdata);

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
    get_trigger(pdata);

    //move on to leading and trailing edges
    get_edges(pdata);

    //check errors
    check_error(pdata);

    //checking trailer
    check_trailer(pdata);

    //calibrate times
    calibrate();
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::skip_padding(int* pdata){
    //skip padding by checking words for add
    bool still_padding = true;
    while(still_padding){
        PADDING* padding = (PADDING*) pdata;
        if(padding->add == add) pdata++;
        else still_padding = false;
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_trigger(int* pdata){
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

void PLASTIC_Detector_System::get_edges(int* pdata){
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

void PLASTIC_Detector_System::check_error(int* pdata){
    //next word
    pdata++;

    TAMEX_ERROR* error = (TAMEX_ERROR*) pdata;
    if(error->error != error_code){
        cerr << "wrong error header in TAMEX!" << endl;
        exit(0);
    }
    if(error->err_code != 0){
        cerr << "Error (not known) in TAMEX occured" << endl;
        exit(0);
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::check_trailer(int* pdata){
    //next word
    pdata++;

    TAMEX_TRAILER* trailer = (TAMEX_TRAILER*) pdata;

    if(trailer->trailer != trailer_code){
        cerr << "Unknown TAMEX trailer format!" << endl;
        exit(0);
    }

}

//---------------------------------------------------------------
