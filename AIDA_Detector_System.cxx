#include "AIDA_Detector_System.h"
#include "AIDA_Data_Types.h"

#include <cstdlib>

using namespace std;

//---------------------------------------------------------------

AIDA_Detector_System::AIDA_Detector_System(){

    Processor = new AIDA_Processor(3);
    AIDA_Store = new AIDA_Decay_Event_Store();
        
    check_FEE64_timestamp = new Bool_t[128];
    
    for(int i = 0; i < 128; i++) check_FEE64_timestamp[i] = true;
    
    feeChannelOrder = new int[num_channels];
    for(int i = 0; i < num_channels; i++) check_FEE64_timestamp[i] = -1;

    FEE_allocation = new int*[num_FEE64s];
    FEE_polarity_map = new int[num_FEE64s];
    channel_offsets_map = new double*[num_channels];

    for(int i = 0; i < num_FEE64s; ++i){
	    FEE_allocation[i] = new int[3];
	    FEE_polarity_map[i] = 0;
	    channel_offsets_map[i] = new double[num_channels];
	    
	    for(int j = 0; j < 3; ++j)			FEE_allocation[i][j] = -1;
	    for(int k = 0; k < num_channels; ++k)	channel_offsets_map[i][k] = -1;    
    }
    
    ADCLowEnergyGain = new double*[num_FEE64s];
    ADCHighEnergyGain = new double*[num_FEE64s];

    for(int i = 0; i < num_FEE64s; ++i){
	    ADCLowEnergyGain[i]  = new double[num_channels];	//keV/ch
	    ADCHighEnergyGain[i] = new double[num_channels];	//MeV/ch
	    
	    for(int j = 0; j < num_channels; ++j){

		ADCLowEnergyGain[i][j]  = 0.7;	//keV/ch
		ADCHighEnergyGain[i][j] = 0.7; 	//MeV/ch
		
	    }

	    
	    
	    
    }
    
    ADCItemCounts = new int*[24];
    ADCLastTimestamp = new ULong64_t*[24];
    
    //initialise the ADC item counter to 0
    for(int i = 0; i < 24; i++){
	
	ADCItemCounts[i] = new int[4];
	ADCLastTimestamp[i] = new ULong64_t[4];
	
	for(int j = 0; j < 4; j++){
		ADCItemCounts[i][j] = 0;
		ADCLastTimestamp[i][j] = 0;
	}
    }

    
    /*unsigned int feeChannelOrder[num_channels] = {62, 63, 59, 60, 61, 56, 57, 58, 52, 53, 54, 55, 49, 50, 51, 45,
			  46, 47, 48, 42, 43, 44, 38, 39, 40, 41, 35, 36, 37, 31, 32, 33,
			  34, 28, 29, 30, 24, 25, 26, 27, 21, 22, 23, 17, 18, 19, 20, 14,
			  15, 16, 10, 11, 12,  7,  3,  0,  8,  4,  1,  9,  5,  2, 13,  6};*/
			  			  
    load_config_file();
    load_channel_order();
    load_polarity_file();
    load_offsets_file();
}

//---------------------------------------------------------------

