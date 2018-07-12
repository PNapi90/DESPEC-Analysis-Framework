#include "AIDA_Detector_System.h"

#include <cstdlib>

using namespace std;

//---------------------------------------------------------------

AIDA_Detector_System::AIDA_Detector_System(){

}

//---------------------------------------------------------------

AIDA_Detector_System::~AIDA_Detector_System(){
}

//---------------------------------------------------------------

void AIDA_Detector_System::get_Event_data(Raw_Event* RAW){



}

//---------------------------------------------------------------

void AIDA_Detector_System::Process_MBS(int* pdata){

    this->pdata = pdata;
    
    
    while(!End_of_AIDA){
    
	AIDA_t0_Header* t0_head  = (AIDA_t0_Header*) this->pdata;
	AIDA_ADC_Times* ADC_head  = (AIDA_ADC_Times*) this->pdata;
	AIDA_1st_Disc* AIDA_disc  = (AIDA_1st_Disc*) this->pdata;
    
	if(t0_head->check == 2 && t0_head->second_check == 5) Check_AIDA_t0_DATA(t0_head);
	else if(ADC_head->check == 3) Check_AIDA_ADC_DATA();
	else if(AIDA_disc->check == 8 && AIDA_disc->second_check == 6) Check_AIDA_Disc_DATA();
	
	this->pdata++;

	/*if(something happens)*/ End_of_AIDA = true;

    }
}

//---------------------------------------------------------------

void AIDA_Detector_System::Check_AIDA_t0_DATA(AIDA_t0_Header* t0_head){
    
    
    tmp_AIDA_t0_0_15 = t0_head->t0_1st;

    pdata++;
    
    AIDA_t0_Header_2* t0_head_2  = (AIDA_t0_Header_2*) pdata;

    tmp_AIDA_t0_36_63 = t0_head_2->t0_3rd;
    
    pdata++;
    
    AIDA_t0_Header_3* t0_head_3  = (AIDA_t0_Header_3*) pdata;

    tmp_AIDA_t0_16_35 = t0_head_3->t0_2nd;
    
    
    AIDA_t0 	 =  tmp_AIDA_t0_0_15 + (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_36_63 << 48);
    AIDA_t0_base =  (tmp_AIDA_t0_16_35 << 28) + (tmp_AIDA_t0_36_63 << 48);


}

//---------------------------------------------------------------

void AIDA_Detector_System::Check_AIDA_ADC_DATA(){
    
    
    pdata++;
    
    AIDA_ADC_Times_2* ADC_head_2  = (AIDA_ADC_Times_2*) pdata;

    tmp_ADC_Timestamp_end = ADC_head_2->ADC_time;
    
    ADC_Timestamp_full = AIDA_t0_base + tmp_ADC_Timestamp_end;
    
}

//---------------------------------------------------------------

void AIDA_Detector_System::Check_AIDA_Disc_DATA(){
    
    
    pdata++;
    
    AIDA_1st_Disc_2* AIDA_disc_2 = (AIDA_1st_Disc_2*) pdata;

    tmp_AIDA_Discriminator_end = AIDA_disc_2->first_disc_time;
    
    AIDA_Discriminator_full = AIDA_t0_base + tmp_AIDA_Discriminator_end;
    
}

//---------------------------------------------------------------

int* AIDA_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

