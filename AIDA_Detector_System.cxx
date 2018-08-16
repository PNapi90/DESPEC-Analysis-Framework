#include "AIDA_Detector_System.h"

#include <cstdlib>

using namespace std;

//---------------------------------------------------------------

AIDA_Detector_System::AIDA_Detector_System(){
        
    check_FEE64_timestamp = new Bool_t[128];
    
    for(int i = 0; i < 128; i++) check_FEE64_timestamp[i] = true;
    
    feeChannelOrder = new int[64];
    for(int i = 0; i < 64; i++) check_FEE64_timestamp[i] = -1;

    FEE_allocation = new int*[24];

    for(int i = 0;i < 24;++i){
	    FEE_allocation[i] = new int[3];
	    for(int j = 0;j < 3;++j){
		FEE_allocation[i][j] = -1;
	}
    
    }
    
    /*unsigned int feeChannelOrder[64] = {62, 63, 59, 60, 61, 56, 57, 58, 52, 53, 54, 55, 49, 50, 51, 45,
			  46, 47, 48, 42, 43, 44, 38, 39, 40, 41, 35, 36, 37, 31, 32, 33,
			  34, 28, 29, 30, 24, 25, 26, 27, 21, 22, 23, 17, 18, 19, 20, 14,
			  15, 16, 10, 11, 12,  7,  3,  0,  8,  4,  1,  9,  5,  2, 13,  6};*/
			  			  
    load_config_file();
    load_channel_order();

}

//---------------------------------------------------------------

AIDA_Detector_System::~AIDA_Detector_System(){
    
    delete[] check_FEE64_timestamp;
    delete[] FEE_allocation;

}


//---------------------------------------------------------------

