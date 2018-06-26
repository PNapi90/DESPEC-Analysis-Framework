#include "PLASTIC_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Detector_System::PLASTIC_Detector_System(){

    //calibration enabled?
    CALIBRATE = false;

    // different codes for PLASTIC MBS words
    add = 2781;
    aa = 170;
    six_eight = 104;
    six_f = 6;
    trailer_code = 187;
    error_code = 238;
    tamex_identifier = 52;
    
    iterator = new int[100];

    coarse_T = new ULong[100];
    fine_T = new ULong[100];
    ch_ID = new unsigned int[100];

    edge_coarse = new ULong**[100];
    edge_fine = new ULong**[100];
    ch_ID_edge = new unsigned int**[100];
    for(int o = 0;o < 100;++o){
        edge_coarse[o] = new ULong*[100];
        edge_fine[o] = new ULong*[100];
        ch_ID_edge[o] = new unsigned int*[100];
        for(int i = 0;i < 100;++i){
            edge_coarse[o][i] = new ULong[2];
            edge_fine[o][i] = new ULong[2];
            ch_ID_edge[o][i] = new unsigned int[2];
        }
    }
}

//---------------------------------------------------------------

PLASTIC_Detector_System::~PLASTIC_Detector_System(){
    for(int i = 0;i < 100;++i){
        for(int j = 0;j < 100;++j){
            delete[] edge_coarse[i][j];
            delete[] edge_fine[i][j];
            delete[] ch_ID_edge[i][j];
        }
        delete[] edge_coarse[i];
        delete[] edge_fine[i];
        delete[] ch_ID_edge[i];
    }
    delete[] edge_coarse;
    delete[] edge_fine;
    delete[] ch_ID_edge;

    delete[] coarse_T;
    delete[] fine_T;
    delete[] ch_ID;
}

//---------------------------------------------------------------


ULong*** PLASTIC_Detector_System::tmp_get_coarse_T(){return edge_coarse;}



//---------------------------------------------------------------

int PLASTIC_Detector_System::tmp_get_am_hits(){return tamex_iter;}

//---------------------------------------------------------------

unsigned int*** PLASTIC_Detector_System::tmp_get_chID(){return ch_ID_edge;}

