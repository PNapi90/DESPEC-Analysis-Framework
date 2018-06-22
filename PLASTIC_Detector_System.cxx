#include "PLASTIC_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Detector_System::PLASTIC_Detector_System(){

    // different codes for PLASTIC MBS words
    add = 2781;
    aa = 170;
    six_eight = 104;
    six_f = 111;
    trailer_code = 187;
    error_code = 238;
    tamex_identifier = 52;
    

    coarse_T_Trigger = new unsigned long[100];
    fine_T_Trigger = new unsigned long[100];

    coarse_T = new unsigned long**[100];
    fine_T = new unsigned long**[100];
    ch_ID = new unsigned int**[100];
    for(int o = 0;o < 100;++o){
        coarse_T[o] = new unsigned long*[100];
        fine_T[o] = new unsigned long*[100];
        ch_ID[o] = new unsigned int*[100];
        for(int i = 0;i < 100;++i){
            coarse_T[o][i] = new unsigned long[2];
            fine_T[o][i] = new unsigned long[2];
            ch_ID[o][i] = new unsigned int[2];
        }
    }
}

//---------------------------------------------------------------

PLASTIC_Detector_System::~PLASTIC_Detector_System(){
    for(int i = 0;i < 100;++i){
        for(int j = 0;j < 100;++j){
            delete[] coarse_T[i][j];
            delete[] fine_T[i][j];
            delete[] ch_ID[i][j];
        }
        delete[] coarse_T[i];
        delete[] fine_T[i];
        delete[] ch_ID[i];
    }
    delete[] coarse_T;
    delete[] fine_T;
    delete[] ch_ID;

    delete[] coarse_T_Trigger;
    delete[] fine_T_Trigger;
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_Event_data(Data_Stream* data_stream){
    return;
    //return important information of event
    //data_stream->set_ID("PLASTIC");
    //data_stream->set_amount_of_Events(iterator[tamex_iter]);
    //data_stream->set_event_data(coarse_T,fine_T,ch_ID,coarse_T_Trigger,fine_T_Trigger);
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::Process_MBS(int* pdata){

    this->pdata = pdata;

    tamex_end = false;
    tamex_iter = 0;
    while(!tamex_end){
        Process_TAMEX();
        cout << "- - - - - - -" <<endl;
        tamex_iter++;
        this->pdata++;
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::Process_TAMEX(){

    //reset iterator[tamex_iter]
    iterator[tamex_iter] = 0;
    no_edges[tamex_iter] = false;
    
    //check for trigger window (beginning of TAMEX MBS)
    if(tamex_iter == 0){
        cout << hex << *pdata << endl;
        TRIGGER_WINDOW* window = (TRIGGER_WINDOW*) pdata;
        Pre_Trigger_Window = window->PRE_TRIGG;
        Post_Trigger_Window = window->POST_TRIGG;

        //move to next word
        pdata++;
        //skip padding in stream
        skip_padding();
    }
    cout << hex << *pdata << endl;
    //get tamex_id, sfp_id and trigger type
    TAMEX_CHANNEL_HEADER* head = (TAMEX_CHANNEL_HEADER*) pdata;
    //check if end of TAMEX MBS reached
    if(head->identify != tamex_identifier){
        tamex_end = true;
        return;
    }
    sfp_id[tamex_iter] = head->sfp_id;
    trigger_type[tamex_iter] = head->trigger_t;
    tamex_id[tamex_iter] = head->Tamex_id;

    //next word
    pdata++;
    cout << hex << *pdata << endl;
    //get amount of fired tdcs (without last trailing words)
    TAMEX_FIRED* fire = (TAMEX_FIRED*) pdata;
    am_fired[tamex_iter] = (fire->am_fired)/4 - 2;

    cout << dec <<  "FIRED: " << am_fired[tamex_iter] << endl;

    //next word
    pdata++;
    cout << hex << *pdata << endl;
    //begin of data header
    TAMEX_BEGIN* begin = (TAMEX_BEGIN*) pdata;
    if(begin->aa != aa){
        cerr << "error in TAMEX format! aa not found after fired amount!" << endl;
        exit(0);
    }

    //next word
    pdata++;
    cout << hex << *pdata << endl;
    //get trigger 
    get_trigger();
    cout << hex << *pdata << endl;
    //move on to leading and trailing edges
    if(am_fired[tamex_iter] > 3) get_edges();
    else no_edges[tamex_iter] = true;
    cout << hex << *pdata << endl;
    //check errors
    //if(no_edges[tamex_iter]) pdata++;
    cout << hex << *pdata << endl;
    check_error();
    cout << hex << *pdata << endl;
    //checking trailer
    check_trailer();
    cout << hex << *pdata << endl;
    cout << "Checked trailer " << endl;
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
    if(hold->six_eight != six_f){
        cerr << dec << hold->six_eight << endl;
        cerr << "PLACE_HOLDER error (trigger) in TAMEX!" << endl;
        exit(0);
    }

    //next word 
    pdata++;
    cout << hex << *pdata << endl;

    //extract data
    TAMEX_DATA* data = (TAMEX_DATA*) pdata;
    coarse_T_Trigger[tamex_iter] = data->coarse_T;
    fine_T_Trigger[tamex_iter] = data->fine_T;

    //next word
    pdata++;
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_edges(){
    //set iterator[tamex_iter] of edges to 0
    iterator[tamex_iter] = 0;

    //lead = 0 -> leading edge, lead = 1 -> trailing edge
    int lead = 0;
    //loop over remaining words (getting leading and trailing edge data)
    int loops = 0;
    while(loops < (am_fired[tamex_iter]-3)/2){
        //check place holder in stream
        PLACE_HOLDER* hold = (PLACE_HOLDER*) pdata;
        if(hold->six_eight != six_f){
            cerr << dec << hold->six_eight << endl;
            cerr << "PLACE_HOLDER error (edges) in TAMEX!" << endl;
            exit(0);
        }

        //next word 
        pdata++;

        //extract data
        TAMEX_DATA* data = (TAMEX_DATA*) pdata;

        coarse_T[tamex_iter][iterator[tamex_iter]][lead] = data->coarse_T;
        fine_T[tamex_iter][iterator[tamex_iter]][lead] = data->fine_T;
        ch_ID[tamex_iter][iterator[tamex_iter]][lead] = data->ch_ID;

        lead++;
        if(lead == 2){
            //move to next pair of edges
            lead = 0;
            iterator[tamex_iter]++;
        }

        //next word
        pdata++;
        loops++;
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::check_error(){
    //next word
    //pdata++;

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

void PLASTIC_Detector_System::check_trailer(){
    //next word
    pdata++;

    TAMEX_TRAILER* trailer = (TAMEX_TRAILER*) pdata;

    if(trailer->trailer != trailer_code){
        cerr << "Unknown TAMEX trailer format!" << endl;
        exit(0);
    }

}

//---------------------------------------------------------------

void PLASTIC_Detector_System::calibrate(){
    return;
    /*
    ULong* tmp_container = new ULong[2];

    PLASTIC_Calibration->calibrate(coarse_T,fine_T,ch_ID,am_fired,tamex_id);
    PLASTIC_Calibration->calibrate(coarse_T_Trigger,fine_T_Trigger,tmp_container,tamex_id);

    coarse_T_Trigger = tmp_container[0];
    fine_T_Trigger = tmp_container[1];

    delete[] tmp_container;
    tmp_container = NULL;
    */
}

//---------------------------------------------------------------

int* PLASTIC_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------