#include "PLASTIC_Detector_System.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Detector_System::PLASTIC_Detector_System(){

    //calibration enabled?
    get_Calib_type();

    PLASTIC_Calibration = new PLASTIC_Calibrator(CALIBRATE);

    // different codes for PLASTIC MBS words
    add = 2781;
    aa = 170;
    six_eight = 104;
    six_f = 6;
    trailer_code = 187;
    error_code = 238;
    tamex_identifier = 52;
    
    iterator = new int[100];
    for(int i = 0;i < 100;++i) iterator[i] = 0;

    tamex_iter = 0;

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

int PLASTIC_Detector_System::tmp_get_am_hits(){
    int h = 0;
    for(int i = 0;i < tamex_iter-1;++i) h += iterator[i];
    return h;
}

//---------------------------------------------------------------

unsigned int*** PLASTIC_Detector_System::tmp_get_chID(){return ch_ID_edge;}

int* PLASTIC_Detector_System::tmp_get_iterator(){return iterator;}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_Event_data(Data_Stream* data_stream){
    if(CALIBRATE) calibrate_ONLINE();
    else calibrate_OFFLINE();
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::Process_MBS(int* pdata){

    this->pdata = pdata;
    //reset old iterator array
    for(int i = 0;i < tamex_iter;++i) iterator[i] = 0;

    tamex_end = false;
    tamex_iter = 0;
    while(!tamex_end){
        Process_TAMEX();
        tamex_iter++;
        this->pdata++;
    }
    
    if(CALIBRATE) calibrate_ONLINE();
    else calibrate_OFFLINE();
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::Process_TAMEX(){

    //reset iterator[tamex_iter]
    iterator[tamex_iter] = 0;
    no_edges[tamex_iter] = false;
    
    written = false;

    //check for trigger window (beginning of TAMEX MBS)
    if(tamex_iter == 0){
        TRIGGER_WINDOW* window = (TRIGGER_WINDOW*) pdata;
        Pre_Trigger_Window = window->PRE_TRIGG;
        Post_Trigger_Window = window->POST_TRIGG;

        //move to next word
        pdata++;
        //skip padding in stream
        skip_padding();
    }
    //get tamex_id, sfp_id and trigger type
    TAMEX_CHANNEL_HEADER* head = (TAMEX_CHANNEL_HEADER*) pdata;
    
    //check if end of TAMEX MBS reached
    bool ongoing = (head->identify == tamex_identifier) && (head->identify_2 == 0);
    
    if(!ongoing){
        tamex_end = true;
        return;
    }
    if(tamex_iter > 0){
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
    //get amount of fired tdcs (without last trailing words)
    TAMEX_FIRED* fire = (TAMEX_FIRED*) pdata;
    am_fired[tamex_iter] = (fire->am_fired)/4 - 2;

    
    //next word
    pdata++;
    //begin of data header
    TAMEX_BEGIN* begin = (TAMEX_BEGIN*) pdata;
    if(begin->aa != aa){
        cerr << "error in TAMEX format! 0xaa...... word not found after fired amount!" << endl;
        exit(0);
    }

    //next word
    pdata++;
    //get trigger 
    get_trigger();
    //move on to leading and trailing edges
    if(am_fired[tamex_iter] > 3) get_edges();
    else no_edges[tamex_iter] = true;
    
    //check errors
    //if(!no_edges[tamex_iter]) pdata++;
    check_error();
    //checking trailer
    check_trailer();
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
    written = false;
    while(no_error_reached()){
        //check place holder in stream
        PLACE_HOLDER* hold = (PLACE_HOLDER*) pdata;

        if(hold->six_eight != six_f && written){
            //cerr << "***Multiple single channel fire***" << endl;
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

        //extract data
        TAMEX_DATA* data = (TAMEX_DATA*) pdata;

        edge_coarse[tamex_iter][iterator[tamex_iter]][lead] = data->coarse_T;
        edge_fine[tamex_iter][iterator[tamex_iter]][lead] = data->fine_T;
        ch_ID_edge[tamex_iter][iterator[tamex_iter]][lead] = data->ch_ID;

        lead++;
        if(lead == 2){
            //move to next pair of edges
            lead = 0;
            iterator[tamex_iter]++;
        }
        
        written = true;

        //next word
        pdata++;
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

    //send data to ROOT histograms in Calibrator object
    PLASTIC_Calibration->get_data(edge_fine,ch_ID_edge,2,iterator);

    cal_count++;
    if(cal_count % 1000 == 0){
        cout << dec << "=========================\n";
        cout << cal_count << endl;
        cout << dec << "=========================" << endl;
    }
    Calibration_Done = false;

    //if critical amount of calibration data reached
    //=> do ONLINE calibration and quit program
    if(cal_count > 10000){
        PLASTIC_Calibration->ONLINE_CALIBRATION();
        Calibration_Done = true;
    }
}

//---------------------------------------------------------------

void PLASTIC_Detector_System::calibrate_OFFLINE(){

}

//---------------------------------------------------------------

int* PLASTIC_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

bool PLASTIC_Detector_System::calibration_done(){return Calibration_Done;}

//---------------------------------------------------------------

void PLASTIC_Detector_System::get_Calib_type(){
    ifstream data("Configuration_Files/PLASTIC_CALIB_FILE.txt");
    if(data.fail()){
        cerr << "Could not find Calibration type file for PLASTIC" << endl;
        exit(0);
    }
    string line;
    const char* format = "%s %d";
    char s[100];
    int val;

    bool FORCE = false;

    while(data.good()){
        getline(data,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&s,&val);
        if(string(s) == string("ONLINE")) CALIBRATE = (val == 1);
        if(string(s) == string("FORCE")) FORCE = (val == 1);
    }
    data.close();

    //only FORCE possible, if ONLINE active
    FORCE = (CALIBRATE) ? FORCE : false;

    //if FORCE == false -> rewrite config file to OFFLINE mode
    if(!FORCE){
        ofstream out("Configuration_Files/PLASTIC_CALIB_FILE.txt");
        out << "#PLASTIC calibration type file" << endl;
        out << "#The file will be changed to OFFLINE after its been read" << endl;
        out << "#-> to force multiple ONLINE calibrations, set FORCE to 1" << endl;
        out << "#ONLINE Calibration and FORCE (1 = yes, 0 = no)" << endl;
        out << "ONLINE\t\t0" << endl;
        out << "FORCE\t\t0" << endl;
        out.close(); 
    }
    else{
        cout << endl;
        cout << "######################################################################" << endl;
        cout << "!!! ONLINE ANALYSIS IN FORCED MODE" << endl;
        cout << "!!! TO DISABLE, CHECK PLASTIC_CALIB_FILE IN Configuration_Files" << endl;
        cout << "######################################################################" << endl;
        cout << endl;
    }

}

//---------------------------------------------------------------