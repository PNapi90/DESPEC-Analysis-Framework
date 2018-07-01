#include "FATIMA_Detector_System.h"

#include <cstdlib>

using namespace std;

//---------------------------------------------------------------

FATIMA_Detector_System::FATIMA_Detector_System(){

    //set amount of QDCs and TDCs

    max_am_dets = 60;

    fired_QDC_amount = 0;

    QLong = new double[max_am_dets];
    QShort = new double[max_am_dets];

    QDC_Time_Coarse = new ULong[max_am_dets];
    QDC_Time_Fine = new ULong[max_am_dets];

    TDC_Time = new ULong[max_am_dets];

    det_ids_QDC = new int[max_am_dets];
    det_ids_TDC = new int[max_am_dets];

    det_ID = new int*[100];
    det_ID_TDC = new int*[100];
    for(int i = 0;i < 100;++i){
    	det_ID[i] = new int[100];
    	det_ID_TDC[i] = new int[100];
    }

    FATIMA_T_CALIB = new FATIMA_Time_Calibration();
    FATIMA_E_CALIB = new FATIMA_Energy_Calibration();

    load_board_channel_file();
    he_iter = 0;
    
}

//---------------------------------------------------------------

FATIMA_Detector_System::~FATIMA_Detector_System(){

	for(int i = 0;i < 100;++i){
		delete[] det_ID[i];
		delete[] det_ID_TDC[i];
	}

	delete[] det_ID_TDC;
	delete[] det_ID;

	delete[] det_ids_QDC;
    delete[] det_ids_TDC;

	delete[] QLong;
	delete[] QShort;
	delete[] QDC_Time_Coarse;
	delete[] QDC_Time_Fine;
	delete[] TDC_Time;

	delete FATIMA_T_CALIB;
	delete FATIMA_E_CALIB;
}

//---------------------------------------------------------------

