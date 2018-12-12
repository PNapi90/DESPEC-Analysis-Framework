//Changes, 24.07.18 MR

//b) changed allocation file name from allocation.txt to
//	 FATIMA_allocation.txt


#include "FATIMA_Detector_System.h"

#include <cstdlib>

using namespace std;

//---------------------------------------------------------------

FATIMA_Detector_System::FATIMA_Detector_System(){

    FAT_evt = 0;
    unknown_header_counter = 0;
    //set amount of QDCs and TDCs
    max_am_dets = 60;

    fired_QDC_amount = 0;

    QLong = std::vector<double>(max_am_dets,0);
    QLong_Raw = std::vector<double>(max_am_dets,0);
    QShort_Raw = std::vector<double>(max_am_dets,0);

    QDC_Time_Coarse = std::vector<ULong64_t>(max_am_dets,0);
    QDC_Time_Fine = std::vector<double>(max_am_dets,0);

    TDC_Time_raw = std::vector<ULong64_t>(max_am_dets,0);
    TDC_Time_ns  = std::vector<double>(max_am_dets,0);

    det_ids_QDC = std::vector<int>(max_am_dets,0);
    det_ids_TDC = std::vector<int>(max_am_dets,0);


    det_ID_QDC = std::vector<std::vector<int> >(100,std::vector<int>(100,0));
    det_ID_TDC = std::vector<std::vector<int> >(100,std::vector<int>(100,0));

    FAT_positions = std::vector<std::vector<double> >(36,std::vector<double>(3,-1));
    source_position_correction = std::vector<double>(36,-1);


    FATIMA_T_CALIB = new FATIMA_Time_Calibration();
    FATIMA_E_CALIB = new FATIMA_Energy_Calibration();

    load_board_channel_file();
    read_config_variables("Configuration_Files/Detector_System_Setup_File.txt");
    he_iter = 0;
    
    load_pos_correction();

    Det_Hist = new TH1D*[32];
    char name[1000];
    for(int i = 0;i < 32;++i){
        sprintf(name,"Det_%d",i);
        Det_Hist[i] = new TH1D(name,name,1000,-40000,40000);
    }
    
}

//---------------------------------------------------------------

FATIMA_Detector_System::~FATIMA_Detector_System(){
	delete FATIMA_T_CALIB;
	delete FATIMA_E_CALIB;
}

//---------------------------------------------------------------

void FATIMA_Detector_System::write(){
    TFile* file = new TFile("FATROOT.root","RECREATE");
    for(int i = 0;i < 2;++i) file->Add(Det_Hist[i]);
    file->Write();

    cout << "written" << endl;
}

//---------------------------------------------------------------

void FATIMA_Detector_System::load_board_channel_file(){

    const char* format = "%d %d %d %d %d %d";

    ifstream file("Configuration_Files/FATIMA_allocation.txt");

    if(file.fail()){
        cerr << "Could not find QDC_FATIMA Board_Channel to DetNum File!" << endl;
        exit(0);
    }

    string line;
    int detector_number,board_id,channel_num,board_id_TDC,TDC_ch,enabled;

    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&detector_number,&board_id,
					&channel_num,&board_id_TDC,&TDC_ch,&enabled);
		if (enabled == 1) {
			det_ID_QDC[board_id][channel_num] = detector_number;
			det_ID_TDC[board_id_TDC][TDC_ch] = detector_number;
		}
    }
}

//---------------------------------------------------------------

void FATIMA_Detector_System::load_det_angles(){

    const char* format = "%d %lf %lf %lf";

    ifstream file("Configuration_Files/FATIMA_Detector_Positions.txt");

    if(file.fail()){
        cerr << "Could not find FATIMA Detector Positions File!" << endl;
        exit(0);
    }

    string line;
    int pos_num;
    double r, theta, phi;

    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format, &pos_num, &r, &theta, &phi);
		
		FAT_positions[pos_num][0] = r;
		FAT_positions[pos_num][1] = theta;
		FAT_positions[pos_num][2] = phi;
    }
}

//---------------------------------------------------------------

