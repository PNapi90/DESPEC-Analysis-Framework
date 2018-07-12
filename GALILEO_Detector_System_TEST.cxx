#include "GALILEO_Detector_System_TEST.h"

#include <cstdlib>
#include <map>

#include "FEBEX.h"



using namespace std;

//---------------------------------------------------------------

GALILEO_Detector_System::GALILEO_Detector_System(){

    //set amount of Germanium Detectors

    max_am_dets = 36;

    fired_FEBEX_amount = 0;

    Sum_Time = new ULong64_t[max_am_dets];
    Pileup = new int[max_am_dets];
    Hit_Pattern = new int[max_am_dets];

    Chan_Time = new ULong64_t[max_am_dets];
    Chan_Energy = new double[max_am_dets];

    det_ids = new int[max_am_dets];

    GALILEO_T_CALIB = new GALILEO_Time_Calibration();
    GALILEO_E_CALIB = new GALILEO_Energy_Calibration();

    load_board_channel_file();

    
}

//---------------------------------------------------------------

GALILEO_Detector_System::~GALILEO_Detector_System(){

	GALILEO_map.clear();

	delete[] det_ids;

	delete[] Sum_Time;
	delete[] Pileup;
	delete[] Hit_Pattern;
	
	delete[] Chan_Time;
	delete[] Chan_Energy;

	delete GALILEO_T_CALIB;
	delete GALILEO_E_CALIB;
}

//---------------------------------------------------------------

void GALILEO_Detector_System::load_board_channel_file(){

    const char* format = "%d %d %d %d";

    ifstream file("Configuration_Files/FEBEX_allocation.txt");

    if(file.fail()){
        cerr << "Could not find FEBEX GALILEO Board_Channel to DetNum Allocation File!" << endl;
        exit(0);
    }

    string line;
    int detector_number,board_id,channel_num,dummy;

    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&detector_number,&board_id,&channel_num,&dummy);
	
	GALILEO_map[std::make_pair(board_id,channel_num)] = detector_number;
	
    }
}

//---------------------------------------------------------------

void GALILEO_Detector_System::get_Event_data(Raw_Event* RAW){
	
	RAW->set_DATA_GALILEO(fired_FEBEX_amount,Sum_Time,Pileup,Hit_Pattern,Chan_Time,Chan_Energy,det_ids);
}

//---------------------------------------------------------------

