#include "FRS_Detector_System.h"

#include "TH1.h"

#include <cstdlib>
#include <iostream>
#include <bitset>

//#include "TFRSSortEvent.h"
//#include "TFRSCalibrEvent.h"
//#include "TFRSAnlEvent.h"
#include "TFRSParameter.h"

FRS_Detector_System::FRS_Detector_System(){



/*  // MUSIC PARAMETERS //
  
  dE     = new Float_t[3];
  dE_cor = new Float_t[3];

  
  for(int i=0; i<3; ++i){
      
     Float_t dE[i]     = 0;
     Float_t dE_cor[i] = 0;

  }
  
  // SCINTILLATOR PARAMETERS //
  
  
  sci_l  = new Float_t[12];
  sci_r  = new Float_t[12];
  sci_e  = new Float_t[12];
  sci_tx = new Float_t[12];
  sci_x  = new Float_t[12];
  
  
  for(int i=0; i<12; ++i){
   
  
      sci_l[i]  = 0;
      sci_r[i]  = 0;
      sci_e[i]  = 0;
      sci_tx[i] = 0;
      sci_x[i]  = 0;
      
  }
      
  
  sci_tofll2 = 0;
  sci_tofll3 = 0
  sci_tof2   = 0;
  sci_tofrr2 = 0;
  sci_tofrr3 = 0;
  sci_tof3   = 0;
  
  // ID PARAMETERS //
  
  ID_x2 = 0;
  ID_y2 = 0;
  ID_a2 = 0;
  ID_b2 = 0;
  
  ID_x4 = 0;
  ID_y4 = 0;
  ID_a4 = 0;
  ID_b4 = 0;
  
  brho = new Float_t[2];
  rho  = new Float_t[2];
  
  for(int i=0; i<2; ++i){

      ID_brho[i] = 0;
      ID_rho[i]  = 0;

  }
  
  beta      = 0;
  beta3     = 0;
  gamma     = 0;
  AoQ       = 0;
  AoQ_corr  = 0;
  
  z         = 0;
  z2        = 0;
  z3        = 0;
  
  timestamp = 0;
  ts        = 0;
  ts2       = 0;*/

}
FRS_Detector_System::~FRS_Detector_System(){

}



/*  TFRSSortEvent *srt = dynamic_cast<TFRSSortEvent*> (GetInputEvent("Calibr"));
  TFRSCalibrEvent *cal = dynamic_cast<TFRSCalibrEvent*> (GetInputEvent("Analysis"));
  TFRSAnlEvent *anl = dynamic_cast<TFRSAnlEvent*> (GetInputEvent());

  //if ((tgt==0) || (cal==0) || (srt==0) || (anl==0)) return;

  // event are flush to store tree only when a event from MRtof arrived meanwhile lastest FRS event is stored.
  //if(anl->EventFlag==Setup->EventFlags[0])
  //  {
  //    CopyPrevious(*anl);
      
  //    SetValid(kFALSE);  // FRS event not flush alone
  //  }
  //if(anl->EventFlag==Setup->EventFlags[1])
  // {
      Process_Analysis(*srt,*cal,*anl,*RAW);
  //    SetValid(kTRUE);  // MRtof + lastest FRS event to be flushed. 
  //  }
}*/

//---------------------------------------------------------------
void FRS_Detector_System::Process_FRS(TModParameter* Modsetup, TGo4MbsSubEvent* psubevt, TGo4MbsEvent* fInput){

  FRS_Unpack(Modsetup, psubevt, fInput); // Unpacks the data
  FRS_Sort(); // Sorts Data
  FRS_Calib(); // Calibrates Data
  FRS_Anal(); // Analyses Data

}