AIDA_Detector_System::~AIDA_Detector_System(){
    
    delete[] check_FEE64_timestamp;
    delete[] FEE_allocation;

    delete Processor;
    delete AIDA_Store;

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
    
	AIDA_Time_First* t0_head  = (AIDA_Time_First*) pdata;
	AIDA_ADC_1* ADC_head  = (AIDA_ADC_1*) pdata;
	AIDA_1st_Disc* AIDA_disc  = (AIDA_1st_Disc*) pdata;
	
	tmp_FEE64ID   = (*pdata >> 22) & 0x003F;  			//tmp_FEE64

	
    	if(t0_head->check == 2 && t0_head->infocode == 2) Pause_Timestamp(t0_head); // Pause Timestamp
    	else if(t0_head->check == 2 && t0_head->infocode == 3) Resume_Timestamp(t0_head); // Resume Timestamp
    	else if(t0_head->check == 2 && t0_head->infocode == 4){  // Shouldn't Happen

	    cout<<"Error: AIDA WR Timestamp 2nd Part Should not happen here!"<<endl;
	    cout<<"AIDA WR Timestamp was split across Subevents!"<<endl;

	    pdata++;

	    //exit(0);
	     
	}
	else if(t0_head->check == 2 && t0_head->infocode == 5) Set_AIDA_Timestamp(); // White Rabbit Timestamp marker
	//else if(t0_head->check == 2 && t0_head->infocode == 6) Set_AIDA_Timestamp(t0_head); // AIDA Disc Data
	else if(t0_head->check == 2 && t0_head->infocode == 8){
	    
	    cout<<"Error: AIDA Correlation Scaler, not unpackable yet"<<endl; // AIDA Correlation Scaler
	    exit(0);
	
	}
	else if(ADC_head->check == 3 && ADC_head->ADC_range == 1 && check_FEE64_timestamp[tmp_FEE64ID]) Set_AIDA_Implantation(ADC_head);
	else if(ADC_head->check == 3 && ADC_head->ADC_range == 0 && check_FEE64_timestamp[tmp_FEE64ID]) Unpack_AIDA_Decay_DATA(ADC_head); // Set_AIDA_DECAY/NOISE(ADC_head);
	else if(AIDA_disc->check == 8 && AIDA_disc->second_check == 6) pdata++; //Check_AIDA_Disc_DATA();

	else{ cout<<"Unidentified Thing:   AIDA Check = "<<t0_head->check<<"  INFO CODE = "<<t0_head->infocode<<"  FEE64???  "<<t0_head->FEE64_num<<endl;
	    cout << hex << *(pdata) <<endl;
	}
	
	pdata++;

    }
    
    
    //cout<<"END OF AIDA DATA REACHED !!!"<<endl;
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Pause_Timestamp(AIDA_Time_First* t0_head){
    
    //int FEE64_Module_No = t0_head->FEE64_num;
    
    check_FEE64_timestamp[tmp_FEE64ID] = false;
    
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Resume_Timestamp(AIDA_Time_First* t0_head){
    
    //int FEE64_Module_No = t0_head->FEE64_num;
    
    check_FEE64_timestamp[tmp_FEE64ID] = true;
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Set_AIDA_Timestamp(){
    
    //cout<<"AIDA Timestamp!!!"<<endl;
    
    tmp_AIDA_t0_0_15  = *pdata   & 0x0000FFFF;  		    
    tmp_AIDA_t0_36_63 = *pdata++ & 0x0FFFFFFF;  		    
    tmp_AIDA_t0_16_35 = *pdata++ & 0x000FFFFF;  		    
    
    AIDA_t0 	 =  tmp_AIDA_t0_36_63 + (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_0_15 << 48);
    AIDA_t0_base =  (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_0_15 << 48);
    
    pdata++;
    
    /*tmp_AIDA_t0_0_15 = t0_head->time_part;

    pdata++;
    
    AIDA_Time_Last* t0_head_2  = (AIDA_Time_Last*) pdata;

    tmp_AIDA_t0_36_63 = t0_head_2->time_part;
    
    pdata++;
    
    AIDA_Time_Mid* t0_head_3  = (AIDA_Time_Mid*) pdata;
    
    tmp_AIDA_t0_16_35 = t0_head_3->time_part;
    
    
    AIDA_t0 	 =  tmp_AIDA_t0_36_63 + (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_0_15 << 48);
    AIDA_t0_base =  (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_0_15 << 48);
    
    pdata++;*/
    
    cout<<"AIDA Timestamp = "<<AIDA_t0<<endl;
    cout<<"AIDA Timestamp Base = "<<AIDA_t0_base<<endl;


}

//---------------------------------------------------------------

void AIDA_Detector_System::Set_AIDA_Implantation(AIDA_ADC_1* ADC_head){
    
    cout<<"Implantation event!!"<<endl;
    
    implantItem.Set_Implant_Data(pdata, AIDA_t0_base);

    get_position_data(implantItem);
    
    implantItem.SetCalEnergy(((((double)implantItem.GetADCData()-adcZero)*(double)FEE_polarity_map[implantItem.GetFEE64ID()-1])
				    *ADCHighEnergyGain[implantItem.GetFEE64ID()-1][implantItem.GetChannelID()]));
    
    CorrectMultiplexer(implantItem);

    pdata++;
    
}


void AIDA_Detector_System::Unpack_AIDA_Decay_DATA(AIDA_ADC_1* ADC_head){
    
    //cout<<"AIDA Decay!!!"<<endl;
    
    //AIDA_ADC_2* ADC_head_2  = (AIDA_ADC_2*) pdata;
    
    decayItem.Set_Decay_Data(pdata, AIDA_t0_base);
      
    get_position_data(decayItem);
        
    decayItem.SetCalEnergy(((((double)decayItem.GetADCData()-adcZero)*(double)FEE_polarity_map[decayItem.GetFEE64ID()-1])
			- channel_offsets_map[decayItem.GetFEE64ID()-1][decayItem.GetChannelID()])*ADCLowEnergyGain[decayItem.GetFEE64ID()-1][decayItem.GetChannelID()]);
    
    CorrectMultiplexer(decayItem);
    
    pdata++;

    //Store "Decay" Event in AIDA Decay Event Store
    //AIDA_Store->Store_Decay_Event(&decayItem); 
    
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

void AIDA_Detector_System::CorrectMultiplexer(ADCDataItem & adcItem){

	//Check the FEE and channel of the DSSD and determine how many events from that ADC have been recoreded.
	//Correct the timestamp for the multiplexing
	itemADC       = ((adcItem.GetChannelID()) / 16);
	itemFEE	      = adcItem.GetFEE64ID()-1;
	itemTimestamp = adcItem.GetTimestamp();
	
	if((itemTimestamp-ADCLastTimestamp[itemFEE][itemADC] > 250 ) && ADCLastTimestamp[itemFEE][itemADC] != 0){
		//If two seperate ADC events within same window reset the adc counter
		ADCItemCounts[itemFEE][itemADC] = 0;
	}
	
	ADCLastTimestamp[itemFEE][itemADC] = itemTimestamp;
	//normalItems++;
	//timestampCorrection = (200 * adcItemCounts[itemFEE][itemADC]);

	adcItem.SetTimestamp((itemTimestamp-(200 * ADCItemCounts[itemFEE][itemADC])));
 	ADCItemCounts[itemFEE][itemADC]++;
	
 	return;

}


void AIDA_Detector_System::get_Event_data(Raw_Event* RAW){
    //Run AIDA Decay Event Builder
    //Processor->PROCESSING(AIDA_Store);

    //get Data using RAW for Implantation Evts
    //...

    //get Data using RAW for Decay Events
    //Processor->get_DATA(RAW);

    //Reset Store
    //AIDA_Store->Reset();
}

//---------------------------------------------------------------

void AIDA_Detector_System::load_polarity_file(){

    const char* format = "%d %d";

    ifstream file("Configuration_Files/AIDA_polarity.txt");

    if(file.fail()){
        cerr << "Could not find AIDA Polarity File!" << endl;
        exit(0);
    }

    string line;
    int FEE_num;
    int Polarity;
    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format, &FEE_num, &Polarity);
		
	FEE_polarity_map[FEE_num-1] = Polarity;
	
    }
}


//---------------------------------------------------------------

void AIDA_Detector_System::load_offsets_file(){

    const char* format = "%d %d %lf";

    ifstream file("Configuration_Files/AIDA_offsets.txt");

    if(file.fail()){
        cerr << "Could not find AIDA Offsets File!" << endl;
        exit(0);
    }

    string line;
    int FEE_num,	Channel_num;
    double  Offset;
    while(file.good()){
        getline(file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format, &FEE_num, &Channel_num, &Offset);
	channel_offsets_map[FEE_num-1][Channel_num] = Offset;
	
    }
}


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

void AIDA_Detector_System::get_position_data(ADCDataItem & adcItem){
    
    int FEE_ID = decayItem.GetFEE64ID();
    int Channel_ID = decayItem.GetChannelID();
    
    
    decayItem.SetLayer(FEE_allocation[FEE_ID - 1][0]);

    decayItem.SetFront_Back(FEE_allocation[FEE_ID - 1][1]);
    
    if(FEE_allocation[FEE_ID - 1][2] == 1) decayItem.SetRealChannelID(feeChannelOrder[Channel_ID]);
    if(FEE_allocation[FEE_ID - 1][2] == 2) decayItem.SetRealChannelID((127 - feeChannelOrder[Channel_ID]));

    return;

    /*if(FEE_allocation[FEE_ID - 1][1] == 0){
	
	//x_number++; // Set X to -1
    
	if(FEE_allocation[FEE_ID - 1][2] == 1){
	    
	    tmp_x = feeChannelOrder[Channel_ID];
	    
	}    
	if(FEE_allocation[FEE_ID - 1][2] == 2){
	    
	    tmp_x = (127 - feeChannelOrder[Channel_ID]);

	}
	
	decayItem.SetChannelID(tmp_x);
	
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
    
    decayItem.Set_Layer(tmp_z);*/

}

//---------------------------------------------------------------

/*void AIDA_Detector_System::get_implantation_coordinate(){
    
    int FEE_ID = implantItem.GetFEE64ID();
    int Channel_ID = implantItem.GetChannelID();

    if(FEE_allocation[FEE_ID - 1][1] == 0){
	
	//x_number++; // Set X to -1
    
	if(FEE_allocation[FEE_ID - 1][2] == 1){
	    
	    tmp_x = feeChannelOrder[Channel_ID];
	    
	}    
	if(FEE_allocation[FEE_ID - 1][2] == 2){
	    
	    tmp_x = (127 - feeChannelOrder[Channel_ID]);

	}
	
	implantItem.Set_X(tmp_x);
	
    }
    if(FEE_allocation[FEE_ID - 1][1] == 1){
	
	//y_check = true; // Set Y to -1
    
	if(FEE_allocation[FEE_ID - 1][2] == 1){
	    
	    tmp_y = feeChannelOrder[Channel_ID];

	    
	}    
	if(FEE_allocation[FEE_ID - 1][2] == 2){
	    
	    tmp_y = (127 - feeChannelOrder[Channel_ID]);
		    
	}
	
	implantItem.Set_Y(tmp_y);
	
    }
    
    tmp_z = FEE_allocation[FEE_ID - 1][0];
    
    implantItem.Set_Layer(tmp_z);

}*/

//---------------------------------------------------------------

int* AIDA_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

