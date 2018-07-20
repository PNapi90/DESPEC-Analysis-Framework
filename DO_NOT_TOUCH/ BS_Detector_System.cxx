#include "BS_Detector_System.h"

#include <cstdlib>
#include <map>

#include <iostream>
#include <bitset>
#include <sstream>
#include <fstream>
#include "TObject.h"

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"
#include "TGo4StepFactory.h"
#include "TGo4Analysis.h"


using namespace std;

//---------------------------------------------------------------

BS_Detector_System::BS_Detector_System(){

    //set amount of Germanium Detectors

    
}

//---------------------------------------------------------------

BS_Detector_System::~BS_Detector_System(){


}

//---------------------------------------------------------------

void BS_Detector_System::Process_FRS(TGo4MbsSubEvent* psubevt){
    
    Setup_Parameters();
    FRS_Unpack(psubevt);
    FRS_Sort();
    
    cout<<"HOLY SHIT IT ACTUALLY FUCKING WORKED!!! "<<dt_21l_21r<<endl;
    
}

//-----------------------------------------------------------------//
//--------------------- UNPACKER STUFF ----------------------------//
//-----------------------------------------------------------------//

void BS_Detector_System::FRS_Unpack(TGo4MbsSubEvent* psubevt){

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
	
	if(psubevt->GetType() == 88){

	    pdata++;
	    
	    len++; // remove 0xbaba.baba
	    
	    
	    // v820 -> no header 16 ch of 32 bits.
	    for(int i=0;i<16;++i){
		
		vme3scaler[i] = *pdata;
		
		pdata++;
		
		len++;
		
	    }
	    
	    
	    static std::bitset<32> barrier_test(0xbabababa);
	    if(std::bitset<32>(*pdata) !=barrier_test)
	    std::cout<<"E> ProcID 40 : Barrier missed !"<<std::bitset<32>(*pdata)<<" =/= 0xbabababa | "<<barrier_test<<std::endl;
	    
	    pdata++;
	    
	    len++;
	    
	    
	    if(ElecMod->Nb_TDC>0){
		
		//v1290 TDC              
		Int_t vme_geo = getbits(*pdata,1,1,5);
		Int_t vme_type = getbits(*pdata,2,12,5);
		
		pdata++;
		
		len++;
		
		Int_t multihit = 0;//, counter = 0;
		
		const auto it_Module = it_Crate->second.find(vme_geo);
		if(it_Module == it_Crate->second.end())
		std::cout<<"E> Crate Mapping does not have this module (vmeGEO//) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
		
		
		if (vme_type == 8){
		    
		    while (len < lenMax){
			
			vme_type = getbits(*pdata,2,12,5);
			if(vme_type==1){ // headerTDC
	
			    pdata++;
			    
			    len++;
	
			}
	
			//multihit = 0;
			
			vme_type = getbits(*pdata,2,12,5);
			
			if(vme_type == 0){
			    
			    // this indicates a TDC measurement
			    Int_t vme_chn = getbits(*pdata,2,6,5);
			    Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
			    Int_t value = getbits(*pdata,1,1,21);
			    multihit = vme3_MT_nhit5[vme_chn][LeadingOrTrailing];
			    
			    if (multihit >= 10){
				
				pdata++;
				
				len++;
				
				continue;
	
			    }
			    if(LeadingOrTrailing == 0){
				
				if (value > 0){
				    
				    vme3s_MT[vme_chn][multihit] = value;
				    //hVME2_TDC[vme_chn]->Fill(value);
				
				}
				
			    }
			    
			    else{
				
				if (value > 0) vme3s_MT_trailing[vme_chn][multihit] = value;
			    
			    }
			    
			    vme3_MT_nhit5[vme_chn][LeadingOrTrailing]++;
			    
			    pdata++;
			    
			    len++;
			    
			}
			else{
			    
			    // TDC trailer vme_type == 3
			    if(vme_type != 3 && vme_type !=16) std::cout<<"E> MTDC strange type :"<<vme_type<<std::endl;
			    
			    if(vme_type==16){
				
				Int_t vme_geoEnd = getbits(*pdata,1,1,5);
				
				if(vme_geo!=vme_geoEnd)
				    std::cout<<"E> MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl; 
				
				break;
			    }
			    
			    pdata++;
			    
			    len++;
			
			}
		    }
		}
	    }
	}
    }
    else if(psubevt->GetProcid() == 50){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(vme4);
	
	
	
    }
}

//---------------------------------------------------------------

void BS_Detector_System::FRS_Timestamp_Module(UInt_t vme_num[21][32]){
    

    // \note FRS TIME STAMP module data (3 longwords)   
    //   has no header or end-of-block info, we must hardwire GEO = 20.
    //
    Long64_t tempTS = 0;
    static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
    for (int i=0;i<4;++i){
	
	vme_num[20][i] = getbits(*pdata,1,1,16);
	tempTS += vme_num[20][i]*bit_weight[i];
	
	pdata++;
	
	len++;
    
    }
    
    previousTimeStamp[0] = currentTimeStamp;
    currentTimeStamp = tempTS;

}

//---------------------------------------------------------------

