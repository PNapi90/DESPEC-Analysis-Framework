#include "QDC_TDC_FATIMA.h"

using namespace std;

//---------------------------------------------------------------

QDC_TDC_FATIMA::QDC_TDC_FATIMA(int QDC_amount,int TDC_amount){

    //amount of QDCs and TDCs used for FATIMA setup
    this->QDC_amount = 100;
    this->TDC_amount = 100;
    amount_of_FAT_DETS = 36;
    //possible special setup (more information like Fine Time,...)
    //setup = new Setup_Specifier();
    
    //load map for detector identification from board_id and channel #
    load_board_channel_file();

    //initialize calibration objects
    FATIMA_T_CALIB = new FATIMA_Time_Calibration();
    FATIMA_E_CALIB = new FATIMA_Energy_Calibration();
    
    //create QDC channel objects -> each object belongs to one detector
    QDC_Channels = new QDC_FATIMA_Channel**[QDC_amount];
    for(int i = 0;i < QDC_amount;++i){
        QDC_Channels[i] = new QDC_FATIMA_Channel*[100];
        for(int j = 0;j < 100;++j){
            if(wired_QDC(i,j)) QDC_Channels[i][j] = new QDC_FATIMA_Channel(det_num[i][j],i,j);
            else QDC_Channels[i][j] = NULL;
        }
    }
    
    //create TDC channel objects -> each object belongs to one detector
    TDC_Channels = new TDC_FATIMA_Channel**[TDC_amount];
    for(int i = 0;i < TDC_amount;++i){
        TDC_Channels[i] = new TDC_FATIMA_Channel*[100];
        for(int j = 0;j < 100;++j){
            if(wired_TDC(i,j)) TDC_Channels[i][j] = new TDC_FATIMA_Channel(det_num_tdc[i][j],i,j);
            else TDC_Channels[i][j] = NULL;
        }
    }

    //create FATIMA detectors
    FATIMA_Detectors = new FATIMA_DETECTOR*[amount_of_FAT_DETS];
    for(int i = 0;i < amount_of_FAT_DETS;++i) FATIMA_Detectors[i] = new FATIMA_DETECTOR(i);


}

//---------------------------------------------------------------

QDC_TDC_FATIMA::~QDC_TDC_FATIMA(){

    for(int i = 0;i < amount_of_FAT_DETS;++i) delete FATIMA_Detectors[i];
    delete[] FATIMA_Detectors;

    for(int i = 0;i < TDC_amount;++i){
        for(int j = 0;j < 100;++j) delete TDC_Channels[i][j];
        delete[] TDC_Channels[i];
    }
    delete[] TDC_Channels;

    for(int i = 0;i < QDC_amount;++i){
        for(int j = 0;j < 100;++j) if(wired_QDC(i,j)) delete QDC_Channels[i][j];
        delete[] QDC_Channels[i];
    }
    delete[] QDC_Channels;
    //delete setup;

    delete FATIMA_T_CALIB;
    delete FATIMA_E_CALIB;
}

//---------------------------------------------------------------