void FRS_Detector_System::FRS_Unpack(TModParameter* ModSetup, TGo4MbsSubEvent* psubevt, TGo4MbsEvent* fInput){
     
     
     analyse = true;
     
    /*ModSetup = dynamic_cast<TModParameter*>(GetParameter("ModPar"));

    if(ModSetup == nullptr)
    {
      std::cout<<"E> Electronic Module parameters not set properly, it is nullptr !"<<std::endl;
      std::exit(-1);
    }*/
    
    previousTimeStamp[0] = 0;
    previousTimeStamp[1] = 0;
    previousTimeStamp[2] = 0;
    currentTimeStamp = 0;
    

  /*  Put the event header stuff into output event  */
  
  qlength   = fInput->GetDlen()   ;
  qtype     = fInput->GetType()   ;
  qsubtype  = fInput->GetSubtype();
  qdummy    = fInput->GetDummy()  ;
  qtrigger  = fInput->GetTrigger();
  qevent_nr = fInput->GetCount()  ;



  //if( (psubevt->GetProcid()!=10) && (psubevt->GetProcid()!=15) && (psubevt->GetProcid()!=20) && (psubevt->GetProcid()!=30) && (psubevt->GetProcid()!=40) && (psubevt->GetProcid()!=50))

    
	  /*  Check the SUBevent type and subtype!             */
	
	  if( !( (psubevt->GetType() == 12) && (psubevt->GetSubtype() == 1) )    &&
	      !( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) ) &&
	      !( (psubevt->GetType() == 88) && (psubevt->GetSubtype() == 8800) ) )
	    {  // for all data 
	      std::cout << "getsubtype " << psubevt->GetSubtype() << std::endl;
	      std::cout << "gettype " << psubevt->GetType() << std::endl;
	      std::cout << "Wrong subevent type " << psubevt->GetType() << std::endl;
	      analyse = false; // skip subevent SL
	    }
	  
	  if( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) )
	    analyse = false;
	  /*    Now select subcrate number:  */
	  //      if((psubevt->GetSubcrate())!=0) {
	  //	std::cout << "Non supported subcrate " << psubevt->GetSubcrate() << std::endl;
	  //	continue; // skip subevent SL
	  //}
         
	  /************************************************************************/
	  /* Here we go for the different triggers.....                           */
	  /************************************************************************/
	  //      printf("TRI 2:%d\n",fInput->GetTrigger());
	  //      printf("PROC 2:%d\n",psubevt->GetProcid());

	  if(fInput->GetTrigger() == 13 && analyse)
	    { // trigger value
      

	      Int_t *pdata = psubevt->GetDataField();
	      Int_t len = 0;	  
	      Int_t vme_chn;
	      Int_t lenMax = (psubevt->GetDlen()-2)/2;
	      const auto it_Crate = ModSetup->MapCrates.find(psubevt->GetProcid());
	      if(it_Crate == ModSetup->MapCrates.end())
		std::cout<<"E> Crate Mapping does not have this ProcID :"<<psubevt->GetProcid()<<std::endl;
	      
	      if(ModSetup->EventFlags.size()!=0)
		{
		  Int_t  event_flag = *pdata++;
		  len++; // 0x200
		  
	    #ifdef DEBUG
		  std::cout<<" Event FLAG :"<<std::endl;
		  std::cout<<std::bitset<32>(event_flag)<<"\n";
		  for(Int_t elem : ModSetup->EventFlags)
		    std::cout<<std::bitset<32>(elem)<<"\n";
	    #endif
		  bool foundFlag = false;
		  for(int setupFlag : ModSetup->EventFlags)
		    if(event_flag == setupFlag)
		      foundFlag = true;
		  if(foundFlag)
		    EventFlag = event_flag;
		}
	    
	      
	      switch(psubevt->GetProcid())
		{
	    
		  
		//===========
		case 30:
		//case 10: //10 was changed to 30; (06.07.2018)
		// (FRS CRATE)
	    #ifdef DEBUG
		  std::cout<<"ProcID :"<<30<<std::endl; //changed to 30
	    #endif
		
		  // get pointer on data
	    
		  //Int_t *pdata = psubevt->GetDataField();
		  //Int_t len = 0;
		  if(ModSetup->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
		   {
		      // \note FRS TIME STAMP module data (3 longwords)   
		      //   has no header or end-of-block info, we must hardwire GEO = 20.
		      //
		      Long64_t tempTS = 0;
		      static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
		      for (int i=0;i<4;++i)
			{
			  vme0[20][i] = getbits(*pdata,1,1,16);
			  tempTS += vme0[20][i]*bit_weight[i];
			  
			  //vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
			  pdata++; len++;
			}
	    
		      previousTimeStamp[0] = currentTimeStamp;
		      currentTimeStamp = tempTS;
		  }
		  
		  if( (psubevt->GetType() != 88) )
		    break;     
	    
		  //pdata++; len++; // remove 0xbaba.baba    
	    
		  
		  if(ModSetup->Nb_Scaler > 0) //v830 scaler
		    {
		      // \note FRS SCALER module data (1 longword per channel)   
		       //  This module has sequential readout therefore no channel
		       //  number contained in the data longwords. 
		       //
		      // read the header longword and extract slot, type & length  
		      Int_t vme_geo = getbits(*pdata,2,12,5);
		      Int_t vme_type = getbits(*pdata,2,9,3);
		      Int_t vme_nlw =  getbits(*pdata,2,3,6);
	    
		      //printf("Event_flag_test=0x%08x\n", EventFlag);
			      
		      pdata++; len++;
	    
		      const auto it_Module = it_Crate->second.find(vme_geo);
		      int IdMod = it_Module->second;
		      if(it_Module == it_Crate->second.end())
			std::cout<<"E> Crate Mapping does not have this module (vmeGEO==) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
	    
		      if(vme_type!=4)
			std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;
	    
	    #ifdef DEBUG
		      std::cout<<"Scaler :"<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<std::endl;
	    #endif
		      // read the data 
		      if (vme_nlw > 0)
			{
			  for(int i=0;i<vme_nlw;i++)
			    {
			      if(ModSetup->Scaler32bit) 
			      {
				  vme0[vme_geo][i] = *pdata;
				  //vme2scaler[i] = *pdata;
	    
	    #ifdef DEBUG
				  std::cout<<" Ch "<<i<<"# "<<vme0[vme_geo][i] <<std::endl;
	    #endif
				  }
				  else
				{
				  Int_t sc_data = get2bits(*pdata,1,1,26);
				  Int_t  sc_ch = get2bits(*pdata,1,27,5);
				  if(sc_ch != i)
				    std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
				  vme0[vme_geo][i] = sc_data;
				  //vme2scaler[i] = sc_data;
	    #ifdef DEBUG
				  std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
	    #endif
				}
			      pdata++; len++;
			    }
			  // read and ignore the expected "end-of-block" longword   
			  //pdata++; len++;
			}
		    }
	    
	    
		    
		  //=============================== (Added on 07.07.2018)
		  /* v775 TDC/V785 ADC */
		  
		    while (len < lenMax){	       
	    
	    
			    /* read the header longword and extract slot, type & length  */
			    Int_t vme_chn = 0;
			    Int_t vme_geo = getbits(*pdata,2,12,5);
			    Int_t vme_type = getbits(*pdata,2,9,3);
			    Int_t vme_nlw =  getbits(*pdata,1,9,6); 
	    
			    pdata++; len++;
	    
			    /* read the data */
			    if ((vme_type == 2) && (vme_nlw > 0)) {
				    for(int i=0;i<vme_nlw;i++) {  
					    vme_geo = getbits(*pdata,2,12,5);
					    vme_type = getbits(*pdata,2,9,3);
					    vme_chn = getbits(*pdata,2,1,5);
					    vme0[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    
					    pdata++; len++;
				    }
		  
			    }
	    
			    /* read and ignore the expected "end-of-block" longword */
			    pdata++; len++;
		
		    }  /* end of the while... loop  */
		 
		   //===============================
	    
		  //All the ADCs are read with the code just above..(07.07.2018)
		    
		  //  now start with the REST of the unpacking...
		  while (len < (psubevt->GetDlen()-2)/2)
		    {
				      
		      // read the header longword and extract slot, type & length  
	    #ifdef DEBUG
		      std::cout<<"word :"<<std::bitset<32>(*pdata)<<" "<<std::endl;
	    #endif
		      Int_t vme_geo = getbits(*pdata,2,12,5);
		      Int_t vme_type = getbits(*pdata,2,9,3);
		      Int_t vme_nlw =  getbits(*pdata,1,9,6);
		      pdata++; len++;
		      const auto it_Module = it_Crate->second.find(vme_geo);
		      int IdMod = it_Module->second;
	    #ifdef DEBUG
		      std::cout<<"data "<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<" idmod:"<<IdMod<<std::endl;
	    #endif
		      if(it_Module == it_Crate->second.end())
			std::cout<<"E> Crate Mapping does not have this module (vmeGEO--) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;

		      if ((vme_type == 2) && (vme_nlw > 0))
			{
			  for(int i=0;i<vme_nlw;i++)
			    {  
			      vme_geo = getbits(*pdata,2,12,5);
			      vme_type = getbits(*pdata,2,9,3);
			      vme_chn = getbits(*pdata,2,1,5);
			      
			      vme0[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
			      Int_t vme_statusVD = getbits(*pdata,14,1,1);
			      Int_t vme_statusUN = getbits(*pdata,13,1,1);
			      Int_t vme_statusOV = getbits(*pdata,12,1,1);
	    
	    #ifdef DEBUG
			      std::cout<<" Ch "<<vme_chn<<" "<<vme0[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
	    #endif
			      pdata++; len++;
			    }
					  
			  // read and ignore the expected "end-of-block" longword 
			  pdata++; len++;
			}
				      
		    }  // end of the while... loop  
				  
		  break; 
		  // end proc ID=30 //changed from 10 to 30 (06.07.2018)
		  //==========
	    
		  
		//========    
		// case 15:
		//   //proc ID=15 - TimeStamp
		//   // get pointer on data   
		  
		//   //Int_t vme_geo;
		//   //Int_t vme_nlw;
		//   //Int_t vme_type;
		  
		//   if (lenMax<2)
		// 	{
		// 	  std::cout << "Bad timming subevent data" << std::endl;
		// 	  return kFALSE;
		// 	}
		  
		//   //Titris information read in subevent 15 as module number 20 in vme0
		//   //and then system time (2x32 bits words) put in the same way
		//   vme_chn = 0  ; 
		  
		//   for (int i=0;i<5;i++)
		// 	{
		// 	  vme0[20][vme_chn++] = getbits(*pdata,1,1,16);
		// 	  vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
		// 	  pdata++; len++;
		// 	}            
		//   break;
		//   // ID=15 loop
		//==========
	    
		  
		//==========  
		//case 30: //changed to 10  (06.07.2018);
		case 10:
		  //  (Main Crate)
		// get pointer on data   
	    #ifdef DEBUG
		  std::cout<<"ProcID :"<<10<<std::endl; //changed from 30 to 10
	    #endif
		  // Int_t *pdata = psubevt->GetDataField();
		  // Int_t len = 0;	  
		  // //Int_t vme_chn;
		  // Int_t lenMax = (psubevt->GetDlen()-2)/2;
		    
		  if (lenMax<2)
		    {
		      std::cout << "Bad User subevent data" << std::endl;
		    }
		  // std::cout<<"length = "<<lenMax<<std::endl;
		  /*
		  if(ModSetup->Nb_TDC>0)
		    {
		      //v1290 TDC              
		      Int_t vme_geo = getbits(*pdata,1,1,5);
		      Int_t vme_type = getbits(*pdata,2,12,5);
		      pdata++; len++;
		      Int_t multihit = 0;//, counter = 0;
	    
		      const auto it_Module = it_Crate->second.find(vme_geo);
		      int IdMod = it_Module->second;
		      if(it_Module == it_Crate->second.end())
		      std::cout<<"E> Crate Mapping does not have this module (vmeGEO**) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
	    
	    #ifdef DEBUG
		      std::cout<<"mTDC geo = "<<vme_geo<<", type = "<<vme_type<<std::endl;
	    #endif  
	    
		      if (vme_type == 8)
			{
			  while (len < lenMax) 
			    {
	    #ifdef DEBUG
			      std::cout<<"word : "<<std::bitset<32>(*pdata)<<" ";
	    #endif
			      vme_type = getbits(*pdata,2,12,5);
			      if(vme_type==1) // headerTDC
				{ 
				  pdata++; len++;
				}
			      //multihit = 0;
	    #ifdef DEBUG
			      std::cout<<"reading :"<<vme_type<<std::endl;
	    #endif
			      vme_type = getbits(*pdata,2,12,5);
	    #ifdef DEBUG
			      std::cout<<"word : "<<std::bitset<32>(*pdata)<<" type:"<<vme_type<<" ";
	    #endif
			      if(vme_type == 0)
				{
				  // this indicates a TDC measurement
				  Int_t vme_chn = getbits(*pdata,2,6,5);
				  Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
				  Int_t value = getbits(*pdata,1,1,21);
	    
				  multihit = nhit5[vme_chn][LeadingOrTrailing];
	    #ifdef DEBUG
				  std::cout << "     tdc vme_chn = " << vme_chn;
				  std::cout << " multihit: " << multihit << " ";
	    #endif
				  if (multihit >= 10)
				    {
				      pdata++; len++;
				      continue;
				    }
				  if(LeadingOrTrailing == 0)
				    {
	    #ifdef DEBUG
				      std::cout << " +-> tdc L value = " << value << std::endl;
	    #endif
				      if (value > 0)
					{
					  vme2s[vme_chn][multihit] = value;
					  hVME2_TDC[vme_chn]->Fill(value);
					}
				    }
			      
				  else
				    {
	    #ifdef DEBUG
				      std::cout << " +-> tdc T value = " << value << std::endl;
	    #endif
				      if (value > 0)
					vme2s_trailing[vme_chn][multihit] = value;
				    }
				  nhit5[vme_chn][LeadingOrTrailing]++;
				
				  pdata++; len++;
				}
			      else
				{
				  // TDC trailer vme_type == 3 
	    #ifdef DEBUG
				  std::cout<<"\n";
	    #endif
				  if(vme_type != 3 && vme_type !=16)
				    std::cout<<"E> MTDC strange type :"<<vme_type<<std::endl;
				  if(vme_type==16)
				    {
				      Int_t vme_geoEnd = getbits(*pdata,1,1,5);
				      if(vme_geo!=vme_geoEnd)
					std::cout<<"E> MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl; 
				      break;
				    }
				  pdata++; len++;
				}
			    }
			}
		    }
		  */
	    
	    
		  /*********************************************/
		  /************* Added on 06.07.2018 ***********/
		  /*********************************************/
	    
		  //Main Crate QDC (v792) and scaler (v830) 
	    
		  // get pointer on data   
	    
		  //Int_t *pdata = psubevt->GetDataField();
		  //Int_t len = 0;
		  if(ModSetup->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
		    {
		      /** \note FRS TIME STAMP module data (3 longwords)   
		       *   has no header or end-of-block info, we must hardwire GEO = 20.
		       */
		      Long64_t tempTS = 0;
		      static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
		      for (int i=0;i<4;++i)
			{
			  vme5[20][i] = getbits(*pdata,1,1,16);
			  tempTS += vme5[20][i]*bit_weight[i];
			  //vme5[20][vme_chn++] = getbits(*pdata,2,1,16);
			  //std::cout << "test4words==" << *pdata <<  "   " << len << std::endl;
			  pdata++; len++;
			  
			}
		      previousTimeStamp[0] = currentTimeStamp;
		      currentTimeStamp = tempTS;
		    }     
		  
		  if( (psubevt->GetType() != 88) )
		    break;
		  
		  pdata++; len++; // remove 0xbaba.baba
		  
		  if(ModSetup->Nb_Scaler > 0)
		  {
		      /** \note FRS SCALER module data (1 longword per channel)   
		       *  This module has sequential readout therefore no channel
		       *  number contained in the data longwords. 
		       */
		      // read the header longword and extract slot, type & length  
		      Int_t vme_geo = getbits(*pdata,2,12,5);
		      Int_t vme_type = getbits(*pdata,2,9,3);
		      Int_t vme_nlw =  getbits(*pdata,2,3,6);
	    
		      pdata++; len++;
	    
		      const auto it_Module = it_Crate->second.find(vme_geo);
		      int IdMod = it_Module->second;
		      if(it_Module == it_Crate->second.end())
			std::cout<<"E> Crate Mapping does not have this module (vmeGEO==) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
	    
		      if(vme_type!=4)
			std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;
	    
	    #ifdef DEBUG
		      std::cout<<"Scaler :"<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<std::endl;
	    #endif    
		      // read the data 
		      if (vme_nlw > 0)
			{
			  for(int i=0;i<vme_nlw;i++)
			    {
			      if(ModSetup->Scaler32bit)
				{
	    
				  vme5[vme_geo][i] = *pdata;
	    
	    #ifdef DEBUG
				  std::cout<<" Ch "<<i<<"# "<<vme5[vme_geo][i] <<std::endl;
	    #endif
				}
			      else
				{
				  Int_t sc_data = get2bits(*pdata,1,1,26);
				  Int_t  sc_ch = get2bits(*pdata,1,27,5);
				  if(sc_ch != i)
				    std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
				  vme5[vme_geo][i] = sc_data;
	    #ifdef DEBUG
				  std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
	    #endif
				}
			      pdata++; len++;
			    }
			  // std::cout<<"1Hz unpack, "<<vme0[6][3]<<std::endl;
			  // read and ignore the expected "end-of-block" longword   
			  //pdata++; len++;
			  }
		   }
		  //  now start with the REST of the unpacking...     
		  
		  while (len < (psubevt->GetDlen()-2)/2)
		    {
				      
		      // read the header longword and extract slot, type & length  
	    #ifdef DEBUG
		      std::cout<<"word :"<<std::bitset<32>(*pdata)<<" "<<std::endl;
	    #endif
		      Int_t vme_chn = 0;
		      Int_t vme_geo = getbits(*pdata,2,12,5);
		      Int_t vme_type = getbits(*pdata,2,9,3);
		      Int_t vme_nlw =  getbits(*pdata,1,9,6); 
		      
		      pdata++; len++;
		      
		      const auto it_Module = it_Crate->second.find(vme_geo);
	    
		      if(it_Module == it_Crate->second.end())
		      std::cout<<"E> Crate Mapping does not have this module (vmeGEO) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
		      
		      int IdMod = it_Module->second;
		      
	    #ifdef DEBUG
		 std::cout<<"data "<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<" idmod:"<<IdMod<<std::endl;
	    #endif
		      
		      if ((vme_type == 2) && (vme_nlw > 0))
			{
			  for(int i=0;i<vme_nlw;i++)
			    {  
			      vme_geo = getbits(*pdata,2,12,5);
			      vme_type = getbits(*pdata,2,9,3);
			      vme_chn = getbits(*pdata,2,1,5);
			      
			      vme5[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
			      Int_t vme_statusVD = getbits(*pdata,14,1,1);
			      Int_t vme_statusUN = getbits(*pdata,13,1,1);
			      Int_t vme_statusOV = getbits(*pdata,12,1,1);
	    
	    #ifdef DEBUG
			      std::cout<<" Ch "<<vme_chn<<" "<<vme5[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
	    #endif
			      pdata++; len++;
			    }
			  
			  // read and ignore the expected "end-of-block" longword 
			  pdata++; len++;
			}
				      
		    }  // end of the while... loop  
	    
		  /********* Added on 06.07.2018 *********/ 
		  break;
		  // ID=10 loop   //changed to 10 from 30 on 06.07.2018
		  //=========
	    
		
		  
		  //=========	    
		case 20:
	    #ifdef DEBUG
		  std::cout<<"ProcID "<<20<<std::endl;
	    #endif      
		  //UnpackUserSubevent(psubevt, event_out); //for historical reasons
		  
	    //       if(ModSetup->Nb_Scaler > 0)
	    // 	{
	    // 	  /** \note FRS SCALER module data (1 longword per channel)   
	    // 	   *  This module has sequential readout therefore no channel
	    // 	   *  number contained in the data longwords. 
	    // 	   */
	    // 	  // read the header longword and extract slot, type & length
	    // #ifdef DEBUG
	    // 	  std::cout<<"word :"<<std::bitset<32>(*pdata)<<" "<<std::endl;
	    // #endif  
	    // 	  Int_t vme_geo = getbits(*pdata,2,12,5);
	    // 	  Int_t vme_type = getbits(*pdata,2,9,3);
	    // 	  Int_t vme_nlw =  getbits(*pdata,2,3,6);
	    // 	  pdata++; len++;
	    // 	  if(vme_type!=4)
	    // 	    std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;
		      
	    // #ifdef DEBUG
	    // 	  std::cout<<"Scaler :"<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<std::endl;
	    // #endif	  
	    // 	  // read the data 
	    // 	  if (vme_nlw > 0)
	    // 	    {
	    // 	      for(int i=0;i<vme_nlw;i++)
	    // 		{
	    // 		  if(ModSetup->Scaler32bit)
	    // 		    {
	    // 		      vme1[vme_geo][i] = *pdata;
	    // #ifdef DEBUG
	    // 		      std::cout<<" Ch "<<i<<"# "<<vme0[vme_geo][i] <<std::endl;
	    // #endif
	    // 		    }
	    // 		  else
	    // 		    {
	    // 		      Int_t sc_data = get2bits(*pdata,1,1,26);
	    // 		      Int_t  sc_ch = get2bits(*pdata,1,27,5);
	    // 		      if(sc_ch != i)
	    // 			std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
	    // 		      vme1[vme_geo][i] = sc_data;
	    // #ifdef DEBUG
	    // 		      std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
	    // #endif
	    // 		    }
	    // 		  pdata++; len++;
	    // 		}
	    // 	      // std::cout<<"1Hz unpack, "<<vme0[6][3]<<std::endl;
	    // 	      // read and ignore the expected "end-of-block" longword   
	    // 	      //pdata++; len++;
	    // 	    }
	    // 	}
	    
	    
		    //printf("len %d, psubevrelen %d\n ",len,(psubevt->GetDlen()-2)/2);
	    
		    //std::cout << "  LENMAX===" << lenMax << std::endl;
		    //printf("test4words==0x%08x\n", *pdata);
		    
		    //for(int ii=0; ii<5; ii++){
		    //printf("test5words==0x%08x\n", *(pdata+ii));
		    //}
		
		    //cout << "vme_type " << vme_type << " vme_nlw  " << vme_nlw << endl; 
		    //std::cout << "VME_TYPE===" << vme_type << "  VME_GEO===" << vme_geo << "  VME_NLW==" << vme_nlw <<  std::endl;
		    //std::cout << "VME_TYPE===" << vme_type << "  VME_GEO===" << vme_geo <<   "   vme_chn=" << vme_chn << " " <<  getbits(*pdata,1,1,16) << std::endl;
	    
		     //=============================== (Added on 07.07.2018)
		     /* v775 TDC/V785 ADC */
		  
			while (len < lenMax){
	    
			    /* read the header longword and extract slot, type & length  */
			    Int_t vme_chn = 0;
			    Int_t vme_geo = getbits(*pdata,2,12,5);
			    Int_t vme_type = getbits(*pdata,2,9,3);
			    Int_t vme_nlw =  getbits(*pdata,1,9,6); 
	    
			    pdata++; len++;
	    
			    /* read the data */
			    if ((vme_type == 2) && (vme_nlw > 0)) {
				    for(int i=0;i<vme_nlw;i++) {  
					    vme_geo = getbits(*pdata,2,12,5);
					    vme_type = getbits(*pdata,2,9,3);
					    vme_chn = getbits(*pdata,2,1,5);
					    vme1[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    
					    pdata++; len++;
				    }
		  
			    }
	    
			    /* read and ignore the expected "end-of-block" longword */
			    pdata++; len++;
		
			}  /* end of the while... loop  */
		 
		   //===============================      
		  //All the ADCs are read with the code just above..(07.07.2018)
			
		  /* for ProcID = 20 - rest of the unpacking */
		  while (len < (psubevt->GetDlen()-2)/2)
		    {
		  
		      /* read the header longword and extract slot, type & length  */
		      Int_t vme_chn = 0;
		      Int_t vme_geo = getbits(*pdata,2,12,5);
		      Int_t vme_type = getbits(*pdata,2,9,3);
		      Int_t vme_nlw =  getbits(*pdata,1,9,6);
		      
		      pdata++; len++;
	    #ifdef DEBUG
		      std::cout<<"data "<<vme_geo<<" "<<vme_type<<" "<<vme_nlw;
	    #endif      
		      const auto it_Module = it_Crate->second.find(vme_geo);
		      if(it_Module == it_Crate->second.end()){
		      std::cout<<"E> Crate Mapping does not have this module (vmeGEO$$) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
		      } 
	    
		      int IdMod = it_Module->second;
	    #ifdef DEBUG
		      std::cout<<" idmod:"<<IdMod<<std::endl;
	    #endif      

		      /* read the data */
		      if ((vme_type == 2) && (vme_nlw > 0))
			{
			  for(int i=0;i<vme_nlw;i++)
			    {  
			      vme_geo = getbits(*pdata,2,12,5);
			      vme_type = getbits(*pdata,2,9,3);
			      vme_chn = getbits(*pdata,2,1,5);
			      vme1[vme_geo][vme_chn] = getbits(*pdata,1,1,16);	                 
			      //printf("DATA:%d %d %d\n",vme_geo,vme_chn,vme1[vme_geo][vme_chn] ); 
			  
			      int vme_statusVD = getbits(*pdata,14,1,1);
			      int vme_statusUN = getbits(*pdata,13,1,1);
			      int vme_statusOV = getbits(*pdata,12,1,1);

	    
	    #ifdef DEBUG
			      std::cout<<" Ch "<<vme_chn<<" "<<vme1[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
	    #endif
			      pdata++; len++;
			    }
		      
			}
	    
		       /* read and ignore the expected "end-of-block" longword */
			  pdata++; len++;
		  
		    }  /* end of the while... loop  */
		  break;
		  //=========
		  
	    
		  //============
		case 40 :
		  
		  if(ModSetup->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
		    {
		      /** \note FRS TIME STAMP module data (3 longwords)   
		       *   has no header or end-of-block info, we must hardwire GEO = 20.
		       */
		      Long64_t tempTS = 0;
		      static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
		      for (int i=0;i<4;++i)
			{
			  vme3[20][i] = getbits(*pdata,1,1,16);
			  tempTS += vme3[20][i]*bit_weight[i];
			  //vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
			  pdata++; len++;
			}
		      previousTimeStamp[1] = currentTimeStamp;
		      currentTimeStamp = tempTS;	  
		    }
	    
		  if( (psubevt->GetType() != 88) )
		    break;
	    
		  pdata++; len++; // remove 0xbaba.baba
	    
		  
		      // v820 -> no header 16 ch of 32 bits.
		  for(int i=0;i<16;++i)
		    {
		      vme3scaler[i] = *pdata;
	    #ifdef DEBUG
		      std::cout<<" Ch "<<i<<"# "<<vme3scaler[i] <<std::endl;
	    #endif
		      pdata++; len++;
		    }
	    
	    
		  static std::bitset<32> barrier_test(0xbabababa);
		  if(std::bitset<32>(*pdata) !=barrier_test)
		    std::cout<<"E> ProcID 40 : Barrier missed !"<<std::bitset<32>(*pdata)<<" =/= 0xbabababa | "<<barrier_test<<std::endl;
		  pdata++; len++;
		  
		  
		  if(ModSetup->Nb_TDC>0)
		    {
		      //v1290 TDC              
		      Int_t vme_geo = getbits(*pdata,1,1,5);
		      Int_t vme_type = getbits(*pdata,2,12,5);
		      pdata++; len++;
		      Int_t multihit = 0;//, counter = 0;
	    
		      const auto it_Module = it_Crate->second.find(vme_geo);
		      if(it_Module == it_Crate->second.end())
			std::cout<<"E> Crate Mapping does not have this module (vmeGEO//) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
		      
	    #ifdef DEBUG
		      std::cout<<"mTDC geo = "<<vme_geo<<", type = "<<vme_type<<std::endl;
	    #endif  
	    
		      if (vme_type == 8)
			{
			  while (len < lenMax) 
			    {
	    #ifdef DEBUG
			      std::cout<<"word : "<<std::bitset<32>(*pdata)<<" ";
	    #endif
			      vme_type = getbits(*pdata,2,12,5);
			      if(vme_type==1) // headerTDC
				{ 
				  pdata++; len++;
				}
			      //multihit = 0;
	    #ifdef DEBUG
			      std::cout<<"reading "<<vme_type<<std::endl;
	    #endif
			      vme_type = getbits(*pdata,2,12,5);
	    #ifdef DEBUG
			      std::cout<<" word : "<<std::bitset<32>(*pdata)<<" type:"<<vme_type<<" ";
	    
	    #endif
			      if(vme_type == 0)
				{
				  // this indicates a TDC measurement
				  Int_t vme_chn = getbits(*pdata,2,6,5);
				  Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
				  Int_t value = getbits(*pdata,1,1,21);
				  multihit = vme3_MT_nhit5[vme_chn][LeadingOrTrailing];
	    #ifdef DEBUG
				  std::cout << "     tdc vme_chn = " << vme_chn;
				  std::cout << " multihit: " << multihit << " ";
	    #endif
				  if (multihit >= 10)
				    {
				      pdata++; len++;
				      continue;
				    }
				  if(LeadingOrTrailing == 0)
				    {
	    #ifdef DEBUG
				      std::cout << " +-> tdc L value = " << value << std::endl;
	    #endif
				      if (value > 0)
					{
					  vme3s_MT[vme_chn][multihit] = value;
					}
				    }
			      
				  else
				    {
	    #ifdef DEBUG
				      std::cout << " +-> tdc T value = " << value << std::endl;
	    #endif
				      if (value > 0)
					vme3s_MT_trailing[vme_chn][multihit] = value;
				    }
				  vme3_MT_nhit5[vme_chn][LeadingOrTrailing]++;
				
				  pdata++; len++;
				}
			      else
				{
				  // TDC trailer vme_type == 3
	    #ifdef DEBUG
				  std::cout<<"\n";
	    #endif
				  if(vme_type != 3 && vme_type !=16)
				    std::cout<<"E> MTDC strange type :"<<vme_type<<std::endl;
				  if(vme_type==16)
				    {
				      Int_t vme_geoEnd = getbits(*pdata,1,1,5);
				      if(vme_geo!=vme_geoEnd)
					std::cout<<"E> MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl; 
				      break;
				    }
				  pdata++; len++;
	    
				}
			    }
			}
		    }
		  break;
		  //===========
	    
		  //===========
		case 50:
	    
		  if(ModSetup->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
		    {
		      /** \note FRS TIME STAMP module data (3 longwords)   
		       *   has no header or end-of-block info, we must hardwire GEO = 20.
		       */
		      Long64_t tempTS = 0;
		      static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
		      for (int i=0;i<4;++i)
			{
			  vme4[20][i] = getbits(*pdata,1,1,16);
			  tempTS += vme4[20][i]*bit_weight[i];
			  //vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
			  pdata++; len++;
			}
		      previousTimeStamp[2] = currentTimeStamp;
		      currentTimeStamp = tempTS;	  
		    }
	    
		default :
		  break;
		} // end switch prodID

    }
    else cout<<"Trigger messed up "<<fInput->GetTrigger()<<endl;

}

void FRS_Detector_System::FRS_Sort(){
    
      /* now we can assign the parameters according to cabling:  */

  /* ### timestamp: */
  EventFlag = EventFlag;

  //  ts_id = vme0[20][0];

  if(EventFlag==0x100)
    {
      ts_word[0] = vme0[20][0];
      ts_word[1] = vme0[20][1];
      ts_word[2] = vme0[20][2];
      ts_word[3] = vme0[20][3];
    }
  else if(EventFlag==0x200)
    {
      ts_word[0] = vme3[20][0];
      ts_word[1] = vme3[20][1];
      ts_word[2] = vme3[20][2];
      ts_word[3] = vme3[20][3];
    }
  else if(EventFlag==0x300)
    {
      ts_word[0] = vme4[20][0];
      ts_word[1] = vme4[20][1];
      ts_word[2] = vme4[20][2];
      ts_word[3] = vme4[20][3];
    }

  timestamp = Long64_t(1)*ts_word[0] + Long64_t(0x10000)*ts_word[1] + Long64_t(0x100000000)*ts_word[2] + Long64_t(0x1000000000000)*ts_word[3];
  // printf("qtrigger=%d timestamp=%ld \n",qtrigger,timestamp);

  tsys_word[0] = Long64_t(1)*ts_word[0] + Long64_t(0x10000)*ts_word[1] ; //s time low word
  tsys_word[1] = Long64_t(0x100000000)*ts_word[2] + Long64_t(0x1000000000000)*ts_word[3] ; //s time high worid... we do not use it
  if(PreviousTS < 0)
    tsys_word[2] = 0;
  else
    tsys_word[2] = (timestamp - PreviousTS)*1.e-5 ; //ms time since the previous s time (ftime routine)

  systemtime_s = tsys_word[2]*1e-3; //tsys_word[0] ; 
  systemtime_ms= tsys_word[2] ; 


  if (qtrigger==12)
    {
      StartOfSpilTime = timestamp; 
      //StartOfSpilTime = 0; 
      StartOfSpilTime2 = timestamp;
      //StartOfSpilTime2 = 0;  
      // printf("12 spill start at %ld ",StartOfSpilTime);
    }   
  else if (qtrigger==13)
    {
      StartOfSpilTime = -1;
    }
  //else                         //changed 170309
  // rest are interesting only if trigger == 1
  //if (qtrigger!=1 ) return;             
  
  // calculate time from spill start in sec
  if (StartOfSpilTime>=0) 
    { 
      timespill = (timestamp - StartOfSpilTime) * 1e-2;// microsec // 50000000.;
      //timespill = 1;
      //printf("timespill= %f \n",timespill);    
    }
  
  timespill2 = (timestamp - StartOfSpilTime2) * 1e-2; //microsec  // 50000000.;
  //timespill2 = 1; 

  /* ### Pattern ###*/
  pattern = vme0[5][0];
  trigger = qtrigger;
  

  /* ### scalers:  */
  /* these are treated as 32-bit integers directly  */
  for(int i=0;i<32;i++)
    {
      //sc_long[i] = vme2scaler[i]; //   
      //sc_long2[i] = vme3scaler[i];//vme1[5][i];
      sc_long[i]  = vme0[0][i]; //frs crate
      sc_long[32+i] = vme5[8][i]; //main crate
      //sc_long2[i] = vme5[8][i]; //main crate
    }

  //std::cout<<"1Hz sort,"<<vme0[6][3]<<std::endl;       
 
  /* ### TA Ionization Chamber dE:  */
  //  ic_de = vme0[13][16] & 0xfff;
  
    
  /* ### MW anodes:  */
  for(int i=0;i<13;i++)
    mw_an[i] = vme0[8][i] & 0xfff;
    
  /* ### MW cathodes:  */
  // from MW11 -> MW31
  for(int i=0;i<4;++i)
    {  
      mw_xr[i] = vme0[8][16+i*4] & 0xfff;
      mw_xl[i] = vme0[8][17+i*4] & 0xfff;
      mw_yu[i] = vme0[8][18+i*4] & 0xfff;
      mw_yd[i] = vme0[8][19+i*4] & 0xfff;
    }
  // from MW41 -> MW51
  for(int i=0;i<3;++i)
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
  for(int i=0;i<5;++i)
    {
      mw_xr[i+8] = vme0[9][12+i*4] & 0xfff;
      mw_xl[i+8] = vme0[9][13+i*4] & 0xfff;
      mw_yu[i+8] = vme0[9][14+i*4] & 0xfff;
      mw_yd[i+8] = vme0[9][15+i*4] & 0xfff;
    }



  //////////////////////////////////////
  // TPC part                         //
  //                                  //
  //////////////////////////////////////
  
  //ADC

  //TPC 1 at S2 (TPC 21) in vaccuum
  tpc_a[0][0]=vme1[15][0] & 0xfff;
  tpc_a[0][1]=vme1[15][1] & 0xfff;
  tpc_a[0][2]=vme1[15][2] & 0xfff;
  tpc_a[0][3]=vme1[15][3] & 0xfff;
  tpc_l[0][0]=vme1[15][4] & 0xfff;
  tpc_r[0][0]=vme1[15][5] & 0xfff;
  tpc_l[0][1]=vme1[15][6] & 0xfff;
  tpc_r[0][1]=vme1[15][7] & 0xfff;

  //TPC 2 at S2 (TPC 22) in vaccuum
  tpc_a[1][0]=vme1[15][8] & 0xfff;
  tpc_a[1][1]=vme1[15][9] & 0xfff;
  tpc_a[1][2]=vme1[15][10] & 0xfff;
  tpc_a[1][3]=vme1[15][11] & 0xfff;
  tpc_l[1][0]=vme1[15][12] & 0xfff;
  tpc_r[1][0]=vme1[15][13] & 0xfff;
  tpc_l[1][1]=vme1[15][14] & 0xfff;
  tpc_r[1][1]=vme1[15][15] & 0xfff;

  //TPC 3 at S2 (TPC 23) in air
  tpc_a[2][0]=vme1[15][16] & 0xfff;
  tpc_a[2][1]=vme1[15][17] & 0xfff;
  tpc_a[2][2]=vme1[15][18] & 0xfff;
  tpc_a[2][3]=vme1[15][19] & 0xfff;
  tpc_l[2][0]=vme1[15][20] & 0xfff;
  tpc_r[2][0]=vme1[15][21] & 0xfff;
  tpc_l[2][1]=vme1[15][22] & 0xfff;
  tpc_r[2][1]=vme1[15][23] & 0xfff;
  
  //TPC 4 at S2 (TPC 24) in air
  tpc_a[3][0]=vme1[15][24] & 0xfff;
  tpc_a[3][1]=vme1[15][25] & 0xfff;
  tpc_a[3][2]=vme1[15][26] & 0xfff;
  tpc_a[3][3]=vme1[15][27] & 0xfff;
  tpc_l[3][0]=vme1[15][28] & 0xfff;
  tpc_r[3][0]=vme1[15][29] & 0xfff;
  tpc_l[3][1]=vme1[15][30] & 0xfff;
  tpc_r[3][1]=vme1[15][31] & 0xfff;
  
  //TPC 5  at S4 (TPC 41) in air
  tpc_a[4][0]=vme1[3][0] & 0xfff;
  tpc_a[4][1]=vme1[3][1] & 0xfff;
  tpc_a[4][2]=vme1[3][2] & 0xfff;
  tpc_a[4][3]=vme1[3][3] & 0xfff;
  tpc_l[4][0]=vme1[3][4] & 0xfff;
  tpc_r[4][0]=vme1[3][5] & 0xfff;
  tpc_l[4][1]=vme1[3][6] & 0xfff;
  tpc_r[4][1]=vme1[3][7] & 0xfff;

  //TPC 6 at S4 (TPC 42) in air
  tpc_a[5][0]=vme1[3][8] & 0xfff;
  tpc_a[5][1]=vme1[3][9] & 0xfff;
  tpc_a[5][2]=vme1[3][10] & 0xfff;
  tpc_a[5][3]=vme1[3][11] & 0xfff;
  tpc_l[5][0]=vme1[3][12] & 0xfff;
  tpc_r[5][0]=vme1[3][13] & 0xfff;
  tpc_l[5][1]=vme1[3][14] & 0xfff;
  tpc_r[5][1]=vme1[3][15] & 0xfff;

  //TPC at S3 (TPC 31) (checkSB)
  tpc_a[6][0]=vme1[3][16] & 0xfff;
  tpc_a[6][1]=vme1[3][17] & 0xfff;
  tpc_a[6][2]=vme1[3][18] & 0xfff;
  tpc_a[6][3]=vme1[3][19] & 0xfff;
  tpc_l[6][0]=vme1[3][20] & 0xfff;
  tpc_r[6][0]=vme1[3][21] & 0xfff;
  tpc_l[6][1]=vme1[3][22] & 0xfff;
  tpc_r[6][1]=vme1[3][23] & 0xfff;
  
 
  //TDC

  //TPC 1 at S2 (TPC 21) in vaccuum
  tpc_dt[0][0]=vme1[8][0] & 0xfff;
  tpc_dt[0][1]=vme1[8][1] & 0xfff;
  tpc_dt[0][2]=vme1[8][2] & 0xfff;
  tpc_dt[0][3]=vme1[8][3] & 0xfff;
  tpc_lt[0][0]=vme1[8][4] & 0xfff;
  tpc_rt[0][0]=vme1[8][5] & 0xfff;
  tpc_lt[0][1]=vme1[8][6] & 0xfff;
  tpc_rt[0][1]=vme1[8][7] & 0xfff;

  //TPC 2 at S2 (TPC 22) in vaccuum
  tpc_dt[1][0]=vme1[8][8] & 0xfff;
  tpc_dt[1][1]=vme1[8][9] & 0xfff;
  tpc_dt[1][2]=vme1[8][10] & 0xfff;
  tpc_dt[1][3]=vme1[8][11] & 0xfff;
  tpc_lt[1][0]=vme1[8][12] & 0xfff;
  tpc_rt[1][0]=vme1[8][13] & 0xfff;
  tpc_lt[1][1]=vme1[8][14] & 0xfff;
  tpc_rt[1][1]=vme1[8][15] & 0xfff;

  //TPC 3 at S2 (TPC 23) in air
  tpc_dt[2][0]=vme1[8][16] & 0xfff;
  tpc_dt[2][1]=vme1[8][17] & 0xfff;
  tpc_dt[2][2]=vme1[8][18] & 0xfff;
  tpc_dt[2][3]=vme1[8][19] & 0xfff;
  tpc_lt[2][0]=vme1[8][20] & 0xfff;
  tpc_rt[2][0]=vme1[8][21] & 0xfff;
  tpc_lt[2][1]=vme1[8][22] & 0xfff;
  tpc_rt[2][1]=vme1[8][23] & 0xfff;

  //TPC 4 at S2 (TPC 24) in air
  tpc_dt[3][0]=vme1[8][24] & 0xfff;
  tpc_dt[3][1]=vme1[8][25] & 0xfff;
  tpc_dt[3][2]=vme1[8][26] & 0xfff;
  tpc_dt[3][3]=vme1[8][27] & 0xfff;
  tpc_lt[3][0]=vme1[8][28] & 0xfff;
  tpc_rt[3][0]=vme1[8][29] & 0xfff;
  tpc_lt[3][1]=vme1[8][30] & 0xfff;
  tpc_rt[3][1]=vme1[8][31] & 0xfff;

  //TPC 5 at S4 (TPC 41) in air  
  tpc_dt[4][0]=vme1[9][0] & 0xfff;
  tpc_dt[4][1]=vme1[9][1] & 0xfff;
  tpc_dt[4][2]=vme1[9][2] & 0xfff;
  tpc_dt[4][3]=vme1[9][3] & 0xfff;
  tpc_lt[4][0]=vme1[9][4] & 0xfff;
  tpc_rt[4][0]=vme1[9][5] & 0xfff;
  tpc_lt[4][1]=vme1[9][6] & 0xfff;
  tpc_rt[4][1]=vme1[9][7] & 0xfff;

  //TPC 6 at S4 (TPC 42) in air
  tpc_dt[5][0]=vme1[9][8] & 0xfff;
  tpc_dt[5][1]=vme1[9][9] & 0xfff;
  tpc_dt[5][2]=vme1[9][10] & 0xfff;
  tpc_dt[5][3]=vme1[9][11] & 0xfff;
  tpc_lt[5][0]=vme1[9][12] & 0xfff;
  tpc_rt[5][0]=vme1[9][13] & 0xfff;
  tpc_lt[5][1]=vme1[9][14] & 0xfff;
  tpc_rt[5][1]=vme1[9][15] & 0xfff;

  //TPC at S3 (TPC 31) (checkSB)
  tpc_dt[6][0]=vme1[9][16] & 0xfff;
  tpc_dt[6][1]=vme1[9][17] & 0xfff;
  tpc_dt[6][2]=vme1[9][18] & 0xfff;
  tpc_dt[6][3]=vme1[9][19] & 0xfff;
  tpc_lt[6][0]=vme1[9][20] & 0xfff;
  tpc_rt[6][0]=vme1[9][21] & 0xfff;
  tpc_lt[6][1]=vme1[9][22] & 0xfff;
  tpc_rt[6][1]=vme1[9][23] & 0xfff;


  
  
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


  //de_21ld = 0;    // 
  //de_21rd = 0;    //

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
  for(int i=0;i<10;i++){  
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
  for(int i=0;i<4;i++)
    {
      music_t3[i] = vme1[11][16+i] & 0xfff;    //TIME
      //music_t3[i] = 0;
      music_e3[i] = vme0[10][16+i] & 0xfff;    //ENERGY
      //music_e3[i] = 0;
    }
               
  /* ### TUM MUSIC dE:  */
  for(int i=0;i<8;i++)
    {
      music_e1[i] = vme0[11][i] & 0xfff;
      music_e2[i] = vme0[11][8+i] & 0xfff;
      
      //music_e2[i] = vme0[12][24+i] & 0xfff;

      music_t1[i] = vme2s[16+i][0] & 0xfff; //TIME
      music_t2[i] = vme2s[24+i][0] & 0xfff; //TIME

      //music_t1[i] = 0; //TIME
      //music_t2[i] = 0; //TIME 
    }
    
  /* ### MUSIC temp & pressure:  */
  //music_pres[0] = vme0[12][23] & 0xfff;
  music_pres[0] = 0; 
  //music_temp[0] = vme0[12][22] & 0xfff;
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
  for(int i=0;i<32;i++)
  {
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

/*void FRS_Detector_System::FRS_Calib(){
    
    
    
    
    
}*/


Int_t getbits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*16 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}

Int_t get2bits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*32 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}



void FRS_Detector_System::Process_MBS(int* pdata){return;
    
  //TFRSSortEvent *srt = dynamic_cast<TFRSSortEvent*> (GetInputEvent("Calibr"));
  
  
  
/*  TFRSCalibrEvent *cal = dynamic_cast<TFRSCalibrEvent*> (GetInputEvent("Analysis"));
  TFRSAnlEvent *anl = dynamic_cast<TFRSAnlEvent*> (GetInputEvent());

  // MUSIC PARAMETERS //

  for(int i=0; i < 3; ++i){	
  
      dE[i]     = anl.de[i];
      dE_cor[i] = anl.de_cor[i];
  
  }
  
  // SCINTILLATOR PARAMETERS //

  
  for(int i=0; i < 12; ++i){	

    sci_l[i]  = anl.sci_l[i];
    sci_r[i]  = anl.sci_r[i];
    sci_e[i]  = anl.sci_e[i];
    sci_tx[i] = anl.sci_tx[i];
    sci_x[i]  = anl.sci_x[i];
    
  }

  
  sci_tofll2 = anl.sci_tofll2;
  sci_tofll3 = anl.sci_tofll3;
  sci_tof2   = anl.sci_tof2;
  sci_tofrr2 = anl.sci_tofrr2;
  sci_tofrr3 = anl.sci_tofrr3;
  sci_tof3   = anl.sci_tof3;
  
  // ID PARAMETERS //
  
  ID_x2 = anl.id_x2;
  ID_y2 = anl.id_y2;
  ID_a2 = anl.id_a2;
  ID_b2 = anl.id_b2;
  
  ID_x4 = anl.id_x4;
  ID_y4 = anl.id_y4;
  ID_a4 = anl.id_a4;
  ID_b4 = anl.id_b4;
  
  
  for(int i = 0; i < 2; ++i){
      
    ID_brho[i] = anl.id_brho[i];
    ID_rho[i]  = anl.id_rho[i];

  }
  
  beta     = anl.id_beta;
  beta3    = anl.id_beta3;
  gamma    = anl.id_gamma;
  
  AoQ      = anl.id_AoQ;
  AoQ_corr = anl.id_AoQ_corr;
  
  z  = anl.id_z;
  z2 = anl.id_z2;
  z3 = anl.id_z3;
  
  timestamp = anl.timestamp;
  ts        = anl.ts;
  ts2       = anl.ts2;  */

}

//---------------------------------------------------------------

void FRS_Detector_System::get_Event_data(Raw_Event* RAW) { return;
    
    /*RAW->set_DATA_MUSIC(dE, dE_cor);
    RAW->set_DATA_SCI(sci_l, sci_r, sci_e, sci_tx, sci_x);
    RAW->set_DATA_SCI_ToF(sci_tofll2, sci_tofll3, sci_tof2, sci_tofrr2, sci_tofrr3, sci_tof3);
    RAW->set_DATA_ID_2_4(ID_x2, ID_y2, ID_a2, ID_b2, ID_x4, ID_y4, ID_a4, ID_b4);
    RAW->set_DATA_ID_Beta_Rho(ID_brho, ID_rho, beta, beta3, gamma);
    RAW->set_DATA_ID_Z_AoQ(AoQ, AoQ_corr, z, z2, z3);
    RAW->set_DATA_ID_Timestamp(timestamp, ts, ts2);*/

}

//---------------------------------------------------------------

int* FRS_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