void GALILEO_Detector_System::Process_MBS(int* pdata){

    reset_fired_channels();
    
    int current_det = 0;
    
    this->pdata = pdata;
    
    bool FEBEX_data_loop = true;
    
    int num_modules = 2; // Number of modules in the data //
    
    fired_FEBEX_amount = 0;
    
    FEBEX_Add* FEBEX_add  = (FEBEX_Add*) this->pdata;

    while (FEBEX_add->add == 2781){
				
	this->pdata++;
		
	FEBEX_add = (FEBEX_Add*) this->pdata;

    }

    FEBEX_Header* FEBEXhead  = (FEBEX_Header*) this->pdata;
    
    
    while(FEBEX_data_loop){
	

		
    
        if (FEBEXhead->ff == 255){ // FEBEX module idicator //
	    
	
            // FEBEXhead->ff;
            // FEBEXhead->chan_head;
            // FEBEXhead->three_four;
    
            board_id = FEBEXhead->chan_head;
			
            this->pdata++; // Moves to Channel Size //
			
            FEBEX_Chan_Size *fbx_size=(FEBEX_Chan_Size*) this->pdata;    
			
            num_channels = ((fbx_size->chan_size)/4) - 1;
	    	    
	    if (num_channels == 0) --num_modules;
		
            this->pdata++; // Moves to Event Timestamp //

            FEBEX_Half_Time *fbx_hT=(FEBEX_Half_Time*) this->pdata;
		
            this->pdata++; // Moves to rest of Event Timestamp //
			
            FEBEX_Evt_Time *fbx_time=(FEBEX_Evt_Time*) this->pdata;
	    
	    ULong64_t tmp_ext_time = ((fbx_hT->ext_time));
			
	    tmp_Sum_Time = (fbx_time->evt_time)+ (tmp_ext_time<<32);//((fbx_hT->ext_time)<<32);
			
			
            this->pdata++; // Moves to Pileup & Hit Pattern //
						
            FEBEX_Flag_Hits *fbx_flag=(FEBEX_Flag_Hits*) this->pdata;
	    
	    tmp_Pileup = fbx_flag->pile_flags;
	    
	    tmp_Hit_Pattern = fbx_flag->hit_pattern; 
    
            this->pdata++; // Moves to DEADBEEF //
    
        }
	else if (FEBEXhead->ff == 240){ // FEBEX channel idicator //
	    
	    this->pdata--; // Moves back to DEADBEEF so channel loop functions properly //

	    
	    for(int i=0; i<num_channels; ++i){
				
		this->pdata++; // Moves to channel header //

		FEBEX_Chan_Header *fbx_Ch=(FEBEX_Chan_Header*) this->pdata;

		current_det = GALILEO_map[std::make_pair(board_id, (fbx_Ch->Ch_ID))];
						
		det_ids[i] = current_det;
		
		
		Sum_Time[current_det] = tmp_Sum_Time;
		
		Pileup[current_det] = tmp_Pileup;
		 
		Hit_Pattern[current_det] = tmp_Hit_Pattern;
		
	    
		this->pdata++; // Moves to rest of channel timestamp //
    
		FEBEX_TS *fbx_Ch_TS=(FEBEX_TS*) this->pdata; 
		
		ULong64_t tmp_ext_chan_ts = (fbx_Ch->ext_chan_ts);
		
		Chan_Time[current_det] = (fbx_Ch_TS->chan_ts)+(tmp_ext_chan_ts<<32); 

		this->pdata++; // Moves to Channel Energy //
			    
		FEBEX_En *fbx_Ch_En=(FEBEX_En*) this->pdata; 
	    
		Chan_Energy[current_det] = fbx_Ch_En->chan_en;
				
		Calibrate_FEBEX(current_det);
					    
		this->pdata++; // Moves to Future Use //
		
		++fired_FEBEX_amount;
		
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
                // @@@@ Traces Would Go Here @@@@@ //
                // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		
			    	    
	    }
	    
	    --num_modules;
	    
	}
	
	if (num_modules != 0){
	    
	    this->pdata++;
	    
	    FEBEXhead  = (FEBEX_Header*) this->pdata;
	    
	}
	else FEBEX_data_loop = false; // Exits FEBEX Loop //
	
    }
    
}

//---------------------------------------------------------------

void GALILEO_Detector_System::reset_fired_channels(){
    
    fired_FEBEX_amount = 0;
    
    for(int i = 0;i < max_am_dets;++i){
        Sum_Time[i] = -1;
        Pileup[i] = -1;
        Hit_Pattern[i] = 0;
        Chan_Time[i] = 0;
        Chan_Energy[i] = 0;
    }
}


//---------------------------------------------------------------

bool GALILEO_Detector_System::wired_FEBEX(int board_id,int ch_num){
    return GALILEO_map[std::make_pair(board_id, ch_num)] != -1;
}

//---------------------------------------------------------------

void GALILEO_Detector_System::Calibrate_FEBEX(int id){

	Sum_Time[id] = GALILEO_T_CALIB->Calibrate_FEBEX_Sum_T(Sum_Time[id],id);

	Chan_Time[id] = GALILEO_T_CALIB->Calibrate_FEBEX_Chan_T(Chan_Time[id],id);
	Chan_Energy[id] = GALILEO_E_CALIB->Calibrate_FEBEX_E(Chan_Energy[id],id);

}

//---------------------------------------------------------------

int* GALILEO_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------