void QDC_TDC_FATIMA::load_board_channel_file(){

    const char* format = "%d %d %d %d %d %d";

    //initialize with -1 (-> id not possible)
    for(int i = 0;i < 100;++i){
        for(int j = 0;j < 100;++j){
            det_num[i][j] = -1;
            det_num_tdc[i][j] = -1;
        }
    }

    ifstream file("Configuration_Files/allocation.txt");

    if(file.fail()){
        cerr << "Could not find QDC_FATIMA Board_Channel to DetNum File!" << endl;
        exit(0);
    }

    string line;
    int detector_number,board_id,channel_num,board_id_TDC,TDC_ch,dummy;

    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&detector_number,&board_id,&channel_num,&board_id_TDC,&TDC_ch,&dummy);
        det_num[board_id][channel_num] = detector_number;
        det_num_tdc[board_id_TDC][TDC_ch] = detector_number;
        cout << detector_number << " " <<board_id << " " << channel_num << " " << board_id_TDC << " " << TDC_ch  << endl;
    }
}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::Run_FATIMA_MBS(int* pdata){
    exiter = false;
    this->pdata = pdata;
    no_data = false;

    //check for QDC and TDC header
    QDC_Header* QDChead  = (QDC_Header*) this->pdata;
    TDC_Check* TDChead = (TDC_Check*) this->pdata;

    //reset TDC called bool
    bool TDC_Called = false;

    //loop over FATIMA modules
    while(!TDC_Called){
        //QDC channel empty (check a -> always with QDC, length-> am_channels called (len-4))
        if(QDChead->check_a == 10 && QDChead->length == 4) for(int i = 0;i < 3;++i) this->pdata++;
        //QDC channel filled 
        else if(QDChead->check_a == 10) Check_QDC_DATA(QDChead);
        //TDC code reached
        else if(TDChead->type == 8) TDC_Called = true;

        this->pdata++;

        QDChead = (QDC_Header*) this->pdata;
        TDChead = (TDC_Check*) this->pdata;
    }

    this->pdata--;
    this->pdata--;

    Check_TDC_DATA(); 
    if(exiter) exit(0);
    if(!no_data) Merge_To_Detectors();

}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::Check_TDC_DATA(){
    bool trail = false;

    int check = 0;
    int tdc_board_ID = 0;
    int TDC_ch = 0;

    no_data = true;

    while(!trail){
        
        pdata++;
        

        TDC_Check*p = (TDC_Check*) pdata;
        check = p->type;
        
         // Global Header Condition //
        if( check == 8 ){
            TDC_Glob_Header* gh = (TDC_Glob_Header*) pdata;
            tdc_board_ID = gh->geo;                
        }
        // TDC Header Condition //
        else if( check == 1 ) {}
        // TDC Measurement Condition //
        else if( check == 0 ){
            TDC_Measurement* m = (TDC_Measurement*) pdata;
            TDC_ch = m->channel;
            
            if (!wired_TDC(tdc_board_ID,TDC_ch)) continue;
            
             // Ensures no multihits occur // 
            if (!TDC_Channels[tdc_board_ID][TDC_ch]->get_TDC_Fired() || true){
                
                Fired_TDC_Channels[fired_TDC_amount][0] = tdc_board_ID;
                Fired_TDC_Channels[fired_TDC_amount][1] = TDC_ch;
                fired_TDC_amount++;

                TDC_Channels[tdc_board_ID][TDC_ch]->set_TDC_Fired();
                TDC_Channels[tdc_board_ID][TDC_ch]->set_TDC_Time(25.*(m->measurement)); // 25x to convert into Picoseconds //
                TDC_Channels[tdc_board_ID][TDC_ch]->Calibrate(FATIMA_T_CALIB);

                no_data = false;
            }
        }
        // TDC Trailer Condition // 
        else if ( check == 16 ) trail = true; 
    }
}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::reset_fired_channels(){
    for(int i = 0;i < 100;++i){
        for(int j = 0;j < 2;++j){
            Fired_QDC_Channels[i][j] = -1;
            Fired_TDC_Channels[i][j] = -1;
        }
    }
    fired_QDC_amount = 0;
    fired_TDC_amount = 0;
}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::Check_QDC_DATA(QDC_Header* QDChead){

    //length - 4 to subtract headers
    //int length = QDChead->length - 4;
    
    reset_fired_channels();
    //cout << hex << *pdata << " ";
    pdata++; // Moves to 2nd header value //
    //cout << hex << *pdata << " " << dec << (*pdata & 0xFF) <<endl;
    QDC_Header_2 *QDChead_2 = (QDC_Header_2*) pdata;


    int board_ID = QDChead_2->geo; // Gets Geographical Address //
    int num_Channels = QDChead_2->channels; // Gets Channels fired 0011 means 1&2 fired //
            
     // Loop retrieves channels fired from integer value //
    for(int j = 7; j >= 0; j--){
        if(num_Channels >= pow(2, j)){
            //fill Fired_Channel array with corresponding ids
            if(wired_QDC(board_ID,j)){
                Fired_QDC_Channels[fired_QDC_amount][0] = board_ID; 
                Fired_QDC_Channels[fired_QDC_amount][1] = j;
                //cout << board_ID << " " << j << " | ";
                fired_QDC_amount++;
            }
            num_Channels -= pow(2, j);
        }
    }
    //cout << endl;
    if(fired_QDC_amount == 2) exiter = true;

    pdata++; // Moves to 3rd header value //
  
    pdata++; // Moves to 4th header value //
  
    //int size = 0;
    int active_Channel = 0;
    int active_board = 0;
    double fine_time = 0;
    for(int i = 0;i < fired_QDC_amount;++i){
        //set active board_ID and channel #
        active_board = Fired_QDC_Channels[i][0];
        active_Channel = Fired_QDC_Channels[i][1];
                                
        pdata++; // Moves to 1st data value
      
        QDC_Format_Size* fs = (QDC_Format_Size*) pdata;
        //size = fs->size - 6;
        pdata++; // Moves to 2nd data value

        pdata++; // Moves to 3rd data value

        //if (setup->get_extras() == 1) pdata++; // Moves to 4th data value
        pdata++; // extras == 1! IMPLEMENT!
        

        QDC_Time* t = (QDC_Time*) pdata;
        

        QDC_Channels[active_board][active_Channel]->set_QDC_Time(t->trigger_tag);
        
        //pdata++;
        //pdata++;
        
        pdata++; // Moves to Extras
        
        QDC_Extras* e = (QDC_Extras*) pdata;

	   //fine_time = (t->trigger_tag + (e->ext_trig)<<32) + ((double)(e->fine_time)/1024.);
	
	    fine_time = (((t->trigger_tag)+((e->ext_trig)<<32))) + ((double)(e->fine_time)/1024.);
	
        QDC_Channels[active_board][active_Channel]->set_QDC_Fine_Time(fine_time);
        //QDC_Channels[active_board][active_Channel]->set_QDC_Ext_Time(chNo, (e->ext_trig));
                    
                                
        //fatima.set_QDC_Ext_Time(chNo,((fatima.get_QDC_Time(chNo))+((fatima.get_QDC_Ext_Time(chNo))<<32)));
        //fatima.set_QDC_Fine_Time(chNo,((fatima.get_QDC_Ext_Time(chNo))+(fatima.get_QDC_Fine_Time(chNo))));
                  
		  
        pdata++; // Moves to 6th data value87454dda

        QDC_Data* d = (QDC_Data*) pdata;
        
        QDC_Channels[active_board][active_Channel]->set_QLong_Raw(d->QL); // Gets Q Long data //
        QDC_Channels[active_board][active_Channel]->set_QShort_Raw(d->QS); // Gets Q Short data //
        QDC_Channels[active_board][active_Channel]->Calibrate(FATIMA_E_CALIB,FATIMA_T_CALIB);
        
    }

}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::Merge_To_Detectors(){

    pdata++;
    
    int board_ID_tmp,Ch_num_tmp,board_ID_tmp_TDC,Ch_num_tmp_TDC;

    
    //set fired detector ids for QDCs
    for(int i = 0;i < fired_QDC_amount;++i){
        board_ID_tmp = Fired_QDC_Channels[i][0];
        Ch_num_tmp = Fired_QDC_Channels[i][1];
        fired_Detectors_QDC[i] = det_num[board_ID_tmp][Ch_num_tmp];
    }

    //set fired detector ids for TDCs
    for(int i = 0;i < fired_TDC_amount;++i){
        board_ID_tmp = Fired_TDC_Channels[i][0];
        Ch_num_tmp = Fired_TDC_Channels[i][1];
        fired_Detectors_TDC[i] = det_num_tdc[board_ID_tmp][Ch_num_tmp];
    }


    
    bool match_found = false;
    double** TDC_tmp_data = NULL;
    double** QDC_tmp_data = NULL;

    fired_Dets = 0;

    //check for matches between QDC and TDC
    //-> merge data together in detector obj
    for(int i = 0;i < fired_QDC_amount;++i){
        match_found = false;
        for(int j = 0;j < fired_TDC_amount;++j){
            if(fired_Detectors_QDC[i] == fired_Detectors_TDC[j]){
                
                board_ID_tmp = Fired_QDC_Channels[i][0];
                Ch_num_tmp = Fired_QDC_Channels[i][1];

                board_ID_tmp_TDC = Fired_TDC_Channels[j][0];
                Ch_num_tmp_TDC = Fired_TDC_Channels[j][1];
                
                //extract Energies and Times from TDC and QDC
                TDC_tmp_data = TDC_Channels[board_ID_tmp_TDC][Ch_num_tmp_TDC]->get_Data();
                QDC_tmp_data = QDC_Channels[board_ID_tmp][Ch_num_tmp]->get_Data();
                
                //send data to respective FATIMA detector
                FATIMA_Detectors[fired_Detectors_QDC[i]]->set_Data(QDC_tmp_data,TDC_tmp_data);
                fired_Dets++;
                match_found = true;
                break;
            }
        }
        //no match found -> exiting (perhaps normal?)
        if(!match_found) print_match_error(i);
    }
    for(int i = 0;i < fired_QDC_amount;++i){
        //reset data arrays in channels
        for(int j = 0;j < fired_TDC_amount;++j){
            board_ID_tmp = Fired_QDC_Channels[i][0];
            Ch_num_tmp = Fired_QDC_Channels[i][1];

            board_ID_tmp_TDC = Fired_TDC_Channels[j][0];
            Ch_num_tmp_TDC = Fired_TDC_Channels[j][1];
            
            TDC_Channels[board_ID_tmp_TDC][Ch_num_tmp_TDC]->reset();
            QDC_Channels[board_ID_tmp][Ch_num_tmp]->reset();
        }
    }
}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::print_match_error(int i){
    int board_ID_tmp = Fired_QDC_Channels[i][0];
    
    cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cerr << "FATIMA: QDC with board_ID " << Fired_QDC_Channels[board_ID_tmp][0] << " Ch # ";
    cerr << Fired_QDC_Channels[board_ID_tmp][1] << " -> no TDC match found!" << endl;
    cerr << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    //exit(0);
}