void FATIMA_Detector_System::load_pos_correction(){

    const char* format = "%lf %lf %lf";

    ifstream file("Configuration_Files/FATIMA_Source_Position.txt");

    if(file.fail()){
        cerr << "Could not find FATIMA Source Position File!" << endl;
        exit(0);
    }

    string line;
    int pos_num;
    double r, theta, phi;
    

    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format, &r, &theta, &phi);
	
	
    }
	
    for(int i = 0; i < 36; ++i){
	
	source_position_correction[i] = distance_between_detectors(r, theta, phi, FAT_positions[i][0], FAT_positions[i][1], FAT_positions[i][2]);

	double average_distance = distance_between_detectors(0.0, 0.0, 0.0, FAT_positions[i][0], FAT_positions[i][1], FAT_positions[i][2]);

	source_position_correction[i] = (average_distance - source_position_correction[i])/300.0;
	
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

    RAW->set_DATA_FATIMA(fired_QDC_amount,fired_TDC_amount,
						QLong_Raw,QShort_Raw,QLong,
						TDC_Time_raw, TDC_Time_ns, QDC_Time_Coarse,QDC_Time_Fine,
						det_ids_QDC,det_ids_TDC);
						
    //QDC_TDC->get_Detector_Data(RAW);
}

//---------------------------------------------------------------

void FATIMA_Detector_System::Process_MBS(int* pdata){


    this->pdata = pdata;
    FAT_evt++;
    //extract QDC and TDC informations from MBS and assign to detectors
    exiter = false;

    no_data = false;

    //check for QDC and TDC header
    QDC_Header* QDChead  = (QDC_Header*) this->pdata;
    TDC_Check* TDChead = (TDC_Check*) this->pdata;

    fired_QDC_amount = 0;
    fired_TDC_amount = 0;



    //reset TDC called bool
    bool TDC_Called = false;
    QDC_DATA = false;

    reset_fired_channels();
    
    num_TDC_modules = num_TDC_modules_fixed;
        
    //loop over FATIMA modules
    while(!TDC_Called){
        //QDC channel empty (check a -> always with QDC, length-> am_channels called (len-4))
        if(QDChead->check_a == 10 && QDChead->length == 4) {
            for(int i = 0;i < 3;++i) this->pdata++;
        }
        //QDC channel filled 
        else if(QDChead->check_a == 10){
        
        // double length = QDChead->length;
    
            QDC_DATA = true;
            Check_QDC_DATA(QDChead);
        }
        //TDC code reached
        else if(TDChead->type == 8){

            num_TDC_modules--;

            this->pdata--;

            Check_TDC_DATA(); 

            if (num_TDC_modules == 0){
                TDC_Called = true;
                this->pdata++;
            } 
        }
        else if(TDChead->no == 3145728){

            num_TDC_modules--;
         
            if (num_TDC_modules == 0){
                TDC_Called = true;
                this->pdata++;
            }
        }
        else{
            cout << "Unknown header word: " << TDChead->type << endl;
            unknown_header_counter++;
            if(unknown_header_counter > 10){
                cout << endl;
                cout << "ERROR: Too many unknown header words in FATIMA unpacker" << endl;
                cout << "Something is probably wrong with the setup configuration" << endl;
                cout << "Most likely White Rabbit is/isn't enabled" << endl;
                exit(0);
            }
        }

        this->pdata++;
        
        QDChead = (QDC_Header*) this->pdata;
        TDChead = (TDC_Check*) this->pdata;
    }

    this->pdata--;
    this->pdata--;

    //Check_TDC_DATA(); 
    //if(exiter) exit(0);
}

//---------------------------------------------------------------

void FATIMA_Detector_System::reset_fired_channels(){
    fired_QDC_amount = 0;
    fired_TDC_amount = 0;
    for(int i = 0;i < max_am_dets;++i){
        QLong[i] = -1;
        QDC_Time_Coarse[i] = 0;
        QDC_Time_Fine[i] = 0;
        TDC_Time_raw[i] = 0;
        TDC_Time_ns[i] = 0;
    }
}


//---------------------------------------------------------------