void BS_Detector_System::FRS_Scaler_Module(UInt_t vme_num[21][32]){


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

//---------------------------------------------------------------

void BS_Detector_System::FRS_775_TDC_785_ADC_Modules(UInt_t vme_num[21][32]){
    
    
    /* read the header longword and extract slot, type & length  */
    Int_t vme_chn = 0;
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,1,9,6); 
    
    pdata++;
    
    len++;
    
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

//---------------------------------------------------------------

void BS_Detector_System::FRS_Rest_Of_Unpacking(UInt_t vme_num[21][32]){
    
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

//-----------------------------------------------------------------//
//----------------------- SORT STUFF ------------------------------//
//-----------------------------------------------------------------//

void BS_Detector_System::FRS_Sort(){
    

    if(EventFlag == 0x100){
	for(int i = 0; i < 4; ++i) ts_word[i] = vme0[20][i];
    }
    else if(EventFlag == 0x200){
	for(int i = 0; i < 4; ++i) ts_word[i] = vme3[20][i];
    }
    else if(EventFlag == 0x300){
	for(int i = 0; i < 4; ++i) ts_word[i] = vme4[20][i];
    }

    timestamp = Long64_t(1)*ts_word[0] + Long64_t(0x10000)*ts_word[1] + Long64_t(0x100000000)*ts_word[2] + Long64_t(0x1000000000000)*ts_word[3];
    // printf("qtrigger=%d timestamp=%ld \n",qtrigger,timestamp);
    
    tsys_word[0] = Long64_t(1)*ts_word[0] + Long64_t(0x10000)*ts_word[1] ; //s time low word
    tsys_word[1] = Long64_t(0x100000000)*ts_word[2] + Long64_t(0x1000000000000)*ts_word[3] ; //s time high worid... we do not use it

    if(PreviousTS < 0) tsys_word[2] = 0;
 
    else tsys_word[2] = (timestamp - PreviousTS)*1.e-5 ; //ms time since the previous s time (ftime routine)

    systemtime_s = tsys_word[2]*1e-3; //tsys_word[0] ; 
    systemtime_ms= tsys_word[2]; 


    if (qtrigger == 12){
	
	StartOfSpilTime = timestamp;
      
	StartOfSpilTime2 = timestamp;
    }   
    else if (qtrigger == 13){
      
	StartOfSpilTime = -1;
    }
    // rest are interesting only if trigger == 1
  
    // calculate time from spill start in sec
    if (StartOfSpilTime >= 0){
	 
	timespill = (timestamp - StartOfSpilTime) * 1e-2;// microsec // 50000000.;
    }
  
    timespill2 = (timestamp - StartOfSpilTime2) * 1e-2; //microsec  // 50000000.;

    /* ### Pattern ###*/
    pattern = vme0[5][0];
    trigger = qtrigger;
  

  /* ### scalers:  */
  /* these are treated as 32-bit integers directly  */
    for(int i = 0 ;i < 32; ++i){
	
	sc_long[i]  = vme0[0][i]; //frs crate
	sc_long[32+i] = vme5[8][i]; //main crate

    }

  /* ### TA Ionization Chamber dE:  */
  
    
  /* ### MW anodes:  */
  for(int i = 0; i < 13; ++i)
    mw_an[i] = vme0[8][i] & 0xfff;
    
  /* ### MW cathodes:  */
  // from MW11 -> MW31
  for(int i = 0; i < 4; ++i)
    {  
      mw_xr[i] = vme0[8][16+i*4] & 0xfff;
      mw_xl[i] = vme0[8][17+i*4] & 0xfff;
      mw_yu[i] = vme0[8][18+i*4] & 0xfff;
      mw_yd[i] = vme0[8][19+i*4] & 0xfff;
    }
  // from MW41 -> MW51
  for(int i = 0 ;i < 3; ++i)
    {
      mw_xr[i+4] = vme0[9][0+i*4] & 0xfff;
      mw_xl[i+4] = vme0[9][1+i*4] & 0xfff;
      mw_yu[i+4] = vme0[9][2+i*4] & 0xfff;
      mw_yd[i+4] = vme0[9][3+i*4] & 0xfff;
    }
  // skip MW61
  //putting MW61 all to zero (09.07.2018)
      mw_xr[7] = 0;
      mw_xl[7] = 0;
      mw_yu[7] = 0;
      mw_yd[7] = 0;
     
  // from MW71 -> MWB2
  for(int i = 0; i < 5; ++i)
    {
      mw_xr[i+8] = vme0[9][12+i*4] & 0xfff;
      mw_xl[i+8] = vme0[9][13+i*4] & 0xfff;
      mw_yu[i+8] = vme0[9][14+i*4] & 0xfff;
      mw_yd[i+8] = vme0[9][15+i*4] & 0xfff;
    }



    //////////////////////////////////////
    //                                  //
    // 		  TPC part              //
    //                                  //
    //////////////////////////////////////
    
    //ADC
    for(int i = 0; i < 4; ++i;){ // TPC 1->4 at S2 (TPC 21 & 22) in vacuum (TPC 23 & 24) in air
    	
	tpc_a[i][0]=vme1[15][0 + i*8] & 0xfff;
	tpc_a[i][1]=vme1[15][1 + i*8] & 0xfff;
	tpc_a[i][2]=vme1[15][2 + i*8] & 0xfff;
	tpc_a[i][3]=vme1[15][3 + i*8] & 0xfff;
	tpc_l[i][0]=vme1[15][4 + i*8] & 0xfff;
	tpc_r[i][0]=vme1[15][5 + i*8] & 0xfff;
	tpc_l[i][1]=vme1[15][6 + i*8] & 0xfff;
	tpc_r[i][1]=vme1[15][7 + i*8] & 0xfff;
	
	
	tpc_dt[i][0]=vme1[8][0 + i*8] & 0xfff;
	tpc_dt[i][1]=vme1[8][1 + i*8] & 0xfff;
	tpc_dt[i][2]=vme1[8][2 + i*8] & 0xfff;
	tpc_dt[i][3]=vme1[8][3 + i*8] & 0xfff;
	tpc_lt[i][0]=vme1[8][4 + i*8] & 0xfff;
	tpc_rt[i][0]=vme1[8][5 + i*8] & 0xfff;
	tpc_lt[i][1]=vme1[8][6 + i*8] & 0xfff;
	tpc_rt[i][1]=vme1[8][7 + i*8] & 0xfff;
	
	
	
    }
    for(int i = 0; i < 3; ++i;){ // TPC 5 & 6 at S4 (TPC 41 & 42) in air & TPC at S3 (TPC 31) (CheckSB)
    	
	tpc_a[i+4][0]=vme1[3][0 + i*8] & 0xfff;
	tpc_a[i+4][1]=vme1[3][1 + i*8] & 0xfff;
	tpc_a[i+4][2]=vme1[3][2 + i*8] & 0xfff;
	tpc_a[i+4][3]=vme1[3][3 + i*8] & 0xfff;
	tpc_l[i+4][0]=vme1[3][4 + i*8] & 0xfff;
	tpc_r[i+4][0]=vme1[3][5 + i*8] & 0xfff;
	tpc_l[i+4][1]=vme1[3][6 + i*8] & 0xfff;
	tpc_r[i+4][1]=vme1[3][7 + i*8] & 0xfff;
	
	tpc_dt[i+4][0]=vme1[9][0 + i*8] & 0xfff;
	tpc_dt[i+4][1]=vme1[9][1 + i*8] & 0xfff;
	tpc_dt[i+4][2]=vme1[9][2 + i*8] & 0xfff;
	tpc_dt[i+4][3]=vme1[9][3 + i*8] & 0xfff;
	tpc_lt[i+4][0]=vme1[9][4 + i*8] & 0xfff;
	tpc_rt[i+4][0]=vme1[9][5 + i*8] & 0xfff;
	tpc_lt[i+4][1]=vme1[9][6 + i*8] & 0xfff;
	tpc_rt[i+4][1]=vme1[9][7 + i*8] & 0xfff;
	
    }

  
    /* ### SCI dE:  */
    de_21l = vme5[11][2] & 0xfff;
    de_21r = vme5[11][3] & 0xfff;
 
    de_41l = vme5[11][0] & 0xfff;
    de_41r = vme5[11][1] & 0xfff;
    de_42l = vme5[11][4] & 0xfff; 
    de_42r = vme5[11][5] & 0xfff;
    de_43l = vme5[11][11] & 0xfff;
    de_43r = vme5[11][12] & 0xfff;  
  
    de_81l = vme5[11][13] & 0xfff;
    de_81r = vme5[11][14] & 0xfff;
  
    de_v1l = 0;
    de_v1r = 0;  
    de_v2l = 0;
    de_v2r = 0;
    de_v3  = 0;

    
    /* ### SCI times:  */
    dt_21l_21r = vme0[12][0] & 0xfff;
    dt_41l_41r = vme0[12][1] & 0xfff;
    dt_21l_41l = vme0[12][5] & 0xfff;
    dt_21r_41r = vme0[12][6] & 0xfff;
    dt_42l_42r = vme0[12][2] & 0xfff;  
    dt_42l_21l = vme0[12][7] & 0xfff; //
    dt_42r_21r = vme0[12][8] & 0xfff; //
    dt_43l_43r = vme0[12][3] & 0xfff; //
    dt_81l_81r = vme0[12][4] & 0xfff;
    dt_21l_81l = vme0[12][9] & 0xfff;
    dt_21r_81r = vme0[12][10] & 0xfff;

 
  /* ### SCI Multihit TDC time:  */
    for(int i = 0; i < 10; ++i){
    
	tdc_sc41l[i] = vme2s[0][i];
	tdc_sc41r[i] = vme2s[1][i];
	tdc_sc21l[i] = vme2s[2][i];
	tdc_sc21r[i] = vme2s[3][i];
	tdc_sc42l[i] = vme2s[4][i];
	tdc_sc42r[i] = vme2s[5][i];
	tdc_sc43l[i] = vme2s[6][i];
	tdc_sc43r[i] = vme2s[7][i];
	tdc_sc81l[i] = vme2s[8][i];
	tdc_sc81r[i] = vme2s[9][i];
    
    }
  

  /* ### MUSIC OLD:  */
    for(int i=0;i<4;i++){
      
	music_t3[i] = vme1[11][16+i] & 0xfff;    //TIME
	//music_t3[i] = 0;
	music_e3[i] = vme0[10][16+i] & 0xfff;    //ENERGY
	//music_e3[i] = 0;

    }
               
    /* ### TUM MUSIC dE:  */
    for(int i=0;i<8;i++){
	
	music_e1[i] = vme0[11][i] & 0xfff;
	music_e2[i] = vme0[11][8+i] & 0xfff;
	
	//music_e2[i] = vme0[12][24+i] & 0xfff;
	
	music_t1[i] = vme2s[16+i][0] & 0xfff; //TIME
	music_t2[i] = vme2s[24+i][0] & 0xfff; //TIME
    
    }
    
    /* ### MUSIC temp & pressure:  */
    music_pres[0] = 0; 
    music_temp[0] = 0;
    
    /*  not included in readout! */
    music_pres[1] = 0;
    music_temp[1] = 0;
    
    /*  not included in readout! */
    music_pres[2] = 0;
    music_temp[2] = 0;
    
    //Single anode 
    SingleAnode_adc = vme0[10][20] & 0xfff; 
    
    //Si detectors 
    si_adc1 = vme0[10][24] & 0xfff; 
    si_adc2 = vme0[10][25] & 0xfff;
    si_adc3 = vme0[10][26] & 0xfff; 
    si_adc4 = vme0[10][27] & 0xfff;
    si_adc5 = vme0[10][28] & 0xfff;
    
    //Si detectors DSSD = alpha tagger
    for(int i=0;i<32;i++){
	
	dssd_adc[i] = vme0[7][i] & 0xfff;
    
    }

    //Channeltron detectors (vme must be adjusted)
    ct_time = vme3s_MT[2][0] & 0xfff;
    ct_signal = vme2scaler[20] & 0xfff; 
    ct_trigger_DU = vme2scaler[5]& 0xfff;
    ct_trigger_SY = vme2scaler[6]& 0xfff;
    
    //Electron current measurement (vme must be adjused)
    ec_signal = vme0[10][1]& 0xfff;
    
    // mrtof
    mrtof_start = vme3s_MT[0][0] ;//& 0x7ffff;
    mrtof_stopDelay = vme3s_MT[1][0];// & 0x7ffff;
    mrtof_stop = vme3s_MT[2][0] ;//& 0x7ffff;
    
    
}


void BS_Detector_System::Setup_Parameters(){

  /*fFRSPar = new TXRSParameter("FRSPar");
  AddParameter(fFRSPar);

  fMWPar = new TMWParameter("MWPar");
  AddParameter(fMWPar);

  fMUSICPar = new TMUSICParameter("MUSICPar");
  AddParameter(fMUSICPar);

  fTPCPar = new TTPCParameter("TPCPar");
  AddParameter(fTPCPar);

  fSCIPar = new TSCIParameter("SCIPar");
  AddParameter(fSCIPar);

  fIDPar = new TIDParameter("IDPar");
  AddParameter(fIDPar);

  fSIPar = new TSIParameter("SIPar");
  AddParameter(fSIPar);

  ModPar = new TModParameter("ModPar");
  AddParameter(ModPar);
  
  MRtofPar = new TMRTOFMSParameter("MRTOFMSPar");
  AddParameter(MRtofPar);*/


  // look up analysis object and all parameters
  
  frs = new TXRSParameter("FRSPar");
  //AddParameter(fFRSPar);

  mw = new TMWParameter("MWPar");
  //AddParameter(fMWPar);

  music = new TMUSICParameter("MUSICPar");
  //AddParameter(fMUSICPar);

  tpc = new TTPCParameter("TPCPar");
  //AddParameter(fTPCPar);

  sci = new TSCIParameter("SCIPar");
  //AddParameter(fSCIPar);

  id = new TIDParameter("IDPar");
  //AddParameter(fIDPar);

  si = new TSIParameter("SIPar");
  //AddParameter(fSIPar);

  ElecMod = new TModParameter("ModPar");
  //AddParameter(ModPar);
  
  mrtof = new TMRTOFMSParameter("MRTOFMSPar");
  //AddParameter(MRtofPar);
  
  //ElecMod = new TModParameter("ModPar");

  
  //TModParameter* ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));

    
  /*TXRSAnalysis* an = dynamic_cast<TXRSAnalysis*> (TGo4Analysis::Instance());
  if (an==0) {
    cout << "!!!  Script should be run in FRS analysis" << endl;
  }
  
  ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));
  if (ElecMod==0) {
    cout << "!!! Parameter ModPar not found " << endl;
  }
  
  
   
  TXRSParameter* frs = dynamic_cast<TXRSParameter*> (an->GetParameter("FRSPar"));
  if (frs==0) {
    cout << "!!!  Parameter FRSPar not found" << endl;
  }

  TMWParameter* mw = dynamic_cast<TMWParameter*> (an->GetParameter("MWPar"));
  if (mw==0) {
    cout << "!!!  Parameter MWPar not found" << endl;
  }

  TMUSICParameter* music = dynamic_cast<TMUSICParameter*> (an->GetParameter("MUSICPar"));
  if (music==0) {
    cout << "!!!  Parameter MUSICPar not found" << endl;
  }

  TSCIParameter* sci = dynamic_cast<TSCIParameter*> (an->GetParameter("SCIPar"));
  if (sci==0) {
    cout << "!!!  Parameter SCIPar not found" << endl;
  }

  TIDParameter* id = dynamic_cast<TIDParameter*> (an->GetParameter("IDPar"));
  if (id==0) {
    cout << "!!!  Parameter IDPar not found" << endl;
  }
   
  TTPCParameter* tpc = dynamic_cast<TTPCParameter*> (an->GetParameter("TPCPar"));
  if (tpc==0) {
    cout << "!!!  Parameter TPCPar not found" << endl;
  }

  TSIParameter* si = dynamic_cast<TSIParameter*> (an->GetParameter("SIPar"));
  if (si==0) {
    cout << "!!!  Parameter SIPar not found" << endl;
  }

  TMRTOFMSParameter* mrtof = dynamic_cast<TMRTOFMSParameter*> (an->GetParameter("MRTOFMSPar"));
  if (mrtof==0) {
    cout << "!!!  Parameter MR-TOF-MSPar not found" << endl;
  }
  
  TModParameter* ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));
   
  cout << endl << "setup script started" << endl;
   

  // setup part 
  an->SetupH2("ID_x4AoQ", 500, 1.3, 2.8, 200, -100, +100, "A/Q", "X4 [mm]");     
  an->SetupH2("ID_Z_AoQ", 600, 1.3, 2.8,600 , 0, 20, "A/Q", "Z"); 
  an->SetupH2("ID_Z_AoQ_corr", 500, 1.3, 2.8, 600, 0, 20, "A/Q (a2 corr)", "Z"); 
  an->SetupH2("ID_x4z", 450, 0., 20.0, 500, -100, 100, "Z", "X4 [mm]"); */

  //      ID_dEToF
  // Float_t my_cID_Z_AoQ_points[5][2] =
  // // s411_57 133I
  // //	{{ 2.5184., 55.8700},
  // //	{ 2.5184., 57.1700},
  // //	{ 2.5305., 57.1700},
  // //	{ 2.5305., 55.7700},
  // //	{ 2.5184., 55.8700}};
  // // s411_57 133Te
  // //	{{ 2.5461., 56.2410},
  // //	{ 2.5461., 55.3040},
  // //	{ 2.5612., 55.3040},
  // //	{ 2.5612., 56.2410},
  // //	{ 2.5461., 56.2410}};
  // // s411_ 213Fr
  //   {{ 2.45205, 86.7875},
  //    { 2.46124, 86.7458},
  //    { 2.46202, 88.1625},
  //    { 2.4496, 88.1417},
  //    { 2.45205, 86.7875}};
  // an->SetupPolyCond("cID_Z_AoQ(0)", 5, my_cID_Z_AoQ_points);


  Float_t my_cID_dEToF_points[4][2] = 
    {{    0.,    0.},
     {    0., 4000.},
     {40000., 4000.},
     {40000.,    0.}}; 
  //an->SetupPolyCond("cID_dEToF", 4, my_cID_dEToF_points);
   

  // /* 20Mg */
  // Float_t my_20mg_points[6][2]=
  //   {{1.665,        13.1000},
  //    {1.705,        12.9500},
  //    {1.705,        12.2500},
  //    {1.665,        12.1000},
  //    {1.624,        12.2500},
  //    {1.624,        12.9500}};

  //an->SetupPolyCond("cID_Z_AoQ(3)", 6, my_20mg_points);


  // setup FRS parameter

  //======
  //  MW
  //======

  mw->x_factor[0] = 0.25; // MW11 [mm/ns] 14.09.05 CN+AM 2ns/mm delay line 
  mw->x_factor[1] = 0.25; // MW21
  mw->x_factor[2] = 0.25; // MW22
  mw->x_factor[3] = 0.25; // MW31
  mw->x_factor[4] = 0.25; // MW41 
  mw->x_factor[5] = 0.25; // MW42
  mw->x_factor[8] = 0.25; // MW71
  mw->x_factor[9] = 0.25; // MW81   not modified
  mw->x_factor[10] = 0.125; // MW82 [mm/ns] 11.05.06  CN 4ns/mm delay line

   
  //   mw->x_offset[0] = -0.4; //  MW11  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[0] = 5.0; // Feb 2014
  //   mw->x_offset[1] = 2.0;  //  MW21  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[1] = -2.0; // Feb 2014
  //mw->x_offset[2] = -1.0; //  MW22  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[2] = -1.5; // Feb 2014
  mw->x_offset[3] = -0.205; // MW31 //10/05/06, ok also 20.01.09, ok also Feb 2014
  mw->x_offset[4] = 0.;  // MW41
  mw->x_offset[5] = -9.; // MW42 20.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[8] = 1.642; // MW71 //15/05/06
  mw->x_offset[9] = 1.;   // MW81 //11/05/06
  mw->x_offset[10] = -5.; // MW82 //27-MAY-2007

  mw->y_factor[0] = 0.25; // MW11 [mm/ns] 14.09.05 CN+AM 2ns/mm delay line 
  mw->y_factor[1] = 0.25; // MW21
  mw->y_factor[2] = 0.25; // MW22
  mw->y_factor[3] = 0.25; // MW31
  mw->y_factor[4] = 0.25; // MW41
  mw->y_factor[5] = 0.25; // MW42
  mw->y_factor[8] = 0.25; // MW71
  mw->y_factor[9] = 0.25; // MW81
  mw->y_factor[10] = 0.125; // MW82  [mm/ns] 11.05.06  CN 4ns/mm delay line

   
  mw->y_offset[0] = -14.0; // MW11 27-MAY-2007 TESTED VALUE WITH SLITS, ok Feb 2014
  //   mw->y_offset[1] = -5.0;  // MW21 27-MAY-2007 TESTED VALUE WITH SLITS
  mw->y_offset[1] = 21.0;   // Feb 2014
  mw->y_offset[2] = -1.0;  // MW22 27-MAY-2007 TESTED VALUE WITH SLITS, ok Feb 2014
  mw->y_offset[3] = 0.0;   // MW31 18-MAY-2007, ok Feb 2014
  mw->y_offset[4] = 0.;  // MW41
  mw->y_offset[5] = 0.;  // MW42
  mw->y_offset[8] = -2.736;  // MW71 //15/05/06
  mw->y_offset[9] = 3.2;     // MW81 //11/05/06
  mw->y_offset[10] = 0.764;  // MW82 //11/05/06


  mw->gain_tdc[0][0] = 0.302929; //  MW11 Anode (#ch  0 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][0] = 0.303253; //  MW11 XL    (#ch 17 TDC V775a)
  mw->gain_tdc[2][0] = 0.303975; //  MW11 XR    (#ch 16 TDC V775a)
  mw->gain_tdc[3][0] = 0.308414; //  MW11 YU    (#ch 18 TDC V775a)
  mw->gain_tdc[4][0] = 0.309826; //  MW11 YD    (#ch 19 TDC V775a)

  mw->gain_tdc[0][1] = 0.306064; //  MW21 Anode (#ch  1 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][1] = 0.306958; //  MW21 XL    (#ch 21 TDC V775a)
  mw->gain_tdc[2][1] = 0.307799; //  MW21 XR    (#ch 20 TDC V775a)
  mw->gain_tdc[3][1] = 0.297774; //  MW21 YU    (#ch 22 TDC V775a)
  mw->gain_tdc[4][1] = 0.310235; //  MW21 YD    (#ch 23 TDC V775a)

  mw->gain_tdc[0][2] = 0.301179;  // MW22 Anode (#ch  2 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][2] = 0.311121; //  MW22 XL    (#ch 25 TDC V775a)
  mw->gain_tdc[2][2] = 0.303233; //  MW22 XR    (#ch 24 TDC V775a)
  mw->gain_tdc[3][2] = 0.300558; //  MW22 YU    (#ch 26 TDC V775a)
  mw->gain_tdc[4][2] = 0.301105; //  MW22 YD    (#ch 27 TDC V775a)

  mw->gain_tdc[0][3] = 0.304426; //  MW31 Anode (#ch  3 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][3] = 0.312163; //  MW31 XL    (#ch 29 TDC V775a)
  mw->gain_tdc[2][3] = 0.305609; //  MW31 XR    (#ch 28 TDC V775a)
  mw->gain_tdc[3][3] = 0.304716; //  MW31 YU    (#ch 30 TDC V775a)
  mw->gain_tdc[4][3] = 0.293695; //  MW31 YD    (#ch 31 TDC V775a)

  mw->gain_tdc[0][4] = 0.298871; //  MW41 Anode (#ch  4 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][4] = 0.284086; //  MW41 XL    (#ch 1 TDC V775b)
  mw->gain_tdc[2][4] = 0.288656; //  MW41 XR    (#ch 0 TDC V775b)
  mw->gain_tdc[3][4] = 0.286589; //  MW41 YU    (#ch 2 TDC V775b)
  mw->gain_tdc[4][4] = 0.29269;  //  MW41 YD    (#ch 3 TDC V775b)

  mw->gain_tdc[0][5] = 0.297881; //  MW42 Anode (#ch  5 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][5] = 0.287364; //  MW42 XL    (#ch 5 TDC V775b)
  mw->gain_tdc[2][5] = 0.289636; //  MW42 XR    (#ch 4 TDC V775b)
  mw->gain_tdc[3][5] = 0.291135; //  MW42 YU    (#ch 6 TDC V775b)
  mw->gain_tdc[4][5] = 0.289867; //  MW42 YD    (#ch 7 TDC V775b)

  mw->gain_tdc[0][6] = 0.307892; //  MW51 Anode (#ch  6 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][6] = 0.289894; //  MW51 XL    (#ch  9 TDC V775b)
  mw->gain_tdc[2][6] = 0.292366; //  MW51 XR    (#ch  8 TDC V775b)
  mw->gain_tdc[3][6] = 0.284708; //  MW51 YU    (#ch 10 TDC V775b)
  mw->gain_tdc[4][6] = 0.28186;  //  MW51 YD    (#ch 11 TDC V775b)

  mw->gain_tdc[0][7] = 0.298266; //  MW61 Anode (#ch  7 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][7] = 0.311; //  MW61 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][7] = 0.305; //  MW61 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][7] = 0.337; //  MW61 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][7] = 0.289; //  MW61 YD    (#ch ? TDC V775b)

  mw->gain_tdc[0][8] = 0.303602; //  MW71 Anode (#ch  8 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][8] = 0.300082; //  MW71 XL    (#ch 13 TDC V775b)
  mw->gain_tdc[2][8] = 0.286092; //  MW71 XR    (#ch 12 TDC V775b)
  mw->gain_tdc[3][8] = 0.294287; //  MW71 YU    (#ch 14 TDC V775b)
  mw->gain_tdc[4][8] = 0.291341; //  MW71 YD    (#ch 15 TDC V775b)

  mw->gain_tdc[0][9] = 0.306041; //  MW81 Anode (#ch  9 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][9] = 0.288468; //  MW81 XL    (#ch 17 TDC V775b)
  mw->gain_tdc[2][9] = 0.293831; //  MW81 XR    (#ch 16 TDC V775b)
  mw->gain_tdc[3][9] = 0.281296; //  MW81 YU    (#ch 18 TDC V775b)
  mw->gain_tdc[4][9] = 0.279099; //  MW81 YD    (#ch 19 TDC V775b)

  mw->gain_tdc[0][10] = 0.31314;  //  MW82 Anode (#ch 10 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][10] = 0.287279; //  MW82 XL    (#ch 21 TDC V775b)
  mw->gain_tdc[2][10] = 0.284028; //  MW82 XR    (#ch 20 TDC V775b)
  mw->gain_tdc[3][10] = 0.28051;  //  MW82 YU    (#ch 22 TDC V775b)
  mw->gain_tdc[4][10] = 0.28743;  //  MW82 YD    (#ch 23 TDC V775b)

  mw->gain_tdc[0][11] = 0.299973; //  MWB21 Anode (#ch 11 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][11] = 0.311; //  MWB21 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][11] = 0.305; //  MWB21 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][11] = 0.337; //  MWB21 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][11] = 0.289; //  MWB21 YD    (#ch ? TDC V775b)

  mw->gain_tdc[0][12] = 0.306923; //  MWB22 Anode (#ch 12 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][12] = 0.311; //  MWB22 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][12] = 0.305; //  MWB22 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][12] = 0.337; //  MWB22 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][12] = 0.289; //  MWB22 YD    (#ch ? TDC V775b)


  // frs->dist_focS2   = 2280.0; // [mm] used in exp s441_oct12
  // frs->dist_focS2   = 3450.0; // [mm] 8.8.2012 for s388: s271 optics with focus 1170 mm after S2 midplane
  //   frs->dist_focS2   = 1228.0; // [mm] 8.8.2012 for s388 position calculated at S2 X-slits
  frs->dist_focS2   = 2013.; // [mm] at wedge position S411
  //frs->dist_focS2   = 2860.; // [mm] at wedge position S417 for dE

  frs->dist_MW21    =  604.0; // 
  frs->dist_MW22    = 1782.5; // 
  frs->dist_SC21    = 1554.5; // 
  //   frs->dist_MW41    = 1140.0; // ok on air
  //   frs->dist_MW42    = 2600.0; // ok on air

  frs->dist_focS4   = 2349.0; // at SC411   

  //**S323,410 exp.
  frs->dist_MUSIC1  = 555.0;  // TUM1
  frs->dist_MUSIC2  = 1210.0; // TUM2 , now removed
  frs->dist_MUSIC3  = 1210.0; // Music2 to be adjusted
 
  frs->dist_MUSICa1 = 52.5;  // do not change
  frs->dist_MUSICa2 = 157.5; // do not change
  frs->dist_MUSICa3 = 262.5; // do not change
  frs->dist_MUSICa4 = 367.5; // do not change

  //=========
  // MUSICs
  //=========

  //MUSIC41
  music->e1_off[0]   = 0.; //MUSIC41 offsets
  music->e1_off[1]   = 0.; 
  music->e1_off[2]   = 0.;
  music->e1_off[3]   = 0.;
  music->e1_off[4]   = 0.;
  music->e1_off[5]   = 0.;
  music->e1_off[6]   = 0.;
  music->e1_off[7]   = 0.;

  music->e1_gain[0]   = 1.; // MUSIC41 gains
  music->e1_gain[1]   = 1.; 
  music->e1_gain[2]   = 1.;
  music->e1_gain[3]   = 1.;
  music->e1_gain[4]   = 1.;
  music->e1_gain[5]   = 1.;
  music->e1_gain[6]   = 1.;
  music->e1_gain[7]   = 1.;

  //MUSIC42
  music->e2_off[0]   = 0.; //MUSIC42 offsets
  music->e2_off[1]   = 0.; 
  music->e2_off[2]   = 0.;
  music->e2_off[3]   = 0.;
  music->e2_off[4]   = 0.;
  music->e2_off[5]   = 0.;
  music->e2_off[6]   = 0.;
  music->e2_off[7]   = 0.;

  music->e2_gain[0]   = 1.; //MUSIC42 gains
  music->e2_gain[1]   = 1.; 
  music->e2_gain[2]   = 1.;
  music->e2_gain[3]   = 1.;
  music->e2_gain[4]   = 1.;
  music->e2_gain[5]   = 1.;
  music->e2_gain[6]   = 1.;
  music->e2_gain[7]   = 1.;

  //MUSIC43
  music->e3_off[0]   = 0.; //MUSIC3 offsets
  music->e3_off[1]   = 0.; 
  music->e3_off[2]   = 0.;
  music->e3_off[3]   = 0.;
  
  music->e3_gain[0]   = 1.; // MUSIC3 gains
  music->e3_gain[1]   = 1.; 
  music->e3_gain[2]   = 1.;
  music->e3_gain[3]   = 1.;

  music->pos_a1[0]   = 1.0;   // C0...Cn position correction not used
  music->pos_a1[1]   = 0.0;
  music->pos_a1[2]   = 0.0;
  music->pos_a1[3]   = 0.0;
  music->pos_a1[4]   = 0.0;
  music->pos_a1[5]   = 0.0;
  music->pos_a1[6]   = 0.0;


  //=========
  //  TPCs
  //=========
  
  //TPC 1 at S2 (TPC 21) in vaccuum  
  tpc->x_factor[0][0] = 0.070623;
  tpc->x_factor[0][1] = 0.07248;
  tpc->y_factor[0][0] =-0.035723;
  tpc->y_factor[0][1] =-0.034725;
  tpc->y_factor[0][2] =-0.0364399;
  tpc->y_factor[0][3] =-0.035037; 
	 
  tpc->x_offset[0][0] =-0.5;
  tpc->x_offset[0][1] =-1.058;
  tpc->y_offset[0][0] = 38.1838;
  tpc->y_offset[0][1] = 38.37;
  tpc->y_offset[0][2] = 39.1557;
  tpc->y_offset[0][3] = 39.097;
  

  //TPC 2 at S2 (TPC 22) in vaccuum
  tpc->x_factor[1][0] = 0.0716;
  tpc->x_factor[1][1] = 0.070329;
  tpc->y_factor[1][0] = -0.0362752;
  tpc->y_factor[1][1] = -0.0365182;
  tpc->y_factor[1][2] = -0.0357276;
  tpc->y_factor[1][3] = -0.0360721;  

  tpc->x_offset[1][0] = -0.81;
  tpc->x_offset[1][1] =  1.9;
  tpc->y_offset[1][0] =  36.77;
  tpc->y_offset[1][1] =  38.0;
  tpc->y_offset[1][2] =  36.123;
  tpc->y_offset[1][3] =  37.13;
  

  //TPC 3 at S2 (TPC 23) in air 
  tpc->x_factor[2][0] = 0.070128;  //L-R time 0
  tpc->x_factor[2][1] = 0.071489;  //L-R time 1
  tpc->y_factor[2][0] = -0.036481; //drift time 0
  tpc->y_factor[2][1] = -0.036863; //drift time 1
  tpc->y_factor[2][2] = -0.037120; //drift time 2
  tpc->y_factor[2][3] = -0.037125; //drift time 3

  //tpc->x_offset[2][0] = 0.81+3.24-0.14; //21.10.2014
  //tpc->x_offset[2][1] = 0.03+3.24-0.14; //21.10.2014
  //tpc->y_offset[2][0] = 19.2;
  //tpc->y_offset[2][1] = 19.7;
  //tpc->y_offset[2][2] = 18.9;
  //tpc->y_offset[2][3] = 19.2;
  
  tpc->x_offset[2][0] = 0.81-4.9; //2016Jul9
  tpc->x_offset[2][1] = 0.03-4.9; //2016Jul9 
  tpc->y_offset[2][0] = 19.2+49.8;//2016Jul9  
  tpc->y_offset[2][1] = 19.7+49.8;//2016Jul9  
  tpc->y_offset[2][2] = 18.9+49.8;//2016Jul9  
  tpc->y_offset[2][3] = 19.2+49.8;//2016Jul9
  

  //TPC 4 at S2 (TPC 24) in air
  tpc->x_factor[3][0] = 0.068179;
  tpc->x_factor[3][1] = 0.066689;
  tpc->y_factor[3][0] = -0.0412;
  tpc->y_factor[3][1] = -0.0411;
  tpc->y_factor[3][2] = -0.0404;
  tpc->y_factor[3][3] = -0.0418;

  //tpc->x_offset[3][0] = -0.37+6.0-1.3; //21.10.2014
  //tpc->x_offset[3][1] = 2.3+6.0-1.3; //21.10.2014
  //tpc->y_offset[3][0] = 28.0;
  //tpc->y_offset[3][1] = 28.4;
  //tpc->y_offset[3][2] = 28.4;
  //tpc->y_offset[3][3] = 27.8;

  tpc->x_offset[3][0] = -0.37+6.0-1.3 -4.8;//2016Jul9
  tpc->x_offset[3][1] = 2.3+6.0-1.3-4.8;//2016Jul9
  tpc->y_offset[3][0] = 28.0+48.9;//2016Jul9
  tpc->y_offset[3][1] = 28.4+48.9;//2016Jul9
  tpc->y_offset[3][2] = 28.4+48.9;//2016Jul9
  tpc->y_offset[3][3] = 27.8+48.9; //2016Jul9
  

  //TPC 5  at S4 (TPC 41) in air
  tpc->x_factor[4][0] = (0.069442);
  tpc->x_factor[4][1] = (0.070509);
  tpc->y_factor[4][0] = (-0.045657);
  tpc->y_factor[4][1] = (-0.044335);
  tpc->y_factor[4][2] = (-0.043486);
  tpc->y_factor[4][3] = (-0.042131);

  tpc->x_offset[4][0] = (-0.556613);
  tpc->x_offset[4][1] = (-4.097965);
  tpc->y_offset[4][0] = (78.432506);
  tpc->y_offset[4][1] = (76.420553);
  tpc->y_offset[4][2] = (75.495579);
  tpc->y_offset[4][3] = (73.756001);

  
  //tpc->x_factor[4][0] = 0.0701334;
  //tpc->x_factor[4][1] = 0.0719196;
  //tpc->y_factor[4][0] =-0.03644;
  //tpc->y_factor[4][1] =-0.03624;
  //tpc->y_factor[4][2] =-0.03709;
  //tpc->y_factor[4][3] =-0.03729;

  //tpc->x_offset[4][0] = 0.86+5.4-4.1; //21.10.2014
  //tpc->x_offset[4][1] =-2.35+5.4-4.1; //21.10.2014
  //tpc->y_offset[4][0] = 30.6;
  //tpc->y_offset[4][1] = 30.9;
  //tpc->y_offset[4][2] = 32.2;
  //tpc->y_offset[4][3] = 32.1;
 

  //TPC 6 at S4 (TPC 42) in air
  tpc->x_factor[5][0] = (0.069366);
  tpc->x_factor[5][1] = (0.069374);
  tpc->y_factor[5][0] = (-0.038537);
  tpc->y_factor[5][1] = (-0.038826);
  tpc->y_factor[5][2] = (-0.038093);
  tpc->y_factor[5][3] = (-0.037369);

  tpc->x_offset[5][0] = (-0.067895);
  tpc->x_offset[5][1] = (0.789764);
  tpc->y_offset[5][0] = (53.681955);
  tpc->y_offset[5][1] = (50.091274);
  tpc->y_offset[5][2] = (51.295231);
  tpc->y_offset[5][3] = (50.501582);
  
  
  //tpc->x_factor[5][0] = 0.071256;
  //tpc->x_factor[5][1] = 0.072858;
  //tpc->y_factor[5][0] =-0.04215;
  //tpc->y_factor[5][1] =-0.04161;
  //tpc->y_factor[5][2] =-0.04100;
  //tpc->y_factor[5][3] =-0.04025;

  //tpc->x_offset[5][0] = -0.4-0.3+0.35; //21.10.2014
  //tpc->x_offset[5][1] = 1.1-0.3+0.35; //21.10.2014
  //tpc->y_offset[5][0] = 22.6;
  //tpc->y_offset[5][1] = 21.9;
  //tpc->y_offset[5][2] = 20.7;
  //tpc->y_offset[5][3] = 20.4;
  

  //TPC at S3 (TPC 31)
  tpc->x_factor[6][0] = 1.;
  tpc->x_factor[6][1] = 1.;
  tpc->y_factor[6][0] = 1.;
  tpc->y_factor[6][1] = 1.;
  tpc->y_factor[6][2] = 1.;
  tpc->y_factor[6][3] = 1.;

  tpc->x_offset[6][0] = 0.;
  tpc->x_offset[6][1] = 0.;
  tpc->y_offset[6][0] = 0.;
  tpc->y_offset[6][1] = 0.;
  tpc->y_offset[6][2] = 0.;
  tpc->y_offset[6][3] = 0.;

  //===========
  // Plastics
  //===========

  //index 2 for Sc21  
  //sci->x_a[0][2] =  -2065.2-27+55;  //  SC21 calibration ch->mm 27.05.2016
  //sci->x_a[1][2] =  0.73; //  27.05.2016 
  sci->x_a[0][2] =  -1.0*(-2065.2-27.+55.-29.);  // YT 2016/Jun/14, See Logbook p212-214
  sci->x_a[1][2] =  -1.0*(0.73); //  YT 2016/Jun/14, See Logbook p212-214         
  sci->x_a[2][2] =  0.000000;  // 
  sci->x_a[3][2] =  0.000000;  //                            
  sci->x_a[4][2] =  0.000000;  //                             
  sci->x_a[5][2] =  0.000000;  //                             
  sci->x_a[6][2] =  0.000000;  //  
  
  /*
  // index 2 for Sc21  
  sci->x_a[0][2] = 1184.51;  //  SC21 calibration ch->mm 
  sci->x_a[1][2] =  -0.5206; //  s323 test run: Xe fragments run
  sci->x_a[2][2] =  0.000000;  // 
  sci->x_a[3][2] =  0.000000;  //                            
  sci->x_a[4][2] =  0.000000;  //                             
  sci->x_a[5][2] =  0.000000;  //                             
  sci->x_a[6][2] =  0.000000;  //    
  */

  // index 5 for Sc41
  sci->x_a[0][5] = -5.84328*330.;  //  SC41 calibration ch->mm 
  sci->x_a[1][5] = 0.00558294;  //
  sci->x_a[2][5] = 0.000000;   //
  sci->x_a[3][5] = 0.000000;   //                            
  sci->x_a[4][5] = 0.000000;   //                             
  sci->x_a[5][5] = 0.000000;   //                             
  sci->x_a[6][5] = 0.000000;   //    

  // index 6 for Sc42
  sci->x_a[0][6] = 0.; //   SC42 calibration ch->mm 
  sci->x_a[1][6] = 1.; // 
  sci->x_a[2][6] = 0.000000;  // 
  sci->x_a[3][6] = 0.000000;  //                            
  sci->x_a[4][6] = 0.000000;  //                             
  sci->x_a[5][6] = 0.000000;  //                             
  sci->x_a[6][6] = 0.000000;  //

   // index 7 for Sc43
  sci->x_a[0][7] = 0.; //   SC43 calibration ch->mm 
  sci->x_a[1][7] = 1.; // 
  sci->x_a[2][7] = 0.000000;  // 
  sci->x_a[3][7] = 0.000000;  //                            
  sci->x_a[4][7] = 0.000000;  //                             
  sci->x_a[5][7] = 0.000000;  //                             
  sci->x_a[6][7] = 0.000000;  //

   // index 10 for Sc81
  sci->x_a[0][10] = 0.; //   SC81 calibration ch->mm 
  sci->x_a[1][10] = 1.; // 
  sci->x_a[2][10] = 0.000000;  // 
  sci->x_a[3][10] = 0.000000;  //                            
  sci->x_a[4][10] = 0.000000;  //                             
  sci->x_a[5][10] = 0.000000;  //                             
  sci->x_a[6][10] = 0.000000;  // 


  // TOF
  /*
  sci->tac_off[0] = 12027.1;  //SC21L-R  
  sci->tac_off[1] = 5843.28;  //SC41L-R 
  sci->tac_off[2] = 7718.14;  //SC41L-SC21L
  sci->tac_off[3] = 8452.87;  //SC41R-SC21R
  sci->tac_off[4] = 0.;  //SC42L-R
  sci->tac_off[5] = 0.;  //SC42L-SC21L
  sci->tac_off[6] = 0.;  //SC42R-SC21R
  sci->tac_off[7] = 0.;  //SC43L-R
  sci->tac_off[8] = 0.;  //SC81L-R
  sci->tac_off[9] = 0.;  //SC81L-SC21L
  sci->tac_off[10] = 0.;  //SC81R-SC21R
  */

  sci->tac_off[0] = 2327.95;  //SC21L-R  
  sci->tac_off[1] = 1447.37;  //SC41L-R 
  sci->tac_off[2] = 1833.28;  //SC41L-SC21L
  sci->tac_off[3] = 1972.59;  //SC41R-SC21R
  sci->tac_off[4] = 2073.4;  //SC42L-R
  sci->tac_off[5] = 1542.25;  //SC42L-SC21L
  sci->tac_off[6] = 1729.58;  //SC42R-SC21R
  sci->tac_off[7] = 1441.13;  //SC43L-R
  sci->tac_off[8] = 1777.77;  //SC81L-R
  sci->tac_off[9] = 3020.83;  //SC81L-SC21L
  sci->tac_off[10] = 1740.21;  //SC81R-SC21R



  // TAC calibration factors assumed for agata
  //   sci->tac_factor[0] = 5.571;   //SC21L-R [ps/ch]  , range 25 ns
  //   sci->tac_factor[1] = 5.604;   //SC41L-R [ps/ch]  , range 25 ns  
  // sci->tac_factor[0] = 5.89623;   //SC21L-R [ps/ch]  , range 25 ns, CH 19.10.2014
  // sci->tac_factor[1] = 5.51268;   //SC41L-R [ps/ch]  , range 25 ns, CH 19.10.2014
  // sci->tac_factor[4] = 5.609;      //SC42L-R [ps/ch]  , range 25 ns

  // //   sci->tac_factor[2] = 16.809;  // SC41L-SC21L [ps/ch] , range 75 ns
  // //   sci->tac_factor[3] = 17.149;  // SC41R-SC21R [ps/ch] , range 75 ns 
  // sci->tac_factor[2] = 10.55619;  // SC41L-SC21L [ps/ch] , range 50 ns, CH 19.10.2014
  // sci->tac_factor[3] = 11.17427;  // SC41R-SC21R [ps/ch] , range 50 ns, CH 19.10.2014 

  /*
  sci->tac_factor[0] = 10.4297;   //SC21L-R [ps/ch], range 10 ns 
  sci->tac_factor[1] = 5.58294;   //SC41L-R [ps/ch], range 10 ns
  sci->tac_factor[2] = 20.8056;   //SC41L-SC21L [ps/ch], range 10 ns
  sci->tac_factor[3] = 21.806;    //SC41R-SC21R [ps/ch], range 10 ns
  sci->tac_factor[4] = 1.;        //SC42L-R
  sci->tac_factor[5] = 1.;        //SC42L-SC21L
  sci->tac_factor[6] = 1.;        //SC42R-SC21R
  sci->tac_factor[7] = 1.;        //SC43L-R
  sci->tac_factor[8] = 1.;        //SC81L-R
  sci->tac_factor[9] = 1.;        //SC81L-SC21L
  sci->tac_factor[10] = 1.;        //SC81R-SC21R
  */

  sci->tac_factor[0] = 20.6895;   //SC21L-R [ps/ch], range 10 ns 
  sci->tac_factor[1] = 26.3158;   //SC41L-R [ps/ch], range 10 ns
  sci->tac_factor[2] = 22.0238;   //SC41L-SC21L [ps/ch], range 10 ns
  sci->tac_factor[3] = 21.6549;    //SC41R-SC21R [ps/ch], range 10 ns
  sci->tac_factor[4] = 20.8798;        //SC42L-R
  sci->tac_factor[5] = 21.3739;        //SC42L-SC21L
  sci->tac_factor[6] = 21.0525;        //SC42R-SC21R
  sci->tac_factor[7] = 21.8067;        //SC43L-R
  sci->tac_factor[8] = 20.911;        //SC81L-R
  sci->tac_factor[9] = 20.8333;        //SC81L-SC21L
  sci->tac_factor[10] = 20.1292;        //SC81R-SC21R

  sci->tof_bll2  = 1.;    // not used online [ps/ch]
  sci->tof_brr2  = 1.;    // not used online
  sci->tof_bll3  = 1.;    // not used online 
  sci->tof_brr3  = 1.;    // not used online
  sci->tof_bll4  = 1.;    // not used online 
  sci->tof_brr4  = 1.;    // not used online

  sci->tof_a2 = 0.; // [ps] offset   Tof S41-S21
  sci->tof_a3 = 0.; // [ps] offset   Tof S42-S21
  sci->tof_a4 = 0.; // [ps] offset   Tof S81-S21


  //   id->id_tofoff2  = 197689.0; // [ps]  S41-S21 from 9.Oct on because of TAC-LL shift
  //   id->id_path2    = 119120.0;

  // used for ToF S2-S4, extracted from agata calib
  //id->id_tofoff2  = 175940.0; // [ps]  S411 21.10.2014
  //id->id_path2    = 126160.0; // path/c [ps]  S411_48 21.10.2014 
  //id->id_tofoff2  = 170680.0; // [ps]  S411 22.10.2014
  //id->id_tofoff2  = 170470.0; // [ps]  S411 shifted 25.10.2014
  //id->id_path2    = 121730.0; // path/c [ps]  S411_48 22.10.2014 

  //  id->id_tofoff2  = 202971  ; // [ps]  S411_86 fit with only one point shifted 25
  //  id->id_path2    = 125329.2; // path/c [ps]  S411 Uranium data -8.6cm

  id->id_tofoff2  = 200120.  ; // [ps]  S411_149 with 12C
  id->id_path2    = 120450. ; // path/c [ps]  old from Xe in June 2016

  id->id_tofoff3  = 210000.;   // [ps] //check
  id->id_path3    = 120000.;   // path/c [ps] //check

  id->id_tofoff4  = 210000.;   // [ps]  //check 
  id->id_path4    = 120000.;   // path/c [ps]  //check

  id->ID_Z_AoverQ_num[0]=5; 
  id->ID_Z_AoverQ_num[1]=5; 
  id->ID_Z_AoverQ_num[2]=5; 
  id->ID_Z_AoverQ_num[3]=5; 
  id->ID_Z_AoverQ_num[4]=5; 

  id->ID_Z_AoverQ[0][0][0]=2.208      ; id->ID_Z_AoverQ[0][0][1]=1180;
  id->ID_Z_AoverQ[0][1][0]=2.208      ; id->ID_Z_AoverQ[0][1][1]=0;
  id->ID_Z_AoverQ[0][2][0]=2.220      ; id->ID_Z_AoverQ[0][2][1]=0;
  id->ID_Z_AoverQ[0][3][0]=2.220      ; id->ID_Z_AoverQ[0][3][1]=1180;
  id->ID_Z_AoverQ[0][4][0]=2.208      ; id->ID_Z_AoverQ[0][4][1]=1180;

  id->ID_Z_AoverQ[1][0][0]=2.208+0.012; id->ID_Z_AoverQ[1][0][1]=1180; 
  id->ID_Z_AoverQ[1][1][0]=2.208+0.012; id->ID_Z_AoverQ[1][1][1]=0; 
  id->ID_Z_AoverQ[1][2][0]=2.220+0.012; id->ID_Z_AoverQ[1][2][1]=0; 
  id->ID_Z_AoverQ[1][3][0]=2.220+0.012; id->ID_Z_AoverQ[1][3][1]=1180; 
  id->ID_Z_AoverQ[1][4][0]=2.208+0.012; id->ID_Z_AoverQ[1][4][1]=1180; 

  id->ID_Z_AoverQ[2][0][0]=2.208+0.024; id->ID_Z_AoverQ[2][0][1]=1180; 
  id->ID_Z_AoverQ[2][1][0]=2.208+0.024; id->ID_Z_AoverQ[2][1][1]=60; 
  id->ID_Z_AoverQ[2][2][0]=2.220+0.024; id->ID_Z_AoverQ[2][2][1]=60; 
  id->ID_Z_AoverQ[2][3][0]=2.220+0.024; id->ID_Z_AoverQ[2][3][1]=1180; 
  id->ID_Z_AoverQ[2][4][0]=2.208+0.024; id->ID_Z_AoverQ[2][4][1]=1180; 

  id->ID_Z_AoverQ[3][0][0]=2.208+0.036; id->ID_Z_AoverQ[3][0][1]=1180; 
  id->ID_Z_AoverQ[3][1][0]=2.208+0.036; id->ID_Z_AoverQ[3][1][1]=60; 
  id->ID_Z_AoverQ[3][2][0]=2.220+0.036; id->ID_Z_AoverQ[3][2][1]=60; 
  id->ID_Z_AoverQ[3][3][0]=2.220+0.036; id->ID_Z_AoverQ[3][3][1]=1180; 
  id->ID_Z_AoverQ[3][4][0]=2.208+0.036; id->ID_Z_AoverQ[3][4][1]=1180; 

  id->ID_Z_AoverQ[4][0][0]=2.208+0.048; id->ID_Z_AoverQ[4][0][1]=1180; 
  id->ID_Z_AoverQ[4][1][0]=2.208+0.048; id->ID_Z_AoverQ[4][1][1]=60; 
  id->ID_Z_AoverQ[4][2][0]=2.220+0.048; id->ID_Z_AoverQ[4][2][1]=60; 
  id->ID_Z_AoverQ[4][3][0]=2.220+0.048; id->ID_Z_AoverQ[4][3][1]=1180; 
  id->ID_Z_AoverQ[4][4][0]=2.208+0.048; id->ID_Z_AoverQ[4][4][1]=1180; 


  id->ID_x2AoverQ_num[0]=5; 
  id->ID_x2AoverQ_num[1]=5; 
  id->ID_x2AoverQ_num[2]=5; 
  id->ID_x2AoverQ_num[3]=5; 
  id->ID_x2AoverQ_num[4]=5; 
  id->ID_x2AoverQ_num[5]=5; 

// <<<<<<< Updated upstream
//   id->ID_x2AoverQ[0][0][0]=2.208      ; id->ID_x2AoverQ[0][0][1]=-100;
//   id->ID_x2AoverQ[0][1][0]=2.208      ; id->ID_x2AoverQ[0][1][1]=100;
//   id->ID_x2AoverQ[0][2][0]=2.220      ; id->ID_x2AoverQ[0][2][1]=100;
//   id->ID_x2AoverQ[0][3][0]=2.220      ; id->ID_x2AoverQ[0][3][1]=-100;
//   id->ID_x2AoverQ[0][4][0]=2.208      ; id->ID_x2AoverQ[0][4][1]=-100;
                                      
//   id->ID_x2AoverQ[1][0][0]=2.208+0.012; id->ID_x2AoverQ[1][0][1]=-100; 
//   id->ID_x2AoverQ[1][1][0]=2.208+0.012; id->ID_x2AoverQ[1][1][1]=100; 
//   id->ID_x2AoverQ[1][2][0]=2.220+0.012; id->ID_x2AoverQ[1][2][1]=100; 
//   id->ID_x2AoverQ[1][3][0]=2.220+0.012; id->ID_x2AoverQ[1][3][1]=-100; 
//   id->ID_x2AoverQ[1][4][0]=2.208+0.012; id->ID_x2AoverQ[1][4][1]=-100; 
                                      
//   id->ID_x2AoverQ[2][0][0]=2.208+0.024; id->ID_x2AoverQ[2][0][1]=-100; 
//   id->ID_x2AoverQ[2][1][0]=2.208+0.024; id->ID_x2AoverQ[2][1][1]=100; 
//   id->ID_x2AoverQ[2][2][0]=2.220+0.024; id->ID_x2AoverQ[2][2][1]=100; 
//   id->ID_x2AoverQ[2][3][0]=2.220+0.024; id->ID_x2AoverQ[2][3][1]=-100; 
//   id->ID_x2AoverQ[2][4][0]=2.208+0.024; id->ID_x2AoverQ[2][4][1]=-100; 
                                      
//   id->ID_x2AoverQ[3][0][0]=2.208+0.036; id->ID_x2AoverQ[3][0][1]=-100; 
//   id->ID_x2AoverQ[3][1][0]=2.208+0.036; id->ID_x2AoverQ[3][1][1]=100; 
//   id->ID_x2AoverQ[3][2][0]=2.220+0.036; id->ID_x2AoverQ[3][2][1]=100; 
//   id->ID_x2AoverQ[3][3][0]=2.220+0.036; id->ID_x2AoverQ[3][3][1]=-100; 
//   id->ID_x2AoverQ[3][4][0]=2.208+0.036; id->ID_x2AoverQ[3][4][1]=-100; 
                                      
//   id->ID_x2AoverQ[4][0][0]=2.208+0.048; id->ID_x2AoverQ[4][0][1]=-100; 
//   id->ID_x2AoverQ[4][1][0]=2.208+0.048; id->ID_x2AoverQ[4][1][1]=100; 
//   id->ID_x2AoverQ[4][2][0]=2.220+0.048; id->ID_x2AoverQ[4][2][1]=100; 
//   id->ID_x2AoverQ[4][3][0]=2.220+0.048; id->ID_x2AoverQ[4][3][1]=-100; 
//   id->ID_x2AoverQ[4][4][0]=2.208+0.048; id->ID_x2AoverQ[4][4][1]=-100; 
// =======


 id->ID_x2AoverQ[0][0][0]=2.24433; id->ID_x2AoverQ[0][0][1]=42.5864;
 id->ID_x2AoverQ[0][1][0]=2.17429; id->ID_x2AoverQ[0][1][1]=-68.2431;
 id->ID_x2AoverQ[0][2][0]=2.18351; id->ID_x2AoverQ[0][2][1]=-70.9073;
 id->ID_x2AoverQ[0][3][0]=2.2573; id->ID_x2AoverQ[0][3][1]=40.9879;
 id->ID_x2AoverQ[0][4][0]=2.24433;id->ID_x2AoverQ[0][4][1]=42.5864;
 
  id->ID_x2AoverQ[1][0][0]=2.27782; id->ID_x2AoverQ[1][0][1]=58.1797; 
  id->ID_x2AoverQ[1][1][0]=2.18477; id->ID_x2AoverQ[1][1][1]=-66.2442; 
  id->ID_x2AoverQ[1][2][0]=2.19417; id->ID_x2AoverQ[1][2][1]=-69.7005; 
  id->ID_x2AoverQ[1][3][0]=2.29192; id->ID_x2AoverQ[1][3][1]=57.0277; 
  id->ID_x2AoverQ[1][4][0]=2.27782; id->ID_x2AoverQ[1][4][1]=58.1797; 

  id->ID_x2AoverQ[2][0][0]=2.28597; id->ID_x2AoverQ[2][0][1]=40.8986; 
  id->ID_x2AoverQ[2][1][0]=2.19548; id->ID_x2AoverQ[2][1][1]=-69.1244; 
  id->ID_x2AoverQ[2][2][0]=2.21129; id->ID_x2AoverQ[2][2][1]=-69.1244; 
  id->ID_x2AoverQ[2][3][0]=2.30014; id->ID_x2AoverQ[2][3][1]=40.3226; 
  id->ID_x2AoverQ[2][4][0]=2.28597; id->ID_x2AoverQ[2][4][1]=40.8986; 

  id->ID_x2AoverQ[3][0][0]=2.30468; id->ID_x2AoverQ[3][0][1]=41.0484; 
  id->ID_x2AoverQ[3][1][0]=2.20712; id->ID_x2AoverQ[3][1][1]=-73.4407; 
  id->ID_x2AoverQ[3][2][0]=2.22237; id->ID_x2AoverQ[3][2][1]=-73.1567; 
  id->ID_x2AoverQ[3][3][0]=2.32009; id->ID_x2AoverQ[3][3][1]=41.0484; 
  id->ID_x2AoverQ[3][4][0]=2.30468; id->ID_x2AoverQ[3][4][1]=41.0484; 
  
  id->ID_x2AoverQ[4][0][0]=2.32064; id->ID_x2AoverQ[4][0][1]=39.6964; 
  id->ID_x2AoverQ[4][1][0]=2.22332; id->ID_x2AoverQ[4][1][1]=-73.875; 
  id->ID_x2AoverQ[4][2][0]=2.23886; id->ID_x2AoverQ[4][2][1]=-73.517; 
  id->ID_x2AoverQ[4][3][0]=2.33663; id->ID_x2AoverQ[4][3][1]=39.6964; 
  id->ID_x2AoverQ[4][4][0]=2.32064; id->ID_x2AoverQ[4][4][1]=39.6964; 
  
  id->ID_x2AoverQ[5][0][0]=2.26732; id->ID_x2AoverQ[5][0][1]=84.9736; 
  id->ID_x2AoverQ[5][1][0]=2.35488; id->ID_x2AoverQ[5][1][1]=78.52  ; 
  id->ID_x2AoverQ[5][2][0]=2.34538; id->ID_x2AoverQ[5][2][1]=75.078 ; 
  id->ID_x2AoverQ[5][3][0]=2.24458; id->ID_x2AoverQ[5][3][1]=82.0336; 
  id->ID_x2AoverQ[5][4][0]=2.26732; id->ID_x2AoverQ[5][4][1]=84.9736; 

  // infinite gating

  // Change the magnetic field here                                                                                   
  frs->bfield[0] = 0.74845;      // FRS D3 field [Tesla]
  frs->bfield[1] = 0.73574;      // FRS D4 field [Tesla]  
  frs->bfield[2] = 0.7178;       // FRS D3 field [Tesla]
  frs->bfield[3] = 0.73174;      // FRS D4 field [Tesla] 

  // from 12C
  frs->bfield[0] = 0.7340;      // FRS D3 field [Tesla] 
  frs->bfield[1] = 0.72174;      // FRS D4 field [Tesla] 
  frs->bfield[2] = 0.7071;       // FRS D3 field [Tesla] 
  frs->bfield[3] = 0.7186;      // FRS D4 field [Tesla]

  frs->dispersion[0] = -6.474266; // RUN81-TA2B-220CM 27.05.2016
  frs->dispersion[1] =  7.7064; // RUN81-TA2B-220CM 27.05.2016
  frs->magnification[1] = 1.1903;   // RUN81-TA2B-220CM 27.05.2016


  // 13.06.2016 S411_86 with 124Xe
  //  frs->rho0[0]   = 11.1340;    // FRS (D2) radius [m]   27.05.2016 
  //  frs->rho0[1]   = 11.287;   // FRS (D3+D4)/2 radius [m]  27.05.2016 
  
  // 14.06.2016 adjusted for S411_87 with 124Xe+thick degrader  
  //frs->rho0[0]   = 11.1340;    // FRS (D2) radius [m]   
  //frs->rho0[1]   = 11.2415;   // FRS (D3+D4)/2 radius [m] 

  //26.06.2016 new effective radius
  //frs->rho0[0]   = 11.1349;    // FRS (D2) radius [m]   
  //frs->rho0[1]   = 11.377;   // FRS (D3+D4)/2 radius [m] 

  //05.07.2016 new effective radius S411_149 12C
  frs->rho0[0]   = 11.1374;    // FRS (D1+D2)/2 radius [m]   
  frs->rho0[1]   = 11.3516;    // FRS (D3+D4)/2 radius [m]


  //  frs->rho0[0]   = 11.22374;    // FRS (D2) radius [m]    
  //  frs->rho0[1]   = 11.282095;   // FRS (D3+D4)/2 radius [m]   

  //   frs->rho0[0]   = 11.203 ;   // FRS (D1+D2)/2 radius [m]    
  //   frs->rho0[1]   = 11.2641 ;   // FRS (D3+D4)/2 radius [m]   16.09.11


  frs->primary_z = 12.*0.32;  // C beam adjusted for S411_150 (B,Be,Li) 
  frs->offset_z  =  0.85 ;
  frs->offset_z2  = 3.0 ;
  frs->offset_z3  = 0.0 ;

  frs->a2AoQCorr = 0.00095;
  frs->a4AoQCorr = 0.0000;//0.0015

	
  // MUSIC 1 velocity correction 
  //id->vel_a[0] =  11612 ;  // S411 oct2014 first days
  id->vel_a[0] =  2750;//11673 *1.096   // shifted calib 25.10.2104  
  id->vel_a[1] =  -2200; // -17462*1.096 
  id->vel_a[2] =   0.0; //7682 *1.096
  id->vel_a[3] =   0.0; 

  // MUSIC 2 velocity correction 
  //id->vel_a2[0] =  19577; // S411 oct2014 first days
  id->vel_a2[0] =  19602 ;  // shifted calib 25.10.2104
  id->vel_a2[1] =  -34442;
  id->vel_a2[2] =  17273;
  id->vel_a2[3] =  0.0; 
	
  // MUSIC 3 velocity correction
  id->vel_a3[0] =  13951.37; //
  id->vel_a3[1] =  -38369.9; 
  id->vel_a3[2] =  28396.46;
  id->vel_a3[3] =  0.0;    
  
//  id->vel_a3[0] =  0.0; //
//  id->vel_a3[1] =  1.0; 
//  id->vel_a3[2] =  0.0;
//  id->vel_a3[3] =  0.0;  

  // gate on Z 
  id->zgate_low = 85;
  id->zgate_high = 90;

  //=======
  //  Si
  //======= 
   
  si->si_factor1=5.82775; //CH 03/06/2016
  si->si_offset1=-381.593; //CH 03/06/2016

  si->si_factor2=3.809; //CH 18.10.2014
  si->si_offset2=-529.01; //CH 18.10.2014

  si->si_factor3=3.2596; //CH 21.05.2016
  si->si_offset3=-550.59; //CH 21.05.2016|

  si->si_factor4=3.2596; //CH 21.05.2016
  si->si_offset4=-550.59; //CH 21.05.2016

  si->dssd_factor[0] = 1.;
  si->dssd_factor[1] = 1.;
  si->dssd_factor[2] = 1.;
  si->dssd_factor[3] = 1.;
  si->dssd_factor[4] = 1.;
  si->dssd_factor[5] = 1.;
  si->dssd_factor[6] = 1.;
  si->dssd_factor[7] = 1.;
  si->dssd_factor[8] = 1.;
  si->dssd_factor[9] = 1.;
  si->dssd_factor[10] = 1.;
  si->dssd_factor[11] = 1.;
  si->dssd_factor[12] = 1.;
  si->dssd_factor[13] = 1.;
  si->dssd_factor[14] = 1.;
  si->dssd_factor[15] = 1.;
  si->dssd_factor[16] = 1.;
  si->dssd_factor[17] = 1.;
  si->dssd_factor[18] = 1.;
  si->dssd_factor[19] = 1.;
  si->dssd_factor[20] = 1.;
  si->dssd_factor[21] = 1.;
  si->dssd_factor[22] = 1.;
  si->dssd_factor[23] = 1.;
  si->dssd_factor[24] = 1.;
  si->dssd_factor[25] = 1.;
  si->dssd_factor[26] = 1.;
  si->dssd_factor[27] = 1.;
  si->dssd_factor[28] = 1.;
  si->dssd_factor[29] = 1.;
  si->dssd_factor[30] = 1.;
  si->dssd_factor[31] = 1.;

  si->dssd_offset[0] = 0.;
  si->dssd_offset[1] = 0.;
  si->dssd_offset[2] = 0.;
  si->dssd_offset[3] = 0.;
  si->dssd_offset[4] = 0.;
  si->dssd_offset[5] = 0.;
  si->dssd_offset[6] = 0.;
  si->dssd_offset[7] = 0.;
  si->dssd_offset[8] = 0.;
  si->dssd_offset[9] = 0.;
  si->dssd_offset[10] = 0.;
  si->dssd_offset[11] = 0.;
  si->dssd_offset[12] = 0.;
  si->dssd_offset[13] = 0.;
  si->dssd_offset[14] = 0.;
  si->dssd_offset[15] = 0.;
  si->dssd_offset[16] = 0.;
  si->dssd_offset[17] = 0.;
  si->dssd_offset[18] = 0.;
  si->dssd_offset[19] = 0.;
  si->dssd_offset[20] = 0.;
  si->dssd_offset[21] = 0.;
  si->dssd_offset[22] = 0.;
  si->dssd_offset[23] = 0.;
  si->dssd_offset[24] = 0.;
  si->dssd_offset[25] = 0.;
  si->dssd_offset[26] = 0.;
  si->dssd_offset[27] = 0.;
  si->dssd_offset[28] = 0.;
  si->dssd_offset[29] = 0.;
  si->dssd_offset[30] = 0.;
  si->dssd_offset[31] = 0.;
  
  //=========
  //MR-TOF-MS
  //=========
  
  mrtof->MRTOFMS_a=0.069;
  mrtof->MRTOFMS_b=0.71;
  mrtof->MRTOFMS_t0=0;
  mrtof->MRTOFMS_TXRS=0;

  cout << "Focus distance S4: " << frs->dist_focS4 << endl;


  ElecMod->Nb_Modules = 64;
  ElecMod->Nb_QDC = 3;
  ElecMod->Nb_ADC = 1;
  ElecMod->Nb_TDC = 5;
  ElecMod->Nb_Scaler = 2;
  ElecMod->Nb_TimeStamp = 2;
  
  for(int i = 0;i<64;++i)
    ElecMod->Nb_Channels.insert(std::pair<int,int>(i,32));

  
  // Crate FRS procID 10
  int IdMod = 0;
  //std::unordered_map<int,int> CrateFRS;
  std::map<int,int> CrateFRS;
  CrateFRS.insert(std::pair<int,int>(1,IdMod++));
  CrateFRS.insert(std::pair<int,int>(9,IdMod++));
  CrateFRS.insert(std::pair<int,int>(8,IdMod++));
  CrateFRS.insert(std::pair<int,int>(11,IdMod++));
  CrateFRS.insert(std::pair<int,int>(12,IdMod++));
  
  cout<<"CRATE FRS VALUE  "<<CrateFRS[1]<<endl;
  cout<<"CRATE FRS VALUE  "<<CrateFRS[9]<<endl;
  cout<<"CRATE FRS VALUE  "<<CrateFRS[8]<<endl;
  cout<<"CRATE FRS VALUE  "<<CrateFRS[11]<<endl;
  cout<<"CRATE FRS VALUE  "<<CrateFRS[12]<<endl;

  //std::unordered_map<int,int> CrateTPC;
  std::map<int,int> CrateTPC;
  //CrateTPC.insert(std::pair<int,int>(5,IdMod++));
  CrateTPC.insert(std::pair<int,int>(8,IdMod++));
  CrateTPC.insert(std::pair<int,int>(9,IdMod++));
  CrateTPC.insert(std::pair<int,int>(15,IdMod++));
  CrateTPC.insert(std::pair<int,int>(3,IdMod++));
  
  //std::unordered_map<int,int> CrateUser;
  std::map<int,int> CrateUser;
  CrateUser.insert(std::pair<int,int>(0,IdMod++));

  std::map<int,int> CrateMT;
  CrateMT.insert(std::pair<int,int>(0,IdMod++));
  CrateMT.insert(std::pair<int,int>(2,IdMod++));

  std::map<int,int> CrateSOFIA; //added                                                                                                                                 
  CrateSOFIA.insert(std::pair<int,int>(0,IdMod++));
  CrateSOFIA.insert(std::pair<int,int>(1,IdMod++));
  CrateSOFIA.insert(std::pair<int,int>(2,IdMod++));

  
  Map1* temp1 = new Map1("temp1");
  Map1* temp2 = new Map1("temp2");
  Map1* temp3 = new Map1("temp3");
  Map1* temp4 = new Map1("temp4");
  Map1* temp5 = new Map1("temp5");
  temp1->map=CrateFRS;
  temp2->map=CrateTPC;
  temp3->map=CrateUser;
  temp4->map=CrateMT;
  temp5->map=CrateSOFIA;
  
  cout<<"TEMP1 VALUE  "<<temp1->map[1]<<endl;
  cout<<"TEMP1 VALUE  "<<temp1->map[9]<<endl;
  cout<<"TEMP1 VALUE  "<<temp1->map[8]<<endl;
  cout<<"TEMP1 VALUE  "<<temp1->map[11]<<endl;
  cout<<"TEMP1 VALUE  "<<temp1->map[12]<<endl;

  
  TObjString* key1 = new TObjString("10");
  TObjString* key2 = new TObjString("20");
  TObjString* key3 = new TObjString("30");
  TObjString* key4 = new TObjString("40");
  TObjString* key5 = new TObjString("50");
  
  ElecMod->Maptemp.Add(key1,temp1);
  ElecMod->Maptemp.Add(key2,temp2);
  ElecMod->Maptemp.Add(key3,temp3);
  ElecMod->Maptemp.Add(key4,temp4);
  ElecMod->Maptemp.Add(key5,temp5);
  
  // ElecMod->Maptemp.insert(std::pair<int,map<int,int> >(10,CrateFRS));
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(20,CrateTPC));
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(30,CrateUser));
  
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(10,CrateFRS));
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(20,CrateTPC));
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(30,CrateUser));

  
  ElecMod->ModType.push_back("FRS_SC_1");
  ElecMod->ModType.push_back("FRS_TDC_1");
  ElecMod->ModType.push_back("FRS_TDC_2");
  ElecMod->ModType.push_back("FRS_QDC_1");
  ElecMod->ModType.push_back("FRS_ADC_1");
  ElecMod->ModType.push_back("TPC_TDC_1");
  ElecMod->ModType.push_back("TPC_TDC_2");
  ElecMod->ModType.push_back("TPC_QDC_1");
  ElecMod->ModType.push_back("TPC_QDC_2");
  ElecMod->ModType.push_back("USER_MTDC_1");
  ElecMod->ModType.push_back("MT_SC_1");
  ElecMod->ModType.push_back("MT_MTDC_1");
  ElecMod->ModType.push_back("SOFIA_VTX_1");
  ElecMod->ModType.push_back("SOFIA_MDP_1");
  ElecMod->ModType.push_back("SOFIA_MADC_1");


  ElecMod->Scaler32bit=1;
  //ElecMod->EventFlags.push_back(0x00000000);
  ElecMod->EventFlags.push_back(0x00000100);
  ElecMod->EventFlags.push_back(0x00000200);
  ElecMod->EventFlags.push_back(0x00000300);
  
  // ElecMod->ModType.push_back("QDC_11");
  // ElecMod->ModType.push_back("QDC_12");
  // ElecMod->ModType.push_back("QDC_13");
  // ElecMod->ModType.push_back("QDC_14");
  // ElecMod->ModType.push_back("QDC_15");
  // ElecMod->ModType.push_back("QDC_16");
  // ElecMod->ModType.push_back("QDC_17");
  // ElecMod->ModType.push_back("QDC_18");
  
  ElecMod->setMap();

  ElecMod->Print();
  
  cout << "Setup done " << endl;




}



void BS_Detector_System::get_Event_data(Raw_Event* RAW){
	
}

//---------------------------------------------------------------

void BS_Detector_System::Process_MBS(int* pdata){

}

int* BS_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

Float_t FRS_Detector_System::rand0_5(){
    
  return rand()*1./RAND_MAX;// - 0.5; 
    
}

//---------------------------------------------------------------

Int_t FRS_Detector_System::getbits(Int_t value, int nword, int start, int length){
    
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*16 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
  
}

//---------------------------------------------------------------

Int_t FRS_Detector_System::get2bits(Int_t value, int nword, int start, int length){
    
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*32 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
  
}

//---------------------------------------------------------------