//---------------------------------------------------------------

bool QDC_TDC_FATIMA::wired_QDC(int board_id,int ch_num){
    return det_num[board_id][ch_num] != -1;
}

//---------------------------------------------------------------

bool QDC_TDC_FATIMA::wired_TDC(int board_id,int ch_num){
    return det_num_tdc[board_id][ch_num] != -1;
}

//---------------------------------------------------------------

int QDC_TDC_FATIMA::get_amount_of_fired_Dets(){return fired_Dets;}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::get_Detector_Data(FATIMA_Data_Stream* data_stream){


    data_stream->reset();
    //return Fatima Detector data to FATIMA_Detector_System object
    ULong raw_T,raw_QDC_T,raw_QDC_T_Fine;
    double raw_E;
    int am_hits_det = 0;
    int am_hits_total = 0;

    int j = 0;
    int iter = 0;
    int k = 0;
    for(int i = 0;i < fired_Dets;++i){
        if(FATIMA_Detectors[i]->get_mismatch()){
            data_stream->set_mismatch();
            return;
        }
        j = fired_Detectors_QDC[i];
        k = FATIMA_Detectors[j]->get_amount_of_hits();

        for(int o = 0;o < k;++o){
            raw_E = FATIMA_Detectors[j]->get_E(o);
            raw_T = FATIMA_Detectors[j]->get_T(o);
            raw_QDC_T = FATIMA_Detectors[j]->get_QDC_T(o);
            raw_QDC_T_Fine = FATIMA_Detectors[j]->get_QDC_FT(o);
            am_hits_total = j;
            am_hits_det = k;
            data_stream->set_event_data(raw_E,raw_T,raw_QDC_T,am_hits_total,am_hits_det,j,raw_QDC_T_Fine);
        }
    }
}

