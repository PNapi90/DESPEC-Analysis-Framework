#include "BS_Detector_System.h"

#include <cstdlib>
#include <map>




using namespace std;

//---------------------------------------------------------------

BS_Detector_System::BS_Detector_System(){

    //set amount of Germanium Detectors

    
}

//---------------------------------------------------------------

BS_Detector_System::~BS_Detector_System(){


}

//---------------------------------------------------------------

void Process_FRS(TGo4MbsSubEvent* psubevt){

    bool keep_unpacking = true;
    
    *pdata = psubevt->GetDataField();
    len = 0;	  
    vme_chn;
    lenMax = (psubevt->GetDlen()-2)/2;
    
      if(ElecMod->EventFlags.size()!=0)
    {
      Int_t  event_flag = *pdata++;
      len++; // 0x200
      
#ifdef DEBUG
      std::cout<<" Event FLAG :"<<std::endl;
      std::cout<<std::bitset<32>(event_flag)<<"\n";
      for(Int_t elem : ElecMod->EventFlags)
	std::cout<<std::bitset<32>(elem)<<"\n";
#endif
      bool foundFlag = false;
      for(int setupFlag : ElecMod->EventFlags)
	if(event_flag == setupFlag)
	  foundFlag = true;
      if(foundFlag)
	EventFlag = event_flag;
    }
    
    if(psubevt->GetProcid() == 10){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(vme5);
	
	if(psubevt->GetType() == 88){
	    
	    pdata++;
	    
	    len++;
	    
	    if(ElecMod->Nb_Scaler > 0){
		
		FRS_Scaler_Module(vme5);
		
	    }
	    
	    while (len < (psubevt->GetDlen()-2)/2)
	    {
		
		FRS_Rest_Of_Unpacking(vme5);
		
	    }   
	
	}
	
    }
    else if(psubevt->GetProcid() == 20){
	
	while(len < lenMax){
		
	    FRS_775_TDC_785_ADC_Modules(vme1);
		
	}
	
	while (len < (psubevt->GetDlen()-2)/2)
	{
		
	    FRS_Rest_Of_Unpacking(vme1);
		
	}   
	
	
    }
    else if(psubevt->GetProcid() == 30){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(vme0);
	
	if(psubevt->GetType() == 88){
	    	    
	    pdata++;
	    
	    len++;
	    
	    if(ElecMod->Nb_Scaler > 0){
		
		FRS_Scaler_Module(vme0);
		
	    }
	    
	    while(len < lenMax){
		
		FRS_775_TDC_785_ADC_Modules(vme0);
		
	    }
	    
	    while (len < (psubevt->GetDlen()-2)/2)
	    {
		
		FRS_Rest_Of_Unpacking(vme0);
		
	    }    
	    
	}
	
    }
    else if(psubevt->GetProcid() == 40){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(vme3);
	
	
	
    }
    else if(psubevt->GetProcid() == 50){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(vme4);
	
	
	
    }
    
    
    
    
    
     || psubevt->GetProcid() == 30 || 
	psubevt->GetProcid() == 40 || psubevt->GetProcid() == 50){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module();
	
	// NO MORE FOR PROC ID 50 //

	if(psubevt->GetType() != 88) keep_unpacking = false;
 
    }
    
    
    
    
}

void FRS_Timestamp_Module(UInt_t vme_num[21][32]){
    

    // \note FRS TIME STAMP module data (3 longwords)   
    //   has no header or end-of-block info, we must hardwire GEO = 20.
    //
    Long64_t tempTS = 0;
    static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
    for (int i=0;i<4;++i)
    {
	vme_num[20][i] = getbits(*pdata,1,1,16);
	tempTS += vme_num[20][i]*bit_weight[i];
	
	pdata++; len++;
    }
    
    previousTimeStamp[0] = currentTimeStamp;
    currentTimeStamp = tempTS;

}

void FRS_Scaler_Module(UInt_t vme_num[21][32]){


    // \note FRS SCALER module data (1 longword per channel)   
    //  This module has sequential readout therefore no channel
    //  number contained in the data longwords. 
    //
    // read the header longword and extract slot, type & length  
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,2,3,6);
    
    pdata++;
    
    len++;
    
    const auto it_Module = it_Crate->second.find(vme_geo);
	int IdMod = it_Module->second;
	if(it_Module == it_Crate->second.end())
	    std::cout<<"E> Crate Mapping does not have this module (vmeGEO==) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
    
    if(vme_type!=4)
	std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;
    
    // read the data 
    if (vme_nlw > 0){
	
	for(int i=0;i<vme_nlw;i++){
	    
	    if(ElecMod->Scaler32bit){
		
		vme_num[vme_geo][i] = *pdata;
	    
	    }
	    else{
		
		Int_t sc_data = get2bits(*pdata,1,1,26);
		Int_t  sc_ch = get2bits(*pdata,1,27,5);
		
		if(sc_ch != i)
		    std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
		    
		vme_num[vme_geo][i] = sc_data;
	    
	    }
	    
	    pdata++; 
	    
	    len++;
	    
	}
	
    }

}

void FRS_775_TDC_785_ADC_Modules(UInt_t vme_num[21][32]){
    
    
    /* read the header longword and extract slot, type & length  */
    Int_t vme_chn = 0;
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,1,9,6); 
    
    pdata++; len++;
    
    /* read the data */
    if ((vme_type == 2) && (vme_nlw > 0)){
	
	for(int i=0;i<vme_nlw;i++){  
	    
	    vme_geo = getbits(*pdata,2,12,5);
	    vme_type = getbits(*pdata,2,9,3);
	    vme_chn = getbits(*pdata,2,1,5);
	    vme_num[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    
	    pdata++;
	    
	    len++;
	    
	}
    
    }
    
    /* read and ignore the expected "end-of-block" longword */
    pdata++;
    
    len++;
    
}

void FRS_Rest_Of_Unpacking(UInt_t vme_num[21][32]){
    
    // read the header longword and extract slot, type & length  
    
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,1,9,6);
    
    pdata++;
    
    len++;
    
    const auto it_Module = it_Crate->second.find(vme_geo);
	int IdMod = it_Module->second;
    
    if(it_Module == it_Crate->second.end())
	std::cout<<"E> Crate Mapping does not have this module (vmeGEO--) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
    
    // read the data
    if ((vme_type == 2) && (vme_nlw > 0)){
	
	for(int i=0;i<vme_nlw;i++){
	      
	    vme_geo = getbits(*pdata,2,12,5);
	    vme_type = getbits(*pdata,2,9,3);
	    vme_chn = getbits(*pdata,2,1,5);
	    
	    vme_num[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    Int_t vme_statusVD = getbits(*pdata,14,1,1);
	    Int_t vme_statusUN = getbits(*pdata,13,1,1);
	    Int_t vme_statusOV = getbits(*pdata,12,1,1);
	    
	    pdata++;
	    
	    len++;
	
	}
	
	// read and ignore the expected "end-of-block" longword 
	pdata++;
	
	len++;
    
    }

}


void BS_Detector_System::get_Event_data(Raw_Event* RAW){
	
}

//---------------------------------------------------------------

void BS_Detector_System::Process_MBS(int* pdata){

}

int* BS_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