void FATIMA_Detector_System::Check_QDC_DATA(QDC_Header* QDChead){

    

    pdata++; // Moves to 2nd header value //

    QDC_Header_2 *QDChead_2 = (QDC_Header_2*) pdata;


    int board_ID = QDChead_2->geo; // Gets Geographical Address //
    int num_Channels = QDChead_2->channels; // Gets Channels fired 0011 means 1&2 fired //

    int num_channels_fired = 0;
    
    //cout<<"Number of Channels = "<<num_Channels<<endl;
        
     // Loop retrieves channels fired from integer value //
    for(int j = 7; j >= 0; j--){
        if(num_Channels >= pow(2, j)){
            //fill Fired_Channel array with corresponding ids
            if(wired_QDC(board_ID,j)){
                Fired_QDC_Channels[num_channels_fired][0] = board_ID; 
                Fired_QDC_Channels[num_channels_fired][1] = j;
                num_channels_fired++;
            }
            else{
                Fired_QDC_Channels[num_channels_fired][0] = board_ID; 
                Fired_QDC_Channels[num_channels_fired][1] = -1;
                num_channels_fired++;
            }
            num_Channels -= pow(2, j);
        }
    }
    
    //reverse(Fired_QDC_Channels.begin(), Fired_QDC_Channels.end());
    
    
    /*for(int i = 0; i < num_channels_fired; ++i){
	
	if(i < (num_channels_fired - i)){
	    
	    int save_value_0 = Fired_QDC_Channels[i][0];
	    int save_value_1 = Fired_QDC_Channels[i][1];
	    
	    Fired_QDC_Channels[i][0] = Fired_QDC_Channels[num_channels_fired-i][0];
	    Fired_QDC_Channels[i][1] = Fired_QDC_Channels[num_channels_fired-i][1];
	    
	    Fired_QDC_Channels[num_channels_fired-i][0] = save_value_0;
	    Fired_QDC_Channels[num_channels_fired-i][1] = save_value_1;
	}
     
    }*/
        
    //fired_QDC_amount += num_channels_fired;
    
    pdata += 2; // Moves from 2nd to 4th header value //
    
    //int size = 0;
    int active_Channel = 0;
    int active_board = 0;
    int active_det = 0;
    double fine_time = 0;
    

    for(int i = (num_channels_fired - 1); i >= 0; --i){
	
		pdata++; // Moves to 1st data value

		QDC_Format_Size* fs = (QDC_Format_Size*) pdata;
	
	
		if (Fired_QDC_Channels[i][1] == -1){ /*pdata += 6;*/

	    
			int skip = fs->size - 1;
	    	    
			pdata += skip;
	      
		}else{
            if(fired_QDC_amount > max_am_dets){
                cerr << "fired_QDC_amount too large! " << fired_QDC_amount << endl;
            }
			//set active board_ID and channel #
			active_board = Fired_QDC_Channels[i][0];
			active_Channel = Fired_QDC_Channels[i][1];
    
			active_det = det_ID_QDC[active_board][active_Channel];
				    
			//cout<<"Channel Number = "<<active_Channel<<" Board ID = "<<active_board<<endl;
			//cout<<"Channel Number = "<<active_det<<endl;
	    
			det_ids_QDC[fired_QDC_amount] = active_det;
		
			//cout<<active_det<<" ";
		
				    	    	    
			pdata += 3; //Moves to QDC time data
	    
			QDC_Time* t = (QDC_Time*) pdata;
			QDC_Time_Coarse[fired_QDC_amount] = t->trigger_tag;
			if(dist_corr_used) QDC_Time_Coarse[fired_QDC_amount] += source_position_correction[active_det];
	    
			pdata++; // Moves to Extras
	   
			QDC_Extras* e = (QDC_Extras*) pdata;	
    
			ULong64_t tmp_ext_trig = e->ext_trig;
    
			fine_time = (((t->trigger_tag) + (tmp_ext_trig << 32)));
			//possibly this should be -= have to verify...
			fine_time += ((double)(e->fine_time)/1024.);
			
			if(dist_corr_used) fine_time += source_position_correction[active_det];

			
	    
			QDC_Time_Fine[fired_QDC_amount] = fine_time;
	    
			pdata++; // Moves to 6th data value87454dda
	    
			QDC_Data* d = (QDC_Data*) pdata;
	    
			QLong_Raw[fired_QDC_amount] = d->QL; // Gets Q Long data //
			QShort_Raw[fired_QDC_amount] = d->QS; // Gets Q Short data //
			//printf("Det%02d  E (chn) %8.2lf ", det_ids_QDC[fired_QDC_amount],
			//									QLong_Raw[fired_QDC_amount]);
			if (gain_match_used) {
				Gain_Match_QDC(fired_QDC_amount);
			//	printf("E (gm) %8.2lf ", QLong[fired_QDC_amount]);
			}
			Calibrate_QDC(fired_QDC_amount);
			//printf("E (gm) %8.2lf\n", QLong[fired_QDC_amount]);
			fired_QDC_amount++;
		}
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
    
    // unused // int TDC_loop = 0;
    
    int loop_counter = 0;
    while(!trail){
        loop_counter++;
        pdata++;
	        
        TDC_Check* p = (TDC_Check*) pdata;
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
            if(!wired_TDC(tdc_board_ID,TDC_ch)){
                //cout << "TDC " << tdc_board_ID << " ch " << TDC_ch << " not wired " << fired_QDC_amount << endl;
                no_data = true;
            }
            else{
                if(fired_QDC_amount > max_am_dets){
                    cerr << "fired_TDC_amount too large! " << fired_TDC_amount << endl;
                }
                active_det = det_ID_TDC[tdc_board_ID][TDC_ch];
                det_ids_TDC[fired_TDC_amount] = active_det;
                //cout << "DET!" << active_det << endl;
                //if(active_det >= 50){
                //    if(fired_QDC_amount == 1 && active_det == 51){
                //        double tmp_val = ((double) m->measurement*25.) - ((double) TDC_Time[det_ids_QDC[0]]);
				//
                //        Det_Hist[det_ids_QDC[0]]->Fill(tmp_val*1e-3);
				//
                //    }
                //    he_iter++;
                //}
                TDC_Time_raw[fired_TDC_amount] = (ULong64_t) (m->measurement);
		
                if (dist_corr_used) TDC_Time_raw[fired_TDC_amount] += source_position_correction[active_det];

                // TDC_Time_raw[fired_TDC_amount] = 25e-3*TDC_Time_raw[fired_TDC_amount];
                Calibrate_TDC(fired_TDC_amount);

                if(dist_corr_used) TDC_Time_ns[fired_TDC_amount] += source_position_correction[active_det];

                fired_TDC_amount++;
                no_data = false;
            }
        }
        // TDC Trailer Condition // 
        else if ( check == 16 ) trail = true;

        if(loop_counter > 100){
            cerr << "FATIMA TDC loop not reaching trailer! pdata iteration problem possible" << endl;
            cerr << "Exiting Program!" << endl;
            exit(0);
        }
    }
}