//---------------------------------------------------------------

void QDC_TDC_FATIMA::get_Detector_Data(Raw_Event* RAW){

    double* raw_E = new double[fired_Dets];
    ULong* raw_T = new ULong[fired_Dets];
    ULong* raw_QDC_T = new ULong[fired_Dets];
    ULong* raw_QDC_T_Fine = new ULong[fired_Dets];
    int* det_ids = new int[fired_Dets];

    int j = 0;
    int k = 0;

    for(int i = 0;i < fired_Dets;++i){
        j = fired_Detectors_QDC[i];
        k = FATIMA_Detectors[j]->get_amount_of_hits();
        raw_E[i] = FATIMA_Detectors[j]->get_E(0);
        raw_T[i] = FATIMA_Detectors[j]->get_T(0);
        raw_QDC_T[i] = FATIMA_Detectors[j]->get_QDC_T(0);
        raw_QDC_T_Fine[i] = FATIMA_Detectors[j]->get_QDC_FT(0);
        det_ids[i] = j;
    }

    RAW->set_DATA_FATIMA(fired_Dets,raw_E,raw_T,raw_QDC_T,raw_QDC_T_Fine,det_ids);

    delete[] raw_E;
    delete[] raw_T;
    delete[] raw_QDC_T;
    delete[] raw_QDC_T_Fine;
    delete[] det_ids;
}



//---------------------------------------------------------------

int QDC_TDC_FATIMA::get_increase(){return increase;}

//---------------------------------------------------------------

int* QDC_TDC_FATIMA::get_pdata(){return pdata;}

//---------------------------------------------------------------