int* PLASTIC_Detector_System::tmp_get_iterator(){return iterator;}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_Event_data(Data_Stream* data_stream){
    if(CALIBRATE) calibrate_ONLINE();
    else calibrate_OFFLINE();

    //data_stream = (PLASTIC_Data_Stream*) data_stream;

    //data_stream->set_event_data(coarse_T,fine_T,ch_ID,0,0);

    
    //return important information of event
    //data_stream->reset();
    //data_stream->set_amount_of_Events(iterator[tamex_iter]);
    //data_stream->set_event_data(coarse_T,fine_T,ch_ID,coarse_T,fine_T);
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
    
    written = false;

    //check for trigger window (beginning of TAMEX MBS)
    if(tamex_iter == 0){
        cout << hex << *pdata << " 0 "<< endl;
        TRIGGER_WINDOW* window = (TRIGGER_WINDOW*) pdata;
        Pre_Trigger_Window = window->PRE_TRIGG;
        Post_Trigger_Window = window->POST_TRIGG;

        //move to next word
        pdata++;
        //skip padding in stream
        skip_padding();
    }
    cout << hex << *pdata << " 1 " << endl;
    //get tamex_id, sfp_id and trigger type
    TAMEX_CHANNEL_HEADER* head = (TAMEX_CHANNEL_HEADER*) pdata;
    //check if end of TAMEX MBS reached
    if(head->identify != tamex_identifier || head->identify_2 != 0){
        tamex_end = true;
        return;
    }

    if(tamex_id > 0){
        if(head->Tamex_id <= tamex_id[tamex_iter-1]){
            tamex_end = true;
            return;
        }
    }

    sfp_id[tamex_iter] = head->sfp_id;
    trigger_type[tamex_iter] = head->trigger_t;
    tamex_id[tamex_iter] = head->Tamex_id;

    //next word
    pdata++;
    cout << hex << *pdata<< " 2 " << endl;
    //get amount of fired tdcs (without last trailing words)
    TAMEX_FIRED* fire = (TAMEX_FIRED*) pdata;
    am_fired[tamex_iter] = (fire->am_fired)/4 - 2;

    cout << dec <<  "FIRED: " << am_fired[tamex_iter] << endl;

    //next word
    pdata++;
    cout << hex << *pdata << " 3 " << endl;
    //begin of data header
    TAMEX_BEGIN* begin = (TAMEX_BEGIN*) pdata;
    if(begin->aa != aa){
        cerr << "error in TAMEX format! aa not found after fired amount!" << endl;
        exit(0);
    }

    //next word
    pdata++;
    cout << hex << *pdata<< " 4" << endl;
    //get trigger 
    get_trigger();
    cout << hex << *pdata << endl;
    //move on to leading and trailing edges
    if(am_fired[tamex_iter] > 3) get_edges();
    else{
        cout << "-> no edges" << endl;
        no_edges[tamex_iter] = true;
    }
    cout << hex << *pdata << " 5 "<< endl;
    //check errors
    //if(!no_edges[tamex_iter]) pdata++;
    cout << hex << *pdata << " 6 "<< endl;
    check_error();
    cout << hex << *pdata << " 7 "<< endl;
    //checking trailer
    check_trailer();
    cout << hex << *pdata << " 8 "<< endl;
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
    
    if(hold->six_eight != six_f && false){
        cerr << dec << hold->six_eight << endl;
        cerr << "PLACE_HOLDER error (trigger) in TAMEX!" << endl;
        exit(0);
    }

    //next word 
    pdata++;
    cout << hex << *pdata << " t1 "<< endl;

    //extract data
    TAMEX_DATA* data = (TAMEX_DATA*) pdata;
    coarse_T[tamex_iter] = data->coarse_T;
    fine_T[tamex_iter] = data->fine_T;
    ch_ID[tamex_iter] = data->ch_ID;

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
    cout << hex << *pdata << " e1 "<< endl;
    written = false;
    while(no_error_reached()){
        //check place holder in stream
        PLACE_HOLDER* hold = (PLACE_HOLDER*) pdata;

        if(hold->six_eight != six_f && written){
            cerr << "Multiple single channel fire!" << endl;
            //written = false;
            pdata++;
            continue;
        }
        else if(hold->six_eight == six_f) written = false;

        if(hold->six_eight != six_f ){
            cerr << dec << hold->six_eight << endl;
            cerr << "PLACE_HOLDER error (edges) in TAMEX!" << endl;
            exit(0);
        }

        //next word 
        pdata++;
        cout << hex << *pdata << " e "<< endl;


        //extract data
        TAMEX_DATA* data = (TAMEX_DATA*) pdata;

        edge_coarse[tamex_iter][iterator[tamex_iter]][lead] = data->coarse_T;
        edge_fine[tamex_iter][iterator[tamex_iter]][lead] = data->fine_T;
        ch_ID_edge[tamex_iter][iterator[tamex_iter]][lead] = data->ch_ID;

        cout << dec << "edge " << edge_coarse[tamex_iter][iterator[tamex_iter]][lead] << " " << ch_ID_edge[tamex_iter][iterator[tamex_iter]][lead] << endl;

        lead++;
        if(lead == 2){
            //move to next pair of edges
            lead = 0;
            iterator[tamex_iter]++;
        }
        
        written = true;

        //next word
        pdata++;
        cout << hex << *pdata << " e "<< endl;
        loops++;
    }
}

//---------------------------------------------------------------

bool PLASTIC_Detector_System::no_error_reached(){

    TAMEX_ERROR* error = (TAMEX_ERROR*) pdata;
    return error->error != error_code;
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

void PLASTIC_Detector_System::calibrate_ONLINE(){

    //
    PLASTIC_Calibration->get_data(edge_fine,ch_ID_edge,tamex_iter,iterator);

    cal_count++;
    if(cal_count > 50000){
        PLASTIC_Calibration->ONLINE_CALIBRATION();
        Calibration_Done = true;
    }
    /*
    ULong* tmp_container = new ULong[2];

    PLASTIC_Calibration->calibrate(coarse_T,fine_T,ch_ID,am_fired,tamex_id);
    PLASTIC_Calibration->calibrate(coarse_T,fine_T,tmp_container,tamex_id);

    coarse_T = tmp_container[0];
    fine_T = tmp_container[1];

    delete[] tmp_container;
    tmp_container = NULL;
    */
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::calibrate_OFFLINE(){

}

//---------------------------------------------------------------

int* PLASTIC_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