//---------------------------------------------------------------

bool FATIMA_Detector_System::check_additional(int active){return (active == 51);}

//---------------------------------------------------------------

bool FATIMA_Detector_System::wired_QDC(int board_id,int ch_num){
    return det_ID_QDC[board_id][ch_num] != -1;
}

//---------------------------------------------------------------

bool FATIMA_Detector_System::wired_TDC(int board_id,int ch_num){
    return det_ID_TDC[board_id][ch_num] != -1;
}

//---------------------------------------------------------------

//det_ids_TDC/det_ids_QDC has to
//be set correctly before calling these cal / gm functions
void FATIMA_Detector_System::Calibrate_QDC(int id){
	if (gain_match_used){
	    QLong[id] = FATIMA_E_CALIB->Calibrate(QLong[id], det_ids_QDC[id]);
	}
	else{
	    QLong[id] = FATIMA_E_CALIB->Calibrate(QLong_Raw[id], det_ids_QDC[id]);
	}
	QDC_Time_Coarse[id] = FATIMA_T_CALIB->Calibrate_QDC(QDC_Time_Coarse[id], det_ids_TDC[id]);
	QDC_Time_Fine[id] = FATIMA_T_CALIB->Calibrate_QDC(QDC_Time_Fine[id], det_ids_TDC[id]);

}

//---------------------------------------------------------------

void FATIMA_Detector_System::Gain_Match_QDC(int id){
	QLong[id] = FATIMA_GAIN_MATCH->Gain_Match(QLong_Raw[id],det_ids_QDC[id]);
}

//---------------------------------------------------------------
void FATIMA_Detector_System::Calibrate_TDC(int id){
	TDC_Time_ns[id] = FATIMA_T_CALIB->Calibrate_TDC(TDC_Time_raw[id],det_ids_TDC[id]);
}

//---------------------------------------------------------------

void FATIMA_Detector_System::set_Gain_Match_Filename(string GM_filename){
	
	FATIMA_GAIN_MATCH = new FATIMA_Gain_Match(GM_filename);
	
	gain_match_used = true;
	
}

//---------------------------------------------------------------

int* FATIMA_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

void FATIMA_Detector_System::read_config_variables(string config_filename){
    
    ifstream file(config_filename);

    if(file.fail()){
        cerr << "Could not find File for setup parameters!" << endl;
        exit(0);
    }

    string line;
    
    file.ignore(256,':');
    file.ignore(256,':');
    file >> gain_match_used;//dummy_var;

    for (int i = 0; i < 4; ++i) file.ignore(256,':');

    file.ignore(256,':');
    file >> dist_corr_used;//dummy_var;   
    
    file.ignore(256,':');
    file >> num_TDC_modules_fixed;//dummy_var;   
        
};

//---------------------------------------------------------------

double FATIMA_Detector_System::distance_between_detectors(double _r, double _theta, double _phi, double r_, double theta_, double phi_){

    _theta = _theta * M_PI/180.0; 
    theta_ = theta_ * M_PI/180.0; 

    _phi = _phi * M_PI/180.0; 
    phi_ = phi_ * M_PI/180.0; 

    double dist = sqrt(_r*_r + r_*r_ - 2.0*_r*r_*(sin(_theta)*sin(theta_)*cos(_phi - phi_) + cos(_theta)*cos(theta_)));
    
    return dist;


}
//---------------------------------------------------------------

