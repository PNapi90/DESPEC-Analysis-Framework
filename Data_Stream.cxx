#include "Data_Stream.h"

using namespace std;


//---------------------------------------------------------------

Data_Stream::Data_Stream(){
	//set ids
	Known_ID[0] = "FRS";
	Known_ID[1] = "AIDA";
	Known_ID[2] = "PLASTIC";
	Known_ID[3] = "FATIMA";
	Known_ID[4] = "GALILEO";

	fat_iter = 0;
	FATIMA_Event_Length = 0;
}

//---------------------------------------------------------------

Data_Stream::~Data_Stream(){

}

//---------------------------------------------------------------

void Data_Stream::set_ID(string ID){
	for(int i = 0;i < 5;++i) if(Known_ID[i] == ID){
		ID_int = i;
		fat_iter = 0;
		return;
	}
	cerr << "Detector System not known to Data_Stream obj!";
	exit(0);
}

//---------------------------------------------------------------

void Data_Stream::set_amount_of_Events(int amount){
	amount_ID[ID_int] = amount;
}

//---------------------------------------------------------------

//Fatima function
void Data_Stream::set_event_data(double E,ULong T,ULong QDC_T,int total_hits,int hits_det,int det_ID){
	FATIMA_Det_ID[fat_iter] = det_ID;
	FATIMA_E[fat_iter] = E;
	FATIMA_T[fat_iter] = T;
	FATIMA_QDC_T[fat_iter] = QDC_T;
	FATIMA_hits_det[fat_iter] = hits_det;
	fat_iter++;

	FATIMA_Event_Length = fat_iter;
}

//---------------------------------------------------------------

//PLASTIC function
void Data_Stream::set_event_data(ULong* coarse_T,ULong* fine_T,UInt* ch_ID,ULong coarse_T_Trigger,ULong fine_T_Trigger){
	for(int i = 0;i < amount_ID[ID_int];++i){
		PLASTIC_Coarse_T[i] = coarse_T[i];
		PLASTIC_Fine_T[i] = fine_T[i];
		PLASTIC_ch_ID[i] = ch_ID[i];
	}
	PLASTIC_Coarse_T_Trigger = coarse_T_Trigger;
	PLASTIC_Fine_T_Trigger = fine_T_Trigger;
}

//---------------------------------------------------------------