void FATIMA_Detector_System::load_board_channel_file(){

    const char* format = "%d %d %d %d %d %d";

    //initialize with -1 (-> id not possible)
    for(int i = 0;i < 100;++i){
        for(int j = 0;j < 100;++j){
            det_ID[i][j] = -1;
            det_ID_TDC[i][j] = -1;
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
        det_ID[board_id][channel_num] = detector_number;
        det_ID_TDC[board_id_TDC][TDC_ch] = detector_number;
    }
}

//---------------------------------------------------------------

void FATIMA_Detector_System::get_Event_data(Raw_Event* RAW){
    bool called = false;
    int a = 0;
    for(int i = 0;i < fired_TDC_amount;++i){
        a = det_ids_TDC[i];
        if(a == 50 || a == 51) called = true;
        if(called) break;
    }

    if(called){
        int show_val = 0;
        for(int i = 0;i < fired_TDC_amount;++i){
            a = det_ids_TDC[i];  
            if(TDC_Time[0] > 0) show_val = 0;
            if(TDC_Time[1] > 0) show_val = 1;
            if(a == 51) cout << "VALUES " << a << " " << TDC_Time[a] << " " << TDC_Time[0] << " " <<TDC_Time[1] <<  " " << (double) (TDC_Time[a]) - (double)(TDC_Time[show_val]) << endl;
        }
        cout << "END" << endl;
    }

	RAW->set_DATA_FATIMA(fired_QDC_amount,fired_TDC_amount,QLong,QShort,TDC_Time,QDC_Time_Coarse,QDC_Time_Fine,det_ids_QDC,det_ids_TDC);
    //QDC_TDC->get_Detector_Data(RAW);
}

//---------------------------------------------------------------

void FATIMA_Detector_System::Process_MBS(int* pdata){

    this->pdata = pdata;
    //extract QDC and TDC informations from MBS and assign to detectors
    exiter = false;

    no_data = false;

    //check for QDC and TDC header
    QDC_Header* QDChead  = (QDC_Header*) this->pdata;
    TDC_Check* TDChead = (TDC_Check*) this->pdata;

    //reset TDC called bool
    bool TDC_Called = false;
    QDC_DATA = false;

    //loop over FATIMA modules
    while(!TDC_Called){
        //QDC channel empty (check a -> always with QDC, length-> am_channels called (len-4))
        if(QDChead->check_a == 10 && QDChead->length == 4) for(int i = 0;i < 3;++i) this->pdata++;
        //QDC channel filled 
        else if(QDChead->check_a == 10){
            QDC_DATA = true;
            Check_QDC_DATA(QDChead);
        }
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
}

//---------------------------------------------------------------

void FATIMA_Detector_System::reset_fired_channels(){
    fired_QDC_amount = 0;
    fired_TDC_amount = 0;
    for(int i = 0;i < max_am_dets;++i){
        QLong[i] = -1;
        QShort[i] = -1;
        QDC_Time_Coarse[i] = 0;
        QDC_Time_Fine[i] = 0;
        TDC_Time[i] = 0;
    }
}


//---------------------------------------------------------------

void FATIMA_Detector_System::Check_QDC_DATA(QDC_Header* QDChead){

    reset_fired_channels();

    pdata++; // Moves to 2nd header value //
    QDC_Header_2 *QDChead_2 = (QDC_Header_2*) pdata;


    int board_ID = QDChead_2->geo; // Gets Geographical Address //
    int num_Channels = QDChead_2->channels; // Gets Channels fired 0011 means 1&2 fired //
    int tmp_tmp = QDChead_2->channels & 0xf;
     // Loop retrieves channels fired from integer value //
    for(int j = 7; j >= 0; j--){
        if(num_Channels >= pow(2, j)){
            //fill Fired_Channel array with corresponding ids
            if(wired_QDC(board_ID,j)){
                Fired_QDC_Channels[fired_QDC_amount][0] = board_ID; 
                Fired_QDC_Channels[fired_QDC_amount][1] = j;
                fired_QDC_amount++;
            }
            num_Channels -= pow(2, j);
        }
    }
    if(tmp_tmp == 3) cout << hex << *pdata << " " << tmp_tmp << endl;
    if(fired_QDC_amount == 2) exiter = true;

    pdata += 2; // Moves from 2nd to 4th header value //
    
    //int size = 0;
    int active_Channel = 0;
    int active_board = 0;
    int active_det = 0;
    double fine_time = 0;

    for(int i = 0;i < fired_QDC_amount;++i){
        //set active board_ID and channel #
        active_board = Fired_QDC_Channels[i][0];
        active_Channel = Fired_QDC_Channels[i][1];

        active_det = det_ID[active_board][active_Channel];
        det_ids_QDC[i] = active_det;
                                
        pdata++; // Moves to 1st data value
      
        QDC_Format_Size* fs = (QDC_Format_Size*) pdata;
        
        pdata += 3; //Moves to QDC time data

        QDC_Time* t = (QDC_Time*) pdata;
        QDC_Time_Coarse[active_det] = t->trigger_tag;
        
        
        pdata++; // Moves to Extras
        
        QDC_Extras* e = (QDC_Extras*) pdata;	

	    fine_time = (((t->trigger_tag) + ((e->ext_trig) << 32)));
	    fine_time += ((double)(e->fine_time)/1024.);
	
		QDC_Time_Fine[active_det] = fine_time;
        
        pdata++; // Moves to 6th data value87454dda

        QDC_Data* d = (QDC_Data*) pdata;
        
        QLong[active_det] = d->QL; // Gets Q Long data //
        QShort[active_det] = d->QS; // Gets Q Short data //
        
        Calibrate_QDC(active_det);
    }
}


//---------------------------------------------------------------

void FATIMA_Detector_System::Check_TDC_DATA(){
    
    bool trail = false;

    int check = 0;
    int tdc_board_ID = 0;
    int TDC_ch = 0;
    int active_det = 0;
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
            
            //if (!wired_TDC(tdc_board_ID,TDC_ch) && QDC_DATA) continue;
            if(false) cout << " " << endl;
            else{
                active_det = det_ID_TDC[tdc_board_ID][TDC_ch];

                det_ids_TDC[fired_TDC_amount] = active_det;
                //cout << "DET!" << active_det << endl;
                if(active_det >= 50){
                    cout << "HE " << active_det << " " <<  he_iter << endl;
                    cout << dec << QLong[0] << " " << QLong[1] << " " << TDC_Time[0] << " " << TDC_Time[1]  << endl;
                    cout <<  25*m->measurement << endl;
                    he_iter++;
                }

                fired_TDC_amount++;

                TDC_Time[active_det] = 25*m->measurement;
                Calibrate_TDC(active_det);

                no_data = false;

            }
        }
        // TDC Trailer Condition // 
        else if ( check == 16 ) trail = true; 
    }
}

//---------------------------------------------------------------

bool FATIMA_Detector_System::check_additional(int active){return (active == 51);}

//---------------------------------------------------------------

bool FATIMA_Detector_System::wired_QDC(int board_id,int ch_num){
    return det_ID[board_id][ch_num] != -1;
}

//---------------------------------------------------------------

bool FATIMA_Detector_System::wired_TDC(int board_id,int ch_num){
    return det_ID_TDC[board_id][ch_num] != -1;
}

//---------------------------------------------------------------

void FATIMA_Detector_System::Calibrate_QDC(int id){
	QLong[id] = FATIMA_E_CALIB->Calibrate(QLong[id],id);
	QShort[id] = FATIMA_E_CALIB->Calibrate(QShort[id],id);
	
	QDC_Time_Coarse[id] = FATIMA_T_CALIB->Calibrate_QDC(QDC_Time_Coarse[id],id);
	QDC_Time_Fine[id] = FATIMA_T_CALIB->Calibrate_QDC(QDC_Time_Fine[id],id);

}

//---------------------------------------------------------------

void FATIMA_Detector_System::Calibrate_TDC(int id){
	TDC_Time[id] = FATIMA_T_CALIB->Calibrate(TDC_Time[id],id);
}

//---------------------------------------------------------------

int* FATIMA_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------