void AIDA_Detector_System::Process_AIDA(TGo4MbsSubEvent* psubevt){
    
    
    pdata=psubevt->GetDataField();
    
    
    pdata_start = pdata;
            
    sub_evt_length  = ((psubevt->GetDlen() - 2) / 2);
    
    //cout<<"Length = "<<sub_evt_length<<endl;
    
    pdata+=5;
    
    //cout<<dec<<"pdata_start = "<<pdata_start<<endl;
    //cout<<dec<<"pdata = "<<pdata<<endl;
    
    //while(!End_of_AIDA){

    //cout<<"START OF AIDA DATA REACHED !!!"<<endl;

    while((pdata - pdata_start) < (sub_evt_length)){
	
	//cout<<"sub_evt_length  "<<sub_evt_length<<endl;

	//cout<<"Number into the cycle  "<<(pdata - pdata_start)<<endl;
    
	AIDA_Time_First* t0_head  = (AIDA_Time_First*) this->pdata;
	AIDA_ADC_1* ADC_head  = (AIDA_ADC_1*) this->pdata;
	AIDA_1st_Disc* AIDA_disc  = (AIDA_1st_Disc*) this->pdata;
	
    	if(t0_head->check == 2 && t0_head->infocode == 2) Pause_Timestamp(t0_head); // Pause Timestamp
    	else if(t0_head->check == 2 && t0_head->infocode == 3) Resume_Timestamp(t0_head); // Resume Timestamp
    	else if(t0_head->check == 2 && t0_head->infocode == 4){  // Shouldn't Happen

	    cout<<"Error: AIDA WR Timestamp 2nd Part Should not happen here!"<<endl;
	    cout<<"AIDA WR Timestamp was split across Subevents!"<<endl;

	    this->pdata++;

	    //exit(0);
	     
	}
	else if(t0_head->check == 2 && t0_head->infocode == 5) Set_AIDA_Timestamp(t0_head); // White Rabbit Timestamp marker
	//else if(t0_head->check == 2 && t0_head->infocode == 6) Set_AIDA_Timestamp(t0_head); // AIDA Disc Data
	else if(t0_head->check == 2 && t0_head->infocode == 8){
	    
	    cout<<"Error: AIDA Correlation Scaler, not unpackable yet"<<endl; // AIDA Correlation Scaler
	    exit(0);
	
	}
	else if(ADC_head->check == 3 && ADC_head->ADC_range == 1){
	    
	    Set_AIDA_Implantation(ADC_head);
	
	    this->pdata++;
	}
	else if(ADC_head->check == 3 && ADC_head->ADC_range == 0){
	    
	    Unpack_AIDA_Decay_DATA(ADC_head); // Set_AIDA_DECAY/NOISE(ADC_head);
	
	    this->pdata++;

	}
	else if(AIDA_disc->check == 8 && AIDA_disc->second_check == 6) this->pdata++; //Check_AIDA_Disc_DATA();

	else{ cout<<"Unidentified Thing:   AIDA Check = "<<t0_head->check<<"  INFO CODE = "<<t0_head->infocode<<"  FEE64???  "<<t0_head->FEE64_num<<endl;
	    cout << hex << *(this->pdata) <<endl;
	}
	
	this->pdata++;

    }
    
    
    //cout<<"END OF AIDA DATA REACHED !!!"<<endl;
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Pause_Timestamp(AIDA_Time_First* t0_head){
    
    int FEE64_Module_No = t0_head->FEE64_num;
    
    check_FEE64_timestamp[FEE64_Module_No] = false;
    
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Resume_Timestamp(AIDA_Time_First* t0_head){
    
    int FEE64_Module_No = t0_head->FEE64_num;
    
    check_FEE64_timestamp[FEE64_Module_No] = true;
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Set_AIDA_Timestamp(AIDA_Time_First* t0_head){
    
    //cout<<"AIDA Timestamp!!!"<<endl;
    
    tmp_AIDA_t0_0_15 = t0_head->time_part;

    pdata++;
    
    AIDA_Time_Last* t0_head_2  = (AIDA_Time_Last*) pdata;

    tmp_AIDA_t0_36_63 = t0_head_2->time_part;
    
    pdata++;
    
    AIDA_Time_Mid* t0_head_3  = (AIDA_Time_Mid*) pdata;
    
    tmp_AIDA_t0_16_35 = t0_head_3->time_part;
    
    
    AIDA_t0 	 =  tmp_AIDA_t0_36_63 + (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_0_15 << 48);
    AIDA_t0_base =  (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_0_15 << 48);
    
    pdata++;
    
    //cout<<"AIDA Timestamp = "<<AIDA_t0<<endl;
    //cout<<"AIDA Timestamp Base = "<<AIDA_t0_base<<endl;


}

//---------------------------------------------------------------

void AIDA_Detector_System::Set_AIDA_Implantation(AIDA_ADC_1* ADC_head){
    
    cout<<"Implantation event!!"<<endl;
    
    pdata++;
    
}


void AIDA_Detector_System::Unpack_AIDA_Decay_DATA(AIDA_ADC_1* ADC_head){
    
    //cout<<"AIDA Decay!!!"<<endl;
    
    //AIDA_ADC_2* ADC_head_2  = (AIDA_ADC_2*) pdata;
    
    decayItem.Set_Decay_Data(pdata, AIDA_t0_base);
      
    get_decay_coordinate();
    
    //decayItem.Print_Event();

}

//---------------------------------------------------------------

void AIDA_Detector_System::Check_AIDA_Disc_DATA(){
    
    //cout<<"AIDA Disc!!!"<<endl;

    pdata++;
    
    AIDA_1st_Disc_2* AIDA_disc_2 = (AIDA_1st_Disc_2*) pdata;

    tmp_AIDA_Discriminator_end = AIDA_disc_2->first_disc_time;
    
    AIDA_Discriminator_full = AIDA_t0_base + tmp_AIDA_Discriminator_end;
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::get_Event_data(Raw_Event* RAW){}

//---------------------------------------------------------------

void AIDA_Detector_System::load_config_file(){

    const char* format = "%d %d %d %d";

    ifstream file("Configuration_Files/AIDA_strip_map.txt");

    if(file.fail()){
        cerr << "Could not find AIDA Config File!" << endl;
        exit(0);
    }

    string line;
    int FEE_num,	DSSSD_num,	Front_Back,	Left_Right;
    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&FEE_num,&DSSSD_num,
					&Front_Back,&Left_Right);
	FEE_allocation[FEE_num-1][0] = DSSSD_num;
	FEE_allocation[FEE_num-1][1] = Front_Back;
	FEE_allocation[FEE_num-1][2] = Left_Right;
	
    }
}


//---------------------------------------------------------------

void AIDA_Detector_System::load_channel_order(){

    const char* format = "%d";

    ifstream file("Configuration_Files/AIDA_Channel_Order.txt");

    if(file.fail()){
        cerr << "Could not find AIDA Channel Order File!" << endl;
        exit(0);
    }
    int line_number = 0;

    string line;
    int ChannelID;
    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&ChannelID);
	feeChannelOrder[line_number] = ChannelID;
	line_number++;
	
    }
}


//---------------------------------------------------------------

void AIDA_Detector_System::get_decay_coordinate(){
    
    int FEE_ID = decayItem.GetFEE64ID();
    int Channel_ID = decayItem.GetChannelID();

    if(FEE_allocation[FEE_ID - 1][1] == 0){
	
	//x_number++; // Set X to -1
    
	if(FEE_allocation[FEE_ID - 1][2] == 1){
	    
	    tmp_x = feeChannelOrder[Channel_ID];
	    
	}    
	if(FEE_allocation[FEE_ID - 1][2] == 2){
	    
	    tmp_x = (127 - feeChannelOrder[Channel_ID]);

	}
	
	decayItem.Set_X(tmp_x);
	
    }
    if(FEE_allocation[FEE_ID - 1][1] == 1){
	
	//y_check = true; // Set Y to -1
    
	if(FEE_allocation[FEE_ID - 1][2] == 1){
	    
	    tmp_y = feeChannelOrder[Channel_ID];

	    
	}    
	if(FEE_allocation[FEE_ID - 1][2] == 2){
	    
	    tmp_y = (127 - feeChannelOrder[Channel_ID]);
		    
	}
	
	decayItem.Set_Y(tmp_y);
	
    }
    
    tmp_z = FEE_allocation[FEE_ID - 1][0];
    
    decayItem.Set_Layer(tmp_z);

}

//---------------------------------------------------------------

int* AIDA_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

