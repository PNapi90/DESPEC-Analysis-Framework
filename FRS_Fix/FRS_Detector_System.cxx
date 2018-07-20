#include "FRS_Detector_System.h"
#include "Riostream.h"

#include "TH1.h"
#include "TMap.h"

#include <cstdlib>
#include <iostream>
#include <bitset>
#include <sstream>
#include <fstream>
#include <map>
#include "TObject.h"

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"
#include "TGo4StepFactory.h"
#include "TGo4Analysis.h"

//#include "TXRSSortEvent.h"
//#include "TXRSCalibrEvent.h"

//#include "TXRSAnalysis.h"

#include "TXRSParameter.h"

FRS_Detector_System::FRS_Detector_System(){

    test = 0;

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



/*  TXRSSortEvent *srt = dynamic_cast<TXRSSortEvent*> (GetInputEvent("Calibr"));
  TXRSCalibrEvent *cal = dynamic_cast<TXRSCalibrEvent*> (GetInputEvent("Analysis"));
  TXRSAnlEvent *anl = dynamic_cast<TXRSAnlEvent*> (GetInputEvent());

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
void FRS_Detector_System::Process_FRS(TGo4MbsSubEvent* psubevt){
    
    
    Setup_Parameters();
    FRS_Unpack(psubevt);
    FRS_Sort();
    FRS_Calib();
    FRS_Anal();

    //TXRSSortEvent *srt = dynamic_cast<TXRSSortEvent*> (GetInputEvent("Calibr"));
        
    cout<<"HOLY FUCKING SHIT IT ACTUALLY WORKS!!!! "<<dt_21l_21r<<endl;

}

void FRS_Detector_System::FRS_Unpack(TGo4MbsSubEvent* psubevt){
    
  Int_t *pdata = psubevt->GetDataField();
  Int_t len = 0;	  
  Int_t vme_chn;
  Int_t lenMax = (psubevt->GetDlen()-2)/2;
    
  const auto it_Crate = ElecMod->MapCrates.find(psubevt->GetProcid());
  if(it_Crate == ElecMod->MapCrates.end())
    std::cout<<"E> Crate Mapping does not have this ProcID :"<<psubevt->GetProcid()<<std::endl;
    
  std::cout<<"Matt E> Crate Mapping does not have this ProcID :"<<std::endl;

  
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

  std::cout<<"Matt E> Crate Mapping does not have this ProcID :"<<std::endl;

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
      if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
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
      
      if( (psubevt->GetType() == 88) )
      {
      //pdata++; len++; // remove 0xbaba.baba    

      
      if(ElecMod->Nb_Scaler > 0) //v830 scaler
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
		  if(ElecMod->Scaler32bit) 
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
	      // std::cout<<"1Hz unpack, "<<vme0[6][3]<<std::endl;
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

	  // read the data
	  if(vme_type == 6)
	    {
	      // not valid data !
	      const auto MaxCh = ElecMod->Nb_Channels.find(vme_geo); 
	      for(int i=0;i<MaxCh->second;++i)
		{
		  //h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
		  //h_UnpackStatus->Fill(IdMod*MaxCh->second+i,6.);
		}
		 
	    }
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

		  /*if(vme_statusVD == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
		  else
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);

		  if(vme_statusUN == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);

		  if(vme_statusOV == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);
*/
#ifdef DEBUG
		  std::cout<<" Ch "<<vme_chn<<" "<<vme0[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
#endif
		  pdata++; len++;
		}
			      
	      // read and ignore the expected "end-of-block" longword 
	      pdata++; len++;
	    }
			  
	}  // end of the while... loop 
	
    }
     		      
      //break; 
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
    std::cout<<"Matt ProcID :"<<10<<std::endl; //changed from 30 to 10

      
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
      if(ElecMod->Nb_TDC>0)
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
      if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
	{
	  /** \note FRS TIME STAMP module data (3 longwords)   
	   *   has no header or end-of-block info, we must hardwire GEO = 20.
	   */
	   
	std::cout<<"Matt well we made it this far"<<std::endl;

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
	
      std::cout<<"Matt we made it this far too"<<std::endl;
     
      
      if( (psubevt->GetType() != 88) )
      {
      
      pdata++; len++; // remove 0xbaba.baba
      
      std::cout<<"Matt we made it this far also"<<std::endl;

      
      if(ElecMod->Nb_Scaler > 0)
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
		  if(ElecMod->Scaler32bit)
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
	  
	  // read the data
	   if(vme_type == 6)
	    {
	      // not valid data !
	      const auto MaxCh = ElecMod->Nb_Channels.find(vme_geo); 
	      for(int i=0;i<MaxCh->second;++i)
	      {
	         //h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
		  //h_UnpackStatus->Fill(IdMod*MaxCh->second+i,6.);
	      }
		 
	    }
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

		  /*if(vme_statusVD == 1)
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
		  else
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);

		  if(vme_statusUN == 1)
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);

		  if(vme_statusOV == 1)
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);*/

#ifdef DEBUG
		  std::cout<<" Ch "<<vme_chn<<" "<<vme5[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
#endif
		  pdata++; len++;
		}
	      
	      // read and ignore the expected "end-of-block" longword 
	      pdata++; len++;
	    }
			  
	}  // end of the while... loop  

    }
      /********* Added on 06.07.2018 *********/ 
      // ID=10 loop   //changed to 10 from 30 on 06.07.2018
      //=========

    
      
      //=========	    
    case 20:
#ifdef DEBUG
      std::cout<<"ProcID "<<20<<std::endl;
#endif      
      //UnpackUserSubevent(psubevt, event_out); //for historical reasons
      
//       if(ElecMod->Nb_Scaler > 0)
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
// 		  if(ElecMod->Scaler32bit)
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
	  // read the data
	  if(vme_type == 6)
	    {
	      // not valid data !
	      const auto MaxCh = ElecMod->Nb_Channels.find(vme_geo); 
	      for(int i=0;i<MaxCh->second;++i)
		{
		  //h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
		  //h_UnpackStatus->Fill(IdMod*MaxCh->second+i,6.);
		}
	  
	    }
	  //std::cout<<"type = "<<vme_type<<"nlw = "<<vme_nlw<<std::endl;
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

		  /*if(vme_statusVD == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
		  else
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);
	      
		  if(vme_statusUN == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);
	      
		  if(vme_statusOV == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);*/

#ifdef DEBUG
		  std::cout<<" Ch "<<vme_chn<<" "<<vme1[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
#endif
		  pdata++; len++;
		}
	  
	    }

	   /* read and ignore the expected "end-of-block" longword */
	      pdata++; len++;
      
	}  /* end of the while... loop  */
      //=========
      

      //============
    case 40 :
      
      if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
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

      if( (psubevt->GetType() == 88) )
    {

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
      
      
      if(ElecMod->Nb_TDC>0)
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
			      //hVME2_TDC[vme_chn]->Fill(value);
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
      }
      //===========

      //===========
    case 50:

      if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12) )
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

void FRS_Detector_System::FRS_Sort(){
    
    
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

void FRS_Detector_System::FRS_Calib(){
    
      //----- from here added by YKT 23.05 --------//   
  /////// 
  ///////  scaler_time_count,  scaler_spill_count,  scaler_time_check_last,   scaler_spill_check_last,  scaler_increase_event[64]     UInt_t
  ///////  check_increase_time[i]   check_increase_spill[i],   scaler_last_event[i]    UInt_t 

  // special channels for normalization
  int scaler_channel_spill_all[2] = {8, 5}; double normalization_factor_spill_all[2] = {1.0,1.0}; //   YKT 23.05
  //int scaler_channel_time_all[2]  = {36,1}; double normalization_factor_time_all[2] = {10.0,100.}; //  36 for 10 Hz clock
  int scaler_channel_time_all[2]  = {36,1}; double normalization_factor_time_all[2] = {1.,100.}; //  36 for 10 Hz clock (normalization 100ms)
  // int scaler_channel_time_all[2]  = {38,1}; double normalization_factor_time_all[2] = {100000.0,100.}; //38 for 100kHz clock
  
  scaler_channel_10khz = 25; //YT 20Jun.
  Long64_t nmax_V830 = 0x100000000;

  //printf("Event_flag_test=0x%08x\n", EventFlag);

  int index_flag = -1;
  if(EventFlag==0x100)
    index_flag=0;
  /*else if(EventFlag==0x200)
    index_flag=1;
  else if(EventFlag==0x300)
  index_flag=1;*/
  else
    std::cout<<"E> EventFlag unknown !"<<EventFlag<<std::endl;


  int scaler_channel_spill = scaler_channel_spill_all[index_flag];
  double normalization_factor_spill = normalization_factor_spill_all[index_flag]; //   YKT 23.05
  int scaler_channel_time  = scaler_channel_time_all[index_flag];
  double normalization_factor_time = normalization_factor_time_all[index_flag] ; //  YKT 23.05 

  //UInt_t tempCurrentTime = index_flag == 0 ? sc_long[scaler_channel_time] : sc_long2[scaler_channel_time];
  UInt_t tempCurrentTime = sc_long[scaler_channel_time];
  
  //----initialize values defined in this file---
  if(1==check_first_event[index_flag]) // if first event has 0 (for all channels) >> use next event as a first event 
    {
      //      std::cout<<"Init Scaler :"<<index_flag<<" "<<EventFlag<<" | "<<check_first_event[index_flag]<<" "<<tempCurrentTime<<"\n";
      if(0!=tempCurrentTime)
	{
	  //std::cout<<" +--> set initial values ";
	  scaler_time_count[index_flag]  = 0; //UInt_t
	  scaler_spill_count[index_flag] = 0; //UInt_t
	  scaler_time_check_last[index_flag] = 0;//UInt_t
	  scaler_spill_check_last[index_flag] = 0;//UInt_t
	  for(size_t i=0; i<64; i++) //changed from 32 to 64 (10.07.2018)
	    {
	      check_increase_time[i]   =0;//UInt_t
	      check_increase_spill[i]  =0;//UInt_t
	      scaler_increase_event[i] =0;//UInt_t
	      scaler_last_event[i] = static_cast<Long64_t>(sc_long[i]);//UInt_t
	    }
	  /*
	  for(size_t i=32;i<64;++i)
	    {
	      check_increase_time[i]   =0;//UInt_t
	      check_increase_spill[i]  =0;//UInt_t
	      scaler_increase_event[i] =0;//UInt_t
	      scaler_last_event[i] = static_cast<Long64_t>(sc_long2[i-32]);//UInt_t
	    }
	  */

	  //std::cout<<" done !\n";
	}
    }
  for(int i=0; i<64; i++)
    scaler_increase_event[i]=0;  
  
  //------------ scaler_increase_event[i] and scaler_last_event[i] -----------
  //////
  // sometimes V830 data from sort is empty (all 0)
  // in such case, we skip updating { scaler_increase_event[i] and scaler_last_event[i] }
  if(0!=tempCurrentTime)
    {
      if(0==check_first_event[index_flag])
	{
	  if(index_flag==0)
	    {
	      for(size_t i=0; i<64; i++) //changed from 32 to 64 (10.07.2018)
		{
		  if(static_cast<Long64_t>(sc_long[i]) >= scaler_last_event[i])
		    scaler_increase_event[i] = sc_long[i] - scaler_last_event[i];
		  else
		    {
		      //printf("sc_long[i], scaler_last_event[i]:%d %d\n", sc_long[i] , scaler_last_event[i]);
		      scaler_increase_event[i] =  sc_long[i]  + nmax_V830 - scaler_last_event[i];
		    }
		  scaler_last_event[i]     = sc_long[i];
		}
	    }
	  /*
	  if(index_flag==1)
	    {
	      for(size_t i=32; i<64; i++)
		{
		  if(static_cast<Long64_t>(sc_long2[i-32]) >= scaler_last_event[i])
		    scaler_increase_event[i] = sc_long2[i-32] - scaler_last_event[i];
		  else
		    {
		      //printf("sc_long[i], scaler_last_event[i]:%d %d\n", sc_long[i] , scaler_last_event[i]);
		      scaler_increase_event[i] =  sc_long2[i-32]  + nmax_V830 - scaler_last_event[i];
		    }
		  scaler_last_event[i]     = sc_long2[i-32];
		}	  
		}*/
	}
    }
  //-----switch off initial event check---
  if(1==check_first_event[index_flag] && (0!=tempCurrentTime)) // if first event has 0 (for all channels) >> use next event as a first event
    { 
      check_first_event[index_flag] = 0;
    }


  // add {increase from last event} to the counters.
  if(0!=tempCurrentTime)
    {
      for(int i=0; i<64; i++)
	{
	  check_increase_spill[i] += scaler_increase_event[i];  
	  check_increase_time[i]  += scaler_increase_event[i];
	}
    }
 
  // integrated count from the beginning
  if(0!=tempCurrentTime)
    {
      scaler_time_count[index_flag]  += scaler_increase_event[scaler_channel_time]; //
      scaler_spill_count[index_flag] += scaler_increase_event[scaler_channel_spill];//
    }
  
  int scaler_time_check  = scaler_time_count[index_flag]/((int)normalization_factor_time);
  int scaler_spill_check = scaler_spill_count[index_flag];

  //  printf("scaler_time_count = %d, sc_long[4]=%d, sc_long[3]=%d, \n",scaler_time_count, sc_long[4], sc_long[3]);
 

  // when scaler_time_check is increased
  if( 0<(scaler_time_check - scaler_time_check_last[index_flag]) )
    {
      //   printf("scaler_time_check = %d, scaler_time_check_last = %d \n",scaler_time_check,scaler_time_check_last);
      //if(10<(scaler_time_check - scaler_time_check_last[index_flag])) //10 Hz clock
      if(1<(scaler_time_check - scaler_time_check_last[index_flag])) //10 Hz clock //for normalization 100ms
      //if(100000<(scaler_time_check - scaler_time_check_last[index_flag])) //100 kHz clock
	{
	  //printf("scaler_time_check - scaler_time_check_last = %d ...\n",);
	  std::cout<<"EventFlag"<<EventFlag<<"|"<<index_flag<<"scaler_time_check - scaler_time_check_last = "<<(scaler_time_check - scaler_time_check_last[index_flag])<<"...\n";
	} 
      for(int i=0; i<64; i++)
	{
	  int x_bin = (scaler_time_check % 3000);
	  int x_bin_short = (scaler_time_check % 300);
	  int y_set;
	  if(check_increase_time[i]>0)
	    {
	      y_set =  (int)(normalization_factor_time*((Float_t)( check_increase_time[i] ))/((Float_t)( check_increase_time[scaler_channel_time] ))); 
	    }
	  else
	    {
	      y_set = 0;
	    }
	  /*hSCALER_TIME[i]  -> SetBinContent(1+x_bin,y_set);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+100)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+101)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+102)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+103)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+104)%3000,0);
	  hSCALER_TIME[i]  -> SetBinContent(1+(x_bin+105)%3000,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+x_bin_short,y_set);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+21)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+22)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+23)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+24)%300,0);
	  hSCALER_TIME_SHORT[i]  -> SetBinContent(1+(x_bin_short+25)%300,0);*/
	}
      for(int i=0; i<64; i++)
	{
	  check_increase_time[i]=0;
	}//reset
      scaler_time_check_last[index_flag] = scaler_time_check;  
    }

  // when scaler_time_check is increased
  if( 0<(scaler_spill_check - scaler_spill_check_last[index_flag]) )
    {
      if(10<(scaler_spill_check - scaler_spill_check_last[index_flag]))
	{
	  //printf("scaler_spill_check - scaler_spill_check_last = %d ...\n",(scaler_spill_check - scaler_spill_check_last));
	  std::cout<<"EventFlag"<<EventFlag<<"|"<<index_flag<<"scaler_spill_check - scaler_spill_check_last = "<<(scaler_spill_check - scaler_spill_check_last[index_flag])<<"...\n";
	}
      for(int i=0; i<64; i++)
	{
	  int x_bin = (scaler_spill_check % 300);
	  int x_bin_short = (scaler_spill_check % 30);
	  int y_set;
	  if(check_increase_spill[i]>0)
	    {
	      y_set =  (int)(normalization_factor_spill*((Float_t)( check_increase_spill[i] ))/((Float_t)( check_increase_spill[scaler_channel_spill] )));
	    }
	  else
	    {
	      y_set = 0;
	    }
	  /*hSCALER_SPILL[i]  -> SetBinContent(1+x_bin,y_set);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+10)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+11)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+12)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+13)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+14)%300,0);
	  hSCALER_SPILL[i]  -> SetBinContent(1+(x_bin+15)%300,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+x_bin_short,y_set);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+1)%30,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+2)%30,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+3)%30,0);
	  hSCALER_SPILL_SHORT[i]  -> SetBinContent(1+(x_bin_short+4)%30,0);*/
	}
      //
      if(index_flag==0)
	{
	  check_total_sc21 += check_increase_spill[7];
	  check_total_sc41 += check_increase_spill[8];
	  check_total_seetram += check_increase_spill[10];
      //printf("Total SC41 = %d,  Total SC21 = %d, Total SEETRAM = %d \n",check_total_sc41,check_total_sc21,check_total_seetram);
	}
      if(index_flag==1)
	{
	  check_total_mrtof_start += check_increase_spill[38];
	  check_total_mrtof_stop += check_increase_spill[39];
	    
	}
      for(int i=0; i<64; i++)
	{
	  check_increase_spill[i]=0;
	}//reset                                                                                                                                                       
      scaler_spill_check_last[index_flag] = scaler_spill_check;
      //           
    }
  //----- up to here added by YKT 23.05 --------// 
   
  UInt_t first[64];
  //  Int_t  first[64]; 

  if (fbFirstEvent)
    { //
      for (int i=0;i<64;i++)  //changed here from 32 to 64 (10.07.2018)
	{
	  //std::cout <<"In first event loop"<<std::endl ; 
	  scaler_save[i] = sc_long[i];
	  //		 std::cout <<"  "<<scaler_save[i] <<"  "<<sc_long[i]<<std::endl ; 
	  first[i]=sc_long[i];
	}
      /*
      for (int i=32;i<64;i++)
	{
	  scaler_save[i] = sc_long2[i-32];
	  first[i]=sc_long2[i-32];
	}
      */

      //    std::cout <<"1Hz  "<<sc_long[3]<<std::endl;
      //    std::cout <<"10Hz "<<sc_long[4]<<std::endl;  

      if(sc_long[35]!=0)
	{     
	  firstsec = sc_long[35]; // TIME //
	  firsttenthsec = sc_long[36];
	  firsthundrethsec = sc_long[38];	//mik
	  firstcycle = sc_long[34];
	  firstseetram= sc_long[2]; // new SE01
	  //      firstseetram= sc_long[9]; // old SE01

	  fbFirstEvent = kFALSE; 
	}
    }


  //  Int_t mon_inc[64];

  Double_t over_scale=  4000000000.;
  //  printf("%15f",over_scale);

  // for 1st module
  for (int i=0;i<32;i++)
    if(0!=sc_long[scaler_channel_time])
      {
	Int_t overload = 0; 
	//  if(sc_long[i]!=0){            
	if ( scaler_save[i] > static_cast<Long64_t>(sc_long[i]))
	  {
	    if (trigger==12 || trigger==13)
	      continue ;

	    //std::cout <<"Hey I got overloaded !!! channel "<<i<<std::endl ; 
	    //std::cout <<scaler_save[i] <<"  "<<sc_long[i]<<std::endl ;
	    //std::cout <<"trigger is : "<<trigger<<std::endl ; 
	    //scaler_save[i] = scaler_save[i] - 4294967295;
	    mon_inc[i] = static_cast<Long64_t>(sc_long[i])+4294967295 - scaler_save[i]; //
	    //scaler_save[i] = (Long64_t)sc_long[i]);
	    overload = 1;
	  }
	else
	  mon_inc[i] = static_cast<Long64_t>(sc_long[i]) - scaler_save[i]; //
	 
	scaler_save[i] = static_cast<Long64_t>(sc_long[i]);
	//
	//if(overload != 0)
	//std::cout<<"case overload"<<std::endl;
	if (mon_inc[i]<0)
	  {
	    //std::cout <<"  "<<i<<"  "<<mon_inc[i]<<"  "<<sc_long[i]<<"  "<<scaler_save[i]<<" | "<<overload<<std::endl ; 
	    std::cout <<"!> mon_inc["<<i<<"] -> WRONG"<<std::endl ;
	  } 


	/* save the current value so that it can be used next time around... */
	//if(mon_inc[i]>4000000000)mon_inc[i]=0; 
	//  if(mon_inc[i]>over_scale)mon_inc[i]=0; 
      
      } 

  // for 2nd module
  for (int i=32;i<64;i++)
    if(0!=sc_long[scaler_channel_time])
      {
	Int_t overload = 0;
	//if (scaler_save[i] > sc_long2[i-32])
	if (scaler_save[i] > sc_long[i])
	  { 
	    //scaler_save[i] = scaler_save[i] - 4294967295;
	    //mon_inc[i] = static_cast<Long64_t>(sc_long2[i-32]) + 4294967295 - scaler_save[i];
	    mon_inc[i] = static_cast<Long64_t>(sc_long[i]) + 4294967295 - scaler_save[i];
	    overload =1;
	  }
	else
	  //mon_inc[i] = sc_long2[i-32] - scaler_save[i];
	  mon_inc[i] = sc_long[i] - scaler_save[i];
	   
	
	if(mon_inc[i] > over_scale)
	  mon_inc[i]=0;

	//save the current value so that it can be used next time around... 
	//scaler_save[i] = sc_long2[i-32];
	scaler_save[i] = sc_long[i];

      
      }

  //Store scalers for later
  freeTrig=mon_inc[0];
  acptTrig=mon_inc[1];
  
  
  if(sc_long[35]!=0)
    {
      if(sc_long[35]<firstsec)
	++scalercycle_Sec;
      //firstsec = sc_long[3];	  
      
      if(sc_long[36]<firsttenthsec)
	++scalercycle_TenthSec;
	
      if(sc_long[38]<firsthundrethsec)		//mik
	++scalercycle_HundrethSec;
	
      if(sc_long[34]<firstcycle)
	++scalercycle_Cycle;


      seconds = sc_long[35] + 4294967295*scalercycle_Sec -firstsec;
      tenthsecs = sc_long[36] + 4294967295*scalercycle_TenthSec - firsttenthsec;
      hundrethsecs = sc_long[38] + 4294967295*scalercycle_HundrethSec - firsthundrethsec;  //mik
      extraction_cycle = sc_long[34] + 4294967295*scalercycle_Cycle - firstcycle;
    }

  //hTimeStamp->Fill((Int_t)((Double_t)timestamp / 5E7) % 3600);
  //hTimeStampZoom->Fill((Int_t)((Double_t)timestamp / 5E7) % 60);
  //hTimeSpill->Fill(timespill);
  //hMON_Pattern->Fill(pattern);

  for(int i=0;i<16;i++)
    {
      coin[i]=pattern>>i & 0x0001;
      //   printf("%d %d %d %d\n",pattern,i,coin[i],pattern>>i);
      //if(coin[i]==1)hMON_PatternH->Fill(i);
    }

  // printf("%d\n",pattern);

  // for 1st module
  for(int i=0;i<32;i++)
    {
      /*if (mon_inc[i]>=0)
	hMON_All->SetBinContent(i+1,(UInt_t) mon_inc[i]);*/
      
      /* per extraction cycle:  */
      //hMON_extr[i]->Fill(extraction_cycle, mon_inc[i]);
    
      /* per second:  */
      //hMON_scaler[i]->Fill(sc_long[i]);
    
      /* NOTE: spec$ is not the same as accu$!!!  */
      //hMON_diff[i]->Fill(seconds, mon_inc[i]);
      //    hMON_diff[i]->Fill(tenthsecs, mon_inc[i]);	//mik  //hundrethsecs
    }

  for(int i=32;i<64;i++)
    {      
      //hMON_All2->Fill(i-32,mon_inc[i]);
      
      /* per extraction cycle:  */
      //hMON_extr[i]->Fill(extraction_cycle, mon_inc[i]);
      
      /* per second:  */
      //hMON_scaler[i]->Fill(sc_long2[i-32]);
      //hMON_scaler[i]->Fill(sc_long[i]);
      
      /* NOTE: spec$ is not the same as accu$!!!  */
     // hMON_diff[i]->Fill(seconds, mon_inc[i]);
    } 

  
  /**  for SeeTram calibration purposes:   **/

  /* All spectra defined per tenth of second:  */
  /*  total number of scaler counts:  */
  //  hMON_seecalA[0]->Fill(tenthsecs, sc_long[0]);
  //  hMON_seecalA[1]->Fill(tenthsecs, sc_long[1]);
  //  hMON_seecalA[2]->Fill(tenthsecs, sc_long[9]);
  //  hMON_seecalA[3]->Fill(tenthsecs, sc_long[5]);
  //  hMON_seecalA[4]->Fill(tenthsecs, sc_long[10]);
  //  hMON_seecalA[5]->Fill(tenthsecs, sc_long[21]);
  //  hMON_seecalA[6]->Fill(tenthsecs, sc_long[11]);
  //  hMON_seecalA[7]->Fill(tenthsecs, sc_long[6]);

  /*  differential number of scaler counts:  */
  //  hMON_seecalB[0]->Fill(tenthsecs, mon_inc[0]);
  //  hMON_seecalB[1]->Fill(tenthsecs, mon_inc[1]);
  //  hMON_seecalB[2]->Fill(tenthsecs, mon_inc[9]);
  //  hMON_seecalB[3]->Fill(tenthsecs, mon_inc[5]);
  //  hMON_seecalB[4]->Fill(tenthsecs, mon_inc[10]);
  //  hMON_seecalB[5]->Fill(tenthsecs, mon_inc[21]);
  //  hMON_seecalB[6]->Fill(tenthsecs, mon_inc[11]);
  //  hMON_seecalB[7]->Fill(tenthsecs, mon_inc[6]);

  //10HzClock = hMON_scaler[4]->Integral();

  /*   Naming conventions:  index     detector   focus #                     */
  /*                         1         MW11        1                         */
  /*                         2         MW21        2                         */
  /*                         3         MW22                                  */
  /*                         4         MW31        3                         */
  /*                         5         MW41                                  */
  /*                         6         MW42                                  */
  /*                         7         MW51                                  */
  /*                         8         MW61                                  */
   /*                        9         MW71                                  */
  /*                         10        MW81                                  */
  /*                         11        MW82                                  */
  /*                         12        MWB1                                  */
  /*                         13        MWB2                                  */
  /////////////////////////////////////////////////////////////////////////////
  int max_index = 13; //upto MWB2 (09.07.2018) /*  up to S3 */

  for(int i=0;i<max_index;i++)
    {	
      /***************/
      /* Raw Spectra */
      /***************/
      //hMW_AN[i]->Fill(mw_an[i]);
      //hMW_XL[i]->Fill(mw_xl[i]);
      //hMW_XR[i]->Fill(mw_xr[i]);
      //hMW_YU[i]->Fill(mw_yu[i]);
      //hMW_YD[i]->Fill(mw_yd[i]);
      
      /********************************************/
      /* Calculate the sum spectra and conditions */
      /********************************************/
      
      /* better test first existence of xl, xr, an before filling of xsum */
      
      if(mw_an[i] && mw_xl[i] && mw_xr[i])
	{
	  // if(mw_xl[i]&&mw_xr[i]) {
	  mw_xsum[i] = 1000+(mw_xl[i] - mw_an[i]) + (mw_xr[i] - mw_an[i]);
      
	  // mw_xsum[i] = (mw_xl[i]) + (mw_xr[i]); //when an doesn't work
	  //hMW_XSUM[i]->Fill(mw_xsum[i]);
	}

      b_mw_xsum[i] = cMW_XSUM[i]->Test(mw_xsum[i]);
      
      
      /* better test first existence of yu, yd, an before filling of ysum */
      
      if(mw_an[i] && mw_yu[i] && mw_yd[i])
	{
	   
	  //if(mw_yu[i]&&mw_yd[i]) {
	  mw_ysum[i] = 1000+(mw_yu[i] - mw_an[i]) + (mw_yd[i] - mw_an[i]);	  
	  
	  //mw_ysum[i] = (mw_yu[i]) + (mw_yd[i]); //when an doesn't work			
	  //hMW_YSUM[i]->Fill(mw_ysum[i]);          
	}
      
      b_mw_ysum[i] = cMW_YSUM[i]->Test(mw_ysum[i]);
      
    
      /*******************************************************************/
      /* If the signals in x and y are valid, calculate position spectra */
      /*******************************************************************/
      
      if (b_mw_xsum[i])
	{
	  //      Int_t r_x = mw_xl[i] - mw_xr[i];
	  Float_t r_x = mw_xl[i] *  mw->gain_tdc[1][i] - mw_xr[i] *  mw->gain_tdc[2][i]; //14.09.05 CN+AM
	  mw_x[i] = mw->x_factor[i] * r_x + mw->x_offset[i];
	  //hMW_X[i]->Fill(mw_x[i]);
	}
    
      if (b_mw_ysum[i])
	{
	  //      Int_t r_y = mw_yd[i] - mw_yu[i];
	  Float_t r_y = mw_yd[i] *  mw->gain_tdc[4][i] - mw_yu[i] *  mw->gain_tdc[3][i]; //14.09.05 CN+AM
	  mw_y[i] = mw->y_factor[i] * r_y + mw->y_offset[i];
	  //hMW_Y[i]->Fill(mw_y[i]);
	}

    
      if(b_mw_xsum[i] && b_mw_ysum[i])
	{
	  //hMW_XY[i]->Fill(mw_x[i], mw_y[i]);  
	}
      
    } // for(int i=0;i<max_index;i++)


  /*********************************************/
  /* S2 Angle and transformed position spectra */
  /*********************************************/
  
  Float_t  dist_MW21_MW22  = frs->dist_MW22  - frs->dist_MW21;
  Float_t  dist_MW22_focS2 = frs->dist_focS2 - frs->dist_MW22;
  Float_t  dist_MW22_SC21  = frs->dist_SC21  - frs->dist_MW22;
  Float_t  dist_MW22_SC22  = frs->dist_SC22  - frs->dist_MW22;
  

  if (b_mw_xsum[1] && b_mw_xsum[2])
    {
      /* MW21 and MW22 X okay */
      /*  X angle at S2  [mrad]:  */
      angle_x_s2 = (mw_x[2]-mw_x[1])/dist_MW21_MW22*1000.;
      //hMW_xAngS2->Fill(angle_x_s2);

      /*  X at nominal S2 focus:  */
      focx_s2 = mw_x[2] + dist_MW22_focS2 * angle_x_s2/1000.;
      //hMW_xFocS2->Fill(focx_s2);
      
      /*  X at SC21 position:    */
      sc21_x = mw_x[2] + dist_MW22_SC21 * angle_x_s2/1000.;
      //hMW_SC21x->Fill(sc21_x);

      /*  X at SC22 position:    */
      sc22_x = mw_x[2] + dist_MW22_SC22 * angle_x_s2/1000.;
      //hMW_SC22x->Fill(sc22_x);
      

      /* 'real' z-position of S2 X focus (cm) */
      Float_t rh = (angle_x_s2 - angle_x_s2m);
      if(fabs(rh)>1e-4)
	{
	  z_x_s2 = ((focx_s2m - focx_s2)/rh)*100. + frs->dist_focS2/10.;  
	  //hMW_zxS2->Fill(z_x_s2);
	}
    
      /* keep values for next event */
      focx_s2m = focx_s2; 
      angle_x_s2m = angle_x_s2;
    }
  
  if (b_mw_ysum[1] && b_mw_ysum[2])
    {
      /* MW21 and MW22 Y okay */
      /*  Y angle at S2 [mrad]:   */
      angle_y_s2 = (mw_y[2] - mw_y[1])/dist_MW21_MW22*1000.;
      //hMW_yAngS2->Fill(angle_y_s2);
      
      /*  Y at nominal S2 focus:  */
      focy_s2 = mw_y[2] + dist_MW22_focS2 * angle_y_s2/1000.;
      //hMW_yFocS2->Fill(focy_s2);

      /* 'real' z-position of S2 Y focus (cm) */
      Float_t rh = (angle_y_s2 - angle_y_s2m);
      if(fabs(rh)>1.e-4)
	{
	  z_y_s2 = ((focy_s2m - focy_s2)/rh)*100. + frs->dist_focS2/10.;  
	  //hMW_zyS2->Fill(z_y_s2);
	}
      
      /* keep values for next event */
      focy_s2m = focy_s2; 
      angle_y_s2m = angle_y_s2; 
    }
  
  /*if (b_mw_ysum[1] && b_mw_ysum[2] && b_mw_xsum[1] && b_mw_xsum[2])
    hMW_FocS2->Fill(focx_s2,focy_s2);*/ 


//////////////////////////////////////////////////////////////////
///  ***********TPC Analysis*************************** //////////

 //================================
  // Nomenclature for TPCs
  //================================
  // TPCs at S2
  // TPC 1 = TPC 21 (in vaccum) [i=0]
  // TPC 2 = TPC 22 (in vaccum) [i=1]
  // TPC 3 = TPC 23 (in air)    [i=2]
  // TPC 4 = TPC 24 (in air)    [i=3]

  // TPCs at S4
  // TPC 5 = TPC 41 (in air)    [i=4]
  // TPC 6 = TPC 42 (in air)    [i=5]

  //TPC at S3
  // TPC 7 = TPC 31             [i=6]
  

  for(int i=0;i<7;i++)
    {  
      /*if (bDrawHist)
	{
	  hTPC_L0[i]->Fill(tpc_l[i][0]);
	  hTPC_R0[i]->Fill(tpc_r[i][0]);
	  hTPC_L1[i]->Fill(tpc_l[i][1]);
	  hTPC_R1[i]->Fill(tpc_r[i][1]);
	  hTPC_LT0[i]->Fill(tpc_lt[i][0]);
	  hTPC_RT0[i]->Fill(tpc_rt[i][0]);
	  hTPC_LT1[i]->Fill(tpc_lt[i][1]);
	  hTPC_RT1[i]->Fill(tpc_rt[i][1]);
	  
	  hTPC_LTRT[i]->Fill(tpc_lt[i][0],tpc_rt[i][1]);
	}*/
      int count =0;
      Int_t r_y = 0;
      
      int countx =0;
      Int_t r_x0 = 0;
      Int_t r_x1 = 0;
      
      tpc_y[i] = 0;
      tpc_x[i] = 9999;

      for(int j=0;j<4;j++)
	{
	  /*if (bDrawHist)
	    {
	      hTPC_DT[i][j]->Fill(tpc_dt[i][j]);
	      hTPC_A[i][j]->Fill(tpc_a[i][j]);
	    }*/

	  //// calculate control sums
	  if(j < 2)
	    tpc_csum[i][j] = (tpc_lt[i][0] + tpc_rt[i][0]- 2*tpc_dt[i][j]);
	  else
	    tpc_csum[i][j] = (tpc_lt[i][1] + tpc_rt[i][1]- 2*tpc_dt[i][j]);


	  //      if((de_42l>230&&de_42l<450)||(de_42r>540&&de_42r<750)){
	  /*if (bDrawHist) 
	    hTPC_CSUM[i][j]->Fill(tpc_csum[i][j]);*/
	  b_tpc_csum[i][j] = cTPC_CSUM[i][j]->Test(tpc_csum[i][j]);
	  //      }
	  
	  if(tpc_lt[i][0]==0 && tpc_rt[i][0]==0 && j<2)
	    b_tpc_csum[i][j]=0;
      
	  if(tpc_lt[i][1]==0 && tpc_rt[i][1]==0 && j>1)
	    b_tpc_csum[i][j]=0;
      
	  if (b_tpc_csum[i][j])
	    {
	      r_y += tpc_dt[i][j];
	      tpc_y[i] += tpc->y_factor[i][j]*tpc_dt[i][j] + tpc->y_offset[i][j];
	      count++;
	    }
	}

      if (b_tpc_csum[i][0] || b_tpc_csum[i][1])
	{
	  r_x0 =  tpc_lt[i][0]-tpc_rt[i][0];
	  //          r_x0 =  tpc_lt[i][0];
	  
	  tpc_x[i]=tpc->x_factor[i][0]*r_x0 + tpc->x_offset[i][0];
	  countx++;
	}
      
      if (b_tpc_csum[i][2] || b_tpc_csum[i][3])
	{
	  r_x1 =  tpc_lt[i][1]-tpc_rt[i][1];
	  if (countx == 0)
	    tpc_x[i]=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  else
	    tpc_x[i]+=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  countx++;
	}

      b_tpc_xy[i] = kFALSE;  // HWE

      if (countx > 0)
	{
	  tpc_y[i] = tpc_y[i]/count;
	  tpc_x[i] = tpc_x[i]/countx;
	  //Int_t r_x=tpc_lt[i]-tpc_rt[i];
	  //tpc_x[i]=tpc->x_factor[i]*r_x + tpc->x_offset[i];
	  
	  //      if(r_x0<-40)
	  
	  /*if (bDrawHist)
	    {
	      hTPC_XRAW0[i]->Fill(r_x0);
	      hTPC_XRAW1[i]->Fill(r_x1);
	      hTPC_YRAW[i]->Fill(r_y/count); 
	      //	      if(coin[15]==1){
	      hTPC_X[i]->Fill(tpc_x[i]);
	      hTPC_Y[i]->Fill(tpc_y[i]);
	      hcTPC_XY[i]->Fill(tpc_x[i],tpc_y[i]);
	      //    	 }
	    }*/
	  b_tpc_xy[i] = kTRUE;
	}
      
      if(countx>1)
	{
	  float x0=tpc->x_factor[i][0]*r_x0 + tpc->x_offset[i][0];
	  float x1=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  /*if (bDrawHist)
	    hTPC_DELTAX[i]->Fill(x0-x1);*/
	}    
    }


  //*********************************************
  // *****Relative distances for tracking *******
  //*********************************************
  //Float_t dist_TPC1_TPC2 = 1.;         
  //distances at S2 for exp s388 measured 'by eye' (i.e. not very precise)! AE, 8.8.2012
  //Float_t dist_TPC4_target1 = 455.;  //8.8.12, s388: taget1 is first Si detector at S2 
  //using: TPC4->chamber= 150 mm, chamber->Si(1)= 305 mm
  //Float_t dist_TPC3_TPC4 = 610.; // exp S417+S411 Oct2014
  //Float_t dist_TPC3_focS2 = 2860 - 2013 ; //exp S411 Oct2014
  //Float_t dist_TPC3_focS2 = 0. ; //exp S417 Oct2014 focus on TPC21
  //Float_t dist_TPC5_TPC6 = 1595. - 385. ; // Oct.2014, exp s411 
  //Float_t dist_TPC6_focS4  = 2349. - 1595. ; // Oct.2014, exp s411
  //THIS SHOULD BE NOT HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Yhis was for Uranium--- see logbook page 6
  //Float_t dist_TPC6_target2  = 1015.; // 8.08.12, exp s388 target2= OTPC entrance (check distance)
  //Float_t dist_TPC4_target1 = 455.;  // 8.8.12, s388: taget1 is first Si detector at S2 
                                     // using: TPC4->chamber= 150 mm, chamber->Si(1)= 305 mm

  //==========================
  // Distances of TPCs at S2
  //==========================

  Float_t dist_TPC21_TPC22 = 1782.5 - 604.0; //check
  Float_t dist_TPC23_TPC24 = 1782.5 - 604.0; //check
  Float_t dist_TPC22_TPC24 = 1782.5 - 604.0; //check

  Float_t dist_TPC21_focS2 = 604.-2165.; //check
  Float_t dist_TPC22_focS2 = 604.-2165.; //check
  Float_t dist_TPC23_focS2 = 604.-2165.; //check


  //Float_t dist_TPC3_TPC4 = 1782.0- 604.; //old value for old tracking 
  //Float_t dist_TPC3_focS2 = 604.-2165. ; //old value for old tracking  

  //==================================
  //Distance of TPCs at S4
  //==================================
  
  Float_t dist_TPC41_TPC42 = 1525.0 - 385.0;  //rough check
  //Float_t dist_TPC41_TPC42 = 1660.0 - 360.0;  //check

  Float_t dist_TPC42_focS4 = 3300.0 - 1525.0; //check
  //Float_t dist_TPC42_focS4 = 3300.0 - 1660.0; //check

  
  //Float_t dist_TPC5_TPC6 = 2063.5 - 382.5 ; 
  //Float_t dist_TPC6_focS4  = 2200. - 2063.5 ;
  //Float_t dist_TPC6_focS4  = 4300. - 1660. ;//before 13Jun2016 17:10
  //Float_t dist_TPC6_focS4  = 2200. - 1660. ;//for S4 focus 2200 , changed at 13Jun2016 17:10 
  //Float_t dist_TPC6_focS4  = 2200. -1135.9 +300.0 - 1660. +100.; //short focus 11jul2016
  //Float_t dist_TPC6_focS4  = 2202 - 1660.; // tensor28 mode
  //Float_t dist_TPC6_focS4  = 3300 - 1660.; // long focus

  //=====================
  // Old tracking for S2 
  //=====================
  /*
  //Position S2 tracked with TPCs 3 and 4 for focus
  if (b_tpc_xy[2]&&b_tpc_xy[3])
    {
      tpc_angle_x_s2_foc = (tpc_x[3] - tpc_x[2])/dist_TPC3_TPC4*1000.;
      tpc_angle_y_s2_foc = (tpc_y[3] - tpc_y[2])/dist_TPC3_TPC4*1000.;
      tpc_x_s2_foc = -tpc_angle_x_s2_foc * dist_TPC3_focS2/1000. + tpc_x[2];
      tpc_y_s2_foc = -tpc_angle_y_s2_foc * dist_TPC3_focS2/1000. + tpc_y[2];
      
      if (bDrawHist)
	{
	  hTPC_X_S2->Fill(tpc_x_s2_foc);
	  hTPC_Y_S2->Fill(tpc_y_s2_foc);
	  hTPC_AX_S2->Fill(tpc_angle_x_s2_foc);
	  hTPC_AY_S2->Fill(tpc_angle_y_s2_foc);
	  hTPC_S2X_time->Fill(tpc_x_s2_foc, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun  
	  hTPC_S2A_time->Fill(tpc_angle_x_s2_foc, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun
	}

 
      //Position S2 tracked with TPCs 3 and 4 to first Si tracker in exp S388
     // tpc_x_s2_target1 = tpc_angle_x_s2_foc * dist_TPC4_target1/1000. + tpc_x[3];
     // tpc_y_s2_target1 = tpc_angle_y_s2_foc * dist_TPC4_target1/1000. + tpc_y[3];
      
     // if (bDrawHist)
	//{
	 // hTPC_X_S2_target1->Fill(tpc_x_s2_target1);
	 // hTPC_Y_S2_target1->Fill(tpc_y_s2_target1);
	 // hTPC_XY_S2_target1->Fill(tpc_x_s2_target1,tpc_y_s2_target1);
	 // }
    }
  else  tpc_x_s2_foc= -999 ;
  */

  //=================================
  // Tracking with TPC 21 and TPC 22 
  //=================================
  
  if (b_tpc_xy[0]&&b_tpc_xy[1])
    {
      tpc_angle_x_s2_foc_21_22 = (tpc_x[1] - tpc_x[0])/dist_TPC21_TPC22*1000.;
      tpc_angle_y_s2_foc_21_22 = (tpc_y[1] - tpc_y[0])/dist_TPC21_TPC22*1000.;
      tpc_x_s2_foc_21_22 = -tpc_angle_x_s2_foc_21_22 * dist_TPC21_focS2/1000. + tpc_x[0]; //check
      tpc_y_s2_foc_21_22 = -tpc_angle_y_s2_foc_21_22 * dist_TPC21_focS2/1000. + tpc_y[0]; //check
      
      /*if (bDrawHist)
	{
	  hTPC_X_S2_TPC_21_22->Fill(tpc_x_s2_foc_21_22);
	  hTPC_Y_S2_TPC_21_22->Fill(tpc_y_s2_foc_21_22);
	  hTPC_AX_S2_TPC_21_22->Fill(tpc_angle_x_s2_foc_21_22);
	  hTPC_AY_S2_TPC_21_22->Fill(tpc_angle_y_s2_foc_21_22);
	  hTPC_S2X_TPC21_22_time->Fill(tpc_x_s2_foc_21_22, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun  
	  hTPC_S2A_TPC21_22_time->Fill(tpc_angle_x_s2_foc_21_22, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun
	}*/
    }
  else  tpc_x_s2_foc_21_22= -999 ;


  //=================================
  // Tracking with TPC 23 and TPC 24 
  //=================================
  
  if (b_tpc_xy[2]&&b_tpc_xy[3])
    {
      tpc_angle_x_s2_foc_23_24 = (tpc_x[3] - tpc_x[2])/dist_TPC23_TPC24*1000.;
      tpc_angle_y_s2_foc_23_24 = (tpc_y[3] - tpc_y[2])/dist_TPC23_TPC24*1000.;
      tpc_x_s2_foc_23_24 = -tpc_angle_x_s2_foc_23_24 * dist_TPC23_focS2/1000. + tpc_x[2]; //check
      tpc_y_s2_foc_23_24 = -tpc_angle_y_s2_foc_23_24 * dist_TPC23_focS2/1000. + tpc_y[2]; //check
      
      /*if (bDrawHist)
	{
	  hTPC_X_S2_TPC_23_24->Fill(tpc_x_s2_foc_23_24);
	  hTPC_Y_S2_TPC_23_24->Fill(tpc_y_s2_foc_23_24);
	  hTPC_AX_S2_TPC_23_24->Fill(tpc_angle_x_s2_foc_23_24);
	  hTPC_AY_S2_TPC_23_24->Fill(tpc_angle_y_s2_foc_23_24);
	  hTPC_S2X_TPC23_24_time->Fill(tpc_x_s2_foc_23_24, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun  
	  hTPC_S2A_TPC23_24_time->Fill(tpc_angle_x_s2_foc_23_24, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun
	}*/
    }
  else  tpc_x_s2_foc_23_24= -999 ;


  //=================================
  // Tracking with TPC 22 and TPC 24 
  //=================================
  
  if (b_tpc_xy[1]&&b_tpc_xy[3])
    {
      tpc_angle_x_s2_foc_22_24 = (tpc_x[3] - tpc_x[1])/dist_TPC22_TPC24*1000.;
      tpc_angle_y_s2_foc_22_24 = (tpc_y[3] - tpc_y[1])/dist_TPC22_TPC24*1000.;
      tpc_x_s2_foc_22_24 = -tpc_angle_x_s2_foc_22_24 * dist_TPC22_focS2/1000. + tpc_x[1]; //check
      tpc_y_s2_foc_22_24 = -tpc_angle_y_s2_foc_22_24 * dist_TPC22_focS2/1000. + tpc_y[1]; //check
      
      /*if (bDrawHist)
	{
	  hTPC_X_S2_TPC_22_24->Fill(tpc_x_s2_foc_22_24);
	  hTPC_Y_S2_TPC_22_24->Fill(tpc_y_s2_foc_22_24);
	  hTPC_AX_S2_TPC_22_24->Fill(tpc_angle_x_s2_foc_22_24);
	  hTPC_AY_S2_TPC_22_24->Fill(tpc_angle_y_s2_foc_22_24);
	  hTPC_S2X_TPC22_24_time->Fill(tpc_x_s2_foc_22_24, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun  
	  hTPC_S2A_TPC22_24_time->Fill(tpc_angle_x_s2_foc_22_24, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun
	}*/
    }
  else  tpc_x_s2_foc_22_24= -999 ;
  
  
  //=====================================================
  // Tracking with TPC 41 and TPC 42 (TPC 5 and 6) at S4  
  //=====================================================
  if (b_tpc_xy[4]&&b_tpc_xy[5])
    {
      
      tpc_angle_x_s4 = (tpc_x[5] - tpc_x[4])/dist_TPC41_TPC42*1000.;
      tpc_angle_y_s4 = (tpc_y[5] - tpc_y[4])/dist_TPC41_TPC42*1000.;
      tpc_x_s4 = tpc_angle_x_s4 * dist_TPC42_focS4/1000. + tpc_x[5];
      tpc_y_s4 = tpc_angle_y_s4 * dist_TPC42_focS4/1000. + tpc_y[5];
      
      music1_x1 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa1)/1000. + tpc_x[5];
      music1_x2 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa2)/1000. + tpc_x[5];
      music1_x3 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa3)/1000. + tpc_x[5];
      music1_x4 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa4)/1000. + tpc_x[5];

      music1_y1 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa1)/1000. + tpc_y[5];
      music1_y2 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa2)/1000. + tpc_y[5];
      music1_y3 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa3)/1000. + tpc_y[5];
      music1_y4 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa4)/1000. + tpc_y[5];


      /*if (bDrawHist)
	{
	  hTPC_X_S4->Fill(tpc_x_s4);
	  hTPC_Y_S4->Fill(tpc_y_s4);
	  hTPC_AX_S4->Fill(tpc_angle_x_s4);
	  hTPC_AY_S4->Fill(tpc_angle_y_s4);
	  hTPC_XAX_S4->Fill(tpc_x_s4,tpc_angle_x_s4);
	  
	  //hTPC_XS4_AX_S2->Fill(tpc_x_s4,tpc_angle_x_s2_foc);

	  hTPC_XS4_AX_TPC21_22_S2->Fill(tpc_x_s4,tpc_angle_x_s2_foc_21_22);
	  hTPC_XS4_AX_TPC23_24_S2->Fill(tpc_x_s4,tpc_angle_x_s2_foc_23_24);
	  hTPC_XS4_AX_TPC22_24_S2->Fill(tpc_x_s4,tpc_angle_x_s2_foc_22_24);

	  
	}*/
      
      //S4 entrance of OTPC tracked with TPCs 5 and 6 for s388    
      //tpc_x_s4_target2 = tpc_angle_x_s4 * dist_TPC6_target2/1000. + tpc_x[5];
      //tpc_y_s4_target2 = tpc_angle_y_s4 * dist_TPC6_target2/1000. + tpc_y[5];
    
     
      /*if (bDrawHist)
	{
	  //hTPC_X_S4_target2->Fill(tpc_x_s4_target2);
	  //hTPC_Y_S4_target2->Fill(tpc_y_s4_target2);
	  //hTPC_XY_S4_target2->Fill(tpc_x_s4_target2,tpc_y_s4_target2);
	  //
	  hTPC_S4X_time->Fill(tpc_x_s4, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun
	  hTPC_S4A_time->Fill(tpc_angle_x_s4, 0.1*((double)(check_increase_spill[scaler_channel_10khz])));//YK 20 Jun     
	}*/
    }
  
  
  // tracking to musics and scis (S323,410 exp.)
  //Float_t dist_TPC3_SC21  = 1375.5;
  //Float_t dist_TPC6_SC41  = 510.; //8.09.11 //check AEv

  // X position at SC21 position:
  // SC21 is before TPC3 and TPC4
  //tpc_sc21_x = tpc_x[2] - dist_TPC3_SC21 * tpc_angle_x_s2_foc/1000.;
  //hTPC_SC21x->Fill(tpc_sc21_x);
 

  // Y position at SC21
  //tpc_sc21_y = tpc_y[2] - dist_TPC3_SC21 * tpc_angle_y_s2_foc/1000.;
  //hTPC_SC21y->Fill(tpc_sc21_y);

  
  // X position at SC41 
  // SC41 is after TPC6
  //tpc_sc41_x  = tpc_x[5] + dist_TPC6_SC41 * tpc_angle_x_s4 / 1000.;
  //hTPC_SC41x->Fill(tpc_sc41_x);
  
  // Y position at SC41
  //tpc_sc41_y = tpc_y[5] + dist_TPC6_SC41 * tpc_angle_y_s4 /1000.;
  //hTPC_SC41y->Fill(tpc_sc41_y);


//////////////////////////////////////////////////////////////////
///  ***********Si Analysis**************************** //////////


  // Si alpha
  si_e1 = (si->si_factor1*si_adc1)+si->si_offset1;
  si_e2 = (si->si_factor2*si_adc2)+si->si_offset2;
  si_e3 = (si->si_factor3*si_adc3)+si->si_offset3;
  si_e4 = (si->si_factor4*si_adc4)+si->si_offset4;
  si_e5 = (si->si_factor5*si_adc5)+si->si_offset5;

  // fill histogram for first si detector trigger ==3

  /*hsi_e1->Fill(si_e1);
  if(trigger==3)
    hsi_e1Trig->Fill(si_e1);*/

  // fill histogram for second si detector trigger ==4
  //if(trigger==3)
  /*hsi_e2->Fill(si_e2);
  if(trigger==4)
    hsi_e2Trig->Fill(si_e2);*/
  // fill histogram for first si detector trigger ==5
  //if(trigger==2)
  /*hsi_e3->Fill(si_e3);
  if(trigger==5)
    hsi_e3Trig->Fill(si_e3);*/
  // fill histogram for first si detector trigger ==6
  //if(trigger==2)
  /*hsi_e4->Fill(si_e4);
  if(trigger==6)
    hsi_e4Trig->Fill(si_e4);*/


  //========================
  //Active Stopper Analysis
  //========================
  
  dssd_xmaxindex=-10;
  dssd_xmaxenergy=1000;

  
  //for(int i=0;i<16;i++){
  //dssd_e[15-i] = (si->dssd_factor[15-i]*(dssd_adc[i]+gRandom->Rndm()-0.5))+si->dssd_offset[15-i];	  	  }
  

  for(int i=0;i<16;i++){
   //dssd_e[i] = dssd_adc[i];
     dssd_e[i] = (si->dssd_factor[i]*dssd_adc[i])+si->dssd_offset[i];
  }


  for (int i=0 ; i<16 ; i++)
      {
        if (dssd_xmaxenergy<dssd_e[i])
           {
	     dssd_xmaxenergy=dssd_e[i];
	     dssd_xmaxindex= i;
	   }
      }
  
	
  dssd_ymaxindex=-10;
  dssd_ymaxenergy=1000;
  
  
  //for(int i=16;i<24;i++){
  //dssd_e[i+8] = (si->dssd_factor[i+8]*(dssd_adc[i]+gRandom->Rndm()-0.5))+si->dssd_offset[i+8];
  //}
	
  //for(int i=24;i<32;i++){
  //dssd_e[i-8] = (si->dssd_factor[i-8]*(dssd_adc[i]+gRandom->Rndm()-0.5))+si->dssd_offset[i-8];
  //}	
   

   for(int i=16;i<32;i++){
      // dssd_e[i] = dssd_adc[i];
	 dssd_e[i] = (si->dssd_factor[i]*dssd_adc[i])+si->dssd_offset[i];
   }


   for (int i=16 ; i<32 ; i++){
     if(dssd_ymaxenergy<dssd_e[i])
       {
	 dssd_ymaxenergy=dssd_e[i];
	 dssd_ymaxindex= i-16;
       }	

   }
  
}

void FRS_Detector_System::FRS_Anal(){
    
  music1_anodes_cnt = 0;  
  music2_anodes_cnt = 0;  
  music3_anodes_cnt = 0;


   // Munich MUSIC 
  
  for (int i=0;i<8;i++)
    {
      /* 8 anodes of TUM MUSIC */		
      /****** first MUSIC ***** threshold changed to 5, 9/8/2012 **/
      if ( music_e1[i] > 5)
	{ 
	  /*if(bDrawHist) 
	    hMUSIC1_E[i]->Fill(music_e1[i]);*/

	  music_b_e1[i] = cMusic1_E[i]->Test(music_e1[i]);

	  if (music_b_e1[i])
	    music1_anodes_cnt++;
	}

        if (music_t1[i] > 0)
	{ 
	  /*if(bDrawHist) 
	    hMUSIC1_T[i]->Fill(music_t1[i]);  */   
	  music_b_t1[i] = cMusic1_T[i]->Test(music_t1[i]);
	}

       
      //hMUSIC1_dE1dE2->Fill(music_e1[0],music_e1[1]);

      //       /****** second MUSIC *****/
      
      if ( music_e2[i] > 5)
	{ 
	  /*if(bDrawHist) 
	    hMUSIC2_E[i]->Fill(music_e2[i]);*/
	  music_b_e2[i] = cMusic2_E[i]->Test(music_e2[i]);
	  if (music_b_e2[i]) music2_anodes_cnt++;
	}

      if (music_t2[i] > 0)
	{ 
	  /*if(bDrawHist) 
	    hMUSIC2_T[i]->Fill(music_t2[i]);   */  
	  music_b_t2[i] = cMusic2_T[i]->Test(music_t2[i]);
	}
    }

  for (int i=0;i<4;i++)
    {
      /* 4 anodes of MUSIC OLD */		
      /****** third MUSIC *****/
      if ( music_e3[i] > 0)
	{ 
	  /*if(bDrawHist) 
	    hMUSIC3_E[i]->Fill(music_e3[i]);*/

	  music_b_e3[i] = cMusic3_E[i]->Test(music_e3[i]);
	  if (music_b_e3[i])
	    music3_anodes_cnt++;
	}
      if (music_t3[i] > 0)
	{ 
	  /*if(bDrawHist) 
	    hMUSIC3_T[i]->Fill(music_t3[i]);  */   
	  music_b_t3[i] = cMusic3_T[i]->Test(music_t3[i]);
	}
    }
  
  // calculate truncated dE from 8 anodes, Munich MUSIC 
  
  if (music1_anodes_cnt == 8)
    {
      /* "quick-n-dirty solution according to Klaus:   */
      //      Float_t r1 = (music_e1[0] - music->e1_off[0])*(music_e1[1] - music->e1_off[1]);
      //      Float_t r2 = (music_e1[2] - music->e1_off[2])*(music_e1[3] - music->e1_off[3]);
      //      Float_t r3 = (music_e1[4] - music->e1_off[4])*(music_e1[5] - music->e1_off[5]);
      //      Float_t r4 = (music_e1[6] - music->e1_off[6])*(music_e1[7] - music->e1_off[7]);

      Float_t r1 = ((music_e1[0])*music->e1_gain[0] + music->e1_off[0])*((music_e1[1])*music->e1_gain[1] + music->e1_off[1]);
      Float_t r2 = ((music_e1[2])*music->e1_gain[2] + music->e1_off[2])*((music_e1[3])*music->e1_gain[3] + music->e1_off[3]);
      Float_t r3 = ((music_e1[4])*music->e1_gain[4] + music->e1_off[4])*((music_e1[5])*music->e1_gain[5] + music->e1_off[5]);
      Float_t r4 = ((music_e1[6])*music->e1_gain[6] + music->e1_off[6])*((music_e1[7])*music->e1_gain[7] + music->e1_off[7]);

      if ( (r1 > 0) && (r2 > 0) && (r3 > 0) && (r4 > 0) )
	{
	  b_de1 = kTRUE;
	  de[0] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );
	  de_cor[0] = de[0];
	  /*if(bDrawHist) 
	    hMUSIC1_dE->Fill(de[0]);*/
	}  
    }

       
  if (music2_anodes_cnt == 8)
    {
      
      Float_t r1 = ((music_e2[0])*music->e2_gain[0] + music->e2_off[0])*((music_e2[1])*music->e2_gain[1] + music->e2_off[1]);
      Float_t r2 = ((music_e2[2])*music->e2_gain[2] + music->e2_off[2])*((music_e2[3])*music->e2_gain[3] + music->e2_off[3]);
      Float_t r3 = ((music_e2[4])*music->e2_gain[4] + music->e2_off[4])*((music_e2[5])*music->e2_gain[5] + music->e2_off[5]);
      Float_t r4 = ((music_e2[6])*music->e2_gain[6] + music->e2_off[6])*((music_e2[7])*music->e2_gain[7] + music->e2_off[7]);

      if ( (r1 > 0) && (r2 > 0) && (r3 > 0) && (r4 > 0) )
	{
	  b_de2 = kTRUE;
	  de[1] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );
	  de_cor[1] = de[1];
	  /*if(bDrawHist) 
	    hMUSIC2_dE->Fill(de[1]);*/
	}  
    }
       

  if (music3_anodes_cnt == 4)
    {         // OLD MUSIC
      
      Float_t r1 = ((music_e3[0])*music->e3_gain[0] + music->e3_off[0])*((music_e3[1])*music->e3_gain[1] + music->e3_off[1]);
      Float_t r2 = ((music_e3[2])*music->e3_gain[2] + music->e3_off[2])*((music_e3[3])*music->e3_gain[3] + music->e3_off[3]);
      //Float_t r3 = ((music_e3[4])*music->e3_gain[4] + music->e3_off[4])*((music_e3[5])*music->e3_gain[5] + music->e3_off[5]);  //added on 22.05.2018 SB
      //Float_t r4 = ((music_e3[6])*music->e3_gain[6] + music->e3_off[6])*((music_e3[7])*music->e3_gain[7] + music->e3_off[7]);  //added on 22.05.2018 SB
      
      if ( (r1 > 0) && (r2 > 0) )
	{
	  b_de3 = kTRUE;
	  de[2] = sqrt( sqrt(r1) * sqrt(r2) ) ;  // corrrected JSW 19.03.2010
	  //de[2] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );   //changed on 22.05.2018 SB
	  de_cor[2] = de[2];
	  /*if(bDrawHist)
	    hMUSIC3_dE->Fill(de[2]);*/
	} 

      if (music_b_t3[0] && music_b_t3[1] && music_b_t3[2] && music_b_t3[3]) 
	b_dt3 = kTRUE;
    


	//hMUSIC1_MUSIC2->Fill(de[0],de[1]);

      /* Position (X) correction by TPC */       //TO DO!!!
    


      //if(!music->b_selfcorr1 && b_de3) {
      //if(b_mw_xsum[4] && b_mw_xsum[5] && b_de3) {
      if(b_de3 && b_tpc_xy[4]&&b_tpc_xy[5])
	{
	  Float_t p1 = music1_x1;
	  Float_t p2 = music1_x2;
	  Float_t p3 = music1_x3;
	  Float_t p4 = music1_x4;

	  //Float_t p5 = music1_x5; //added on 22.05.2018 SB
	  //Float_t p6 = music1_x6; //added on 22.05.2018 SB
	  //Float_t p7 = music1_x7; //added on 22.05.2018 SB
	  //Float_t p8 = music1_x8; //added on 22.05.2018 SB
	  
	  x1_mean = (p1+p2+p3+p4)/4.;	// Mean position
	  //x1_mean = (p1+p2+p3+p4+p5+p6+p7+p8)/8.;	// Mean position  //added on 22.05.2018 SB
 
	  /*if(bDrawHist) 
	    hMUSIC3_dEx->Fill(x1_mean, de[2]);*/
	  
	  Float_t power = 1., Corr = 0.;
	  for(int i=0;i<4;i++) {
	    Corr += music->pos_a1[i] * power;
	    power *= x1_mean;  
	  }
      
	  if (Corr!=0) {
	    Corr = music->pos_a1[0] / Corr;
	    de_cor[2] = de[2] * Corr;
	  }
	  
	  /*if(bDrawHist) 
	    hMUSIC3_dExc->Fill(x1_mean, de_cor[2]);*/
	}
	//}
      

      /* Special gate on corrected music for cleaning x2 vs. AoQ spectrum */
      b_decor = cMusic3_dec->Test(de_cor[2]);
      
      /*if(b_de3)
	if(bDrawHist) 
       	hMUSIC3_dECOR->Fill(de_cor[2]);*/
    
    }


        /*-------------------------------------------------------------------------*/
 	/* focus index: detector number                  tof index  tof path       */
 	/*       0:     Sc01                                0:     TA - S1         */
 	/*       1:     Sc11                                1:     S1 - S2         */
 	/*       2:     Sc21                                2:     S2 - S41        */
 	/*       3:     Sc21                                3:     S2 - S42        */
 	/*       4:     Sc31                                4:     S2 - 81         */
 	/*       5:     Sc41                                5:     S2 - E1         */
	/*                                                                         */
 	/*       6:     Sc42                              tof index not used up to */
 	/*       7:     Sc43 (previously Sc51)             now, only separate      */
 	/*       8:     Sc61                              variables for S2-S41 and */
 	/*       9:     ScE1 (ESR)                                S2-S42           */
 	/*      10:     Sc81                                                       */
 	/*      11:     Sc82                                                       */
 	/*-------------------------------------------------------------------------*/
    
    

  /*  Raw data  */
   sci_l[2] = de_21l;  /* 21L         */
   sci_r[2] = de_21r;  /* 21R         */
   sci_tx[2] = dt_21l_21r + rand0_5();

   sci_l[5] = de_41l;  /* 41L         */
   sci_r[5] = de_41r;  /* 41R         */
   sci_tx[5] = dt_41l_41r + rand0_5();
  
   sci_l[6] = de_42l;  /* 42L         */
   sci_r[6] = de_42r;  /* 42R         */
   sci_tx[6] = dt_42l_42r + rand0_5();

   sci_l[7] = de_43l;  /* 43L         */
   sci_r[7] = de_43r;  /* 43R         */
   sci_tx[7] = dt_43l_43r + rand0_5();

   sci_l[10] = de_81l; /* 81L         */
   sci_r[10] = de_81r; /* 81R         */
   sci_tx[10] = dt_81l_81r + rand0_5();
   

   for (int cnt=0;cnt<6;cnt++) // 
     {
       int idx = 0 ;
       //int mw_idx = 0;
       //Float_t mwx = 0;
       switch(cnt)
	 {
	 case 0:        /* SC21 */
	   idx = 2; 
	   //mw_idx = 2;
	   //mwx = sc21_x;
	   break;    
	 case 1:        /* SC21 delayed */
	   idx = 3; 
	   //mw_idx = 2;
	   //mwx = sc21_x;
	   break;    
	 case 2:        /* SC41 */
	   idx = 5; 
	   //mw_idx = 5;
	   //mwx = tpc_sc41_x;
	   break;    
	 case 3:        /* SC42 */
           idx = 6;
	   break;
	 case 4:
	   idx = 7;     /* SC43 */
	   break;
	 case 5:
	   idx = 10;    /* SC81 */
	   break;
	 default: idx = 2;
	 }   
     
       // raw spectra 
       sci_b_l[idx] = cSCI_L[idx]->Test(sci_l[idx]);
       sci_b_r[idx] = cSCI_R[idx]->Test(sci_r[idx]);

       /*if(bDrawHist)
	 {
	   hSCI_L[idx]->Fill(sci_l[idx]);   
	   hSCI_R[idx]->Fill(sci_r[idx]);  
	 }*/
     
       if(sci_b_l[idx] && sci_b_r[idx])
	 {     
	   sci_e[idx] = sqrt( (sci_l[idx] - sci->le_a[0][idx]) * sci->le_a[1][idx] 
				  * (sci_r[idx] - sci->re_a[0][idx]) * sci->re_a[1][idx]);
	   
	   sci_b_e[idx] = cSCI_E[idx]->Test(sci_e[idx]);
	   /*if(bDrawHist) 
	     hSCI_E[idx]->Fill(sci_e[idx]);*/
	 }


       /*   Position in X direction:   */
       sci_b_tx[idx] = cSCI_Tx[idx]->Test(sci_tx[idx]);
       if (sci_b_tx[idx])
	 {
	   /*if(bDrawHist)
	     hSCI_Tx[idx]->Fill(sci_tx[idx]); */   

	   /* mm-calibrated     */
	   Float_t R = sci_tx[idx] ;//+ rand0_5(); 
	   
	   Float_t power = 1., sum = 0.;
	   for(int i=0;i<7;i++)
	     {
	       sum += sci->x_a[i][idx] * power;
	       power *= R;  
	     }
	   
	   sci_x[idx] = sum;
	   
	   sci_b_x[idx] = cSCI_X[idx]->Test(sci_x[idx]);
	   /*if(bDrawHist) 
	     hSCI_X[idx]->Fill(sci_x[idx]);*/
	 }

     } // end of loop for indices 2,3,5,6,7,10   


   /***  Scintillator Tof  spectra ***/

   /* S21 - S41 */ 

   /*  Calibrated tof  */
   sci_tofll2 = dt_21l_41l*sci->tac_factor[2] - sci->tac_off[2] ;   /* S41L- S21L */
   sci_tofrr2 = dt_21r_41r*sci->tac_factor[3] - sci->tac_off[3] ;   /* S41R- S21R */
   //        sci_tofll2 = dt_21l_41l;  
   //        sci_tofrr2 = dt_21r_41r;
   //     std::cout <<" tac_factor2 "<<sci->tac_factor[2]<<std::endl; 
   //     std::cout <<" tac_factor3 "<<sci->tac_factor[3]<<std::endl;  
   /*if(bDrawHist)
     {
       //     std::cout <<" TofLL2 "<<sci_tofll2<<std::endl;
       //     std::cout <<" TofRR2 "<<sci_tofrr2<<std::endl;
       hSCI_TofLL2->Fill(sci_tofll2);
       hSCI_TofRR2->Fill(sci_tofrr2);
     }*/
  
   sci_b_tofll2 = cSCI_TofLL2->Test(sci_tofll2);
   sci_b_tofrr2 = cSCI_TofRR2->Test(sci_tofrr2);

   /* sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      */
   if (sci_b_tofll2 && sci_b_tofrr2)
     {      /* TOF SC41 - SC21 [ps]  */
       sci_tof2 =  (sci->tof_bll2 * sci_tofll2 + sci->tof_a2 
			+ sci->tof_brr2 * sci_tofrr2)/2.0 ;
      
       /*if(bDrawHist) 
	 hSCI_Tof2->Fill(sci_tof2);*/
     }
 
   /*  
    else if (sci_b_tofll2) {
    sci_tof2 =  (sci->tof_bll2 * sci_tofll2 + sci->tof_a2) ;
    
    hSCI_Tof2->Fill(sci_tof2);
    }
    else if (sci_b_tofrr2) {
    sci_tof2 =  (sci->tof_a2 + sci->tof_brr2 * sci_tofrr2);
    
    hSCI_Tof2->Fill(sci_tof2);
    }
   */


   /***  Scintillator Tof  spectra with SC21-SC42 (as a backup) ***/ //added on 03.07.2018 SB

   //  Calibrated tof  
   sci_tofll3 = dt_42l_21l*sci->tac_factor[5] - sci->tac_off[5] ;   // S42L- S21L 
   sci_tofrr3 = dt_42r_21r*sci->tac_factor[6] - sci->tac_off[6] ;   // S42R- S21R
   //     std::cout <<" tac_factor5 "<<sci->tac_factor[5]<<std::endl; 
   //     std::cout <<" tac_factor6 "<<sci->tac_factor[6]<<std::endl;  
   /*if(bDrawHist)
     {
       //     std::cout <<" TofLL3 "<<sci_tofll3<<std::endl;
       //     std::cout <<" TofRR3 "<<sci_tofrr3<<std::endl;
       hSCI_TofLL3->Fill(sci_tofll3);
       hSCI_TofRR3->Fill(sci_tofrr3);
     }*/
  
   sci_b_tofll3 = cSCI_TofLL3->Test(sci_tofll3);
   sci_b_tofrr3 = cSCI_TofRR3->Test(sci_tofrr3);

   // sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      
   if (sci_b_tofll3 && sci_b_tofrr3)
     {      // TOF SC42 - SC21 [ps]
       sci_tof3 =  (sci->tof_bll3 * sci_tofll3 + sci->tof_a3 
			+ sci->tof_brr3 * sci_tofrr3)/2.0 ;
      
       /*if(bDrawHist) 
	 hSCI_Tof3->Fill(sci_tof3);*/
     }


   /***  Scintillator Tof  spectra with SC21-SC81 ***/ //added on 03.07.2018 SB

   //  Calibrated tof  
   sci_tofll4 = dt_21l_81l*sci->tac_factor[9] - sci->tac_off[9] ;   // S81L- S21L 
   sci_tofrr4 = dt_21r_81r*sci->tac_factor[10] - sci->tac_off[10] ;   // S81R- S21R
   /*if(bDrawHist)
     {
       hSCI_TofLL4->Fill(sci_tofll4);
       hSCI_TofRR4->Fill(sci_tofrr4);
     }*/
  
   sci_b_tofll4 = cSCI_TofLL4->Test(sci_tofll4);
   sci_b_tofrr4 = cSCI_TofRR4->Test(sci_tofrr4);

   // sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      
   if (sci_b_tofll4 && sci_b_tofrr4)
     {      // TOF SC81 - SC21 [ps]
       sci_tof4 =  (sci->tof_bll4 * sci_tofll4 + sci->tof_a4 
			+ sci->tof_brr4 * sci_tofrr4)/2.0 ;
      
       /*if(bDrawHist) 
	 hSCI_Tof4->Fill(sci_tof4);*/
     }
   

   /* check for polygon in raw detof spectrum of SC41 */
   sci_b_detof = cSCI_detof->Test(sci_tof2, sci_e[5]);
 
    /*if(bDrawHist)
     {
       /* check sc21 energy loss vs. sc21 position   */
       //     hSCI_dEx2->Fill(sci_x[2],sci_e[2]);
       //hSCI_dEx2->Fill(tpc_x[2],sqrt(sci_e[2]));
       /* check sc41 energy loss vs. sc21 position   */
       //hSCI_dEx5->Fill(sci_x[5],sci_e[5]);
       /* check sc21 (delayed) energy loss vs. sc21 position   */
       //hSCI_dEx2del->Fill(sci_x[2], sci_e[3]);
       /* compare dE at SC21 and SC41  */
       //hSCI_dE24->Fill(sci_e[2], sci_e[5]);
     //}

  id_trigger=trigger;
  /* accumulate raw detof spectrum  */
  //if(bDrawHist) 
    //  hID_dEToF->Fill(sci_tof2, de[2]);// changed to Music1. YT. 2016Jun.13 19:00 
    //hID_dEToF->Fill(sci_tof2, de[0]);// changed to Music1. YT. 2016Jun.13 19:00 

  // sci_e[idx] = sqrt( (sci_l[idx] - sci->le_a[0][idx]) * sci->le_a[1][idx] 
  //                   * (sci_r[idx] - sci->re_a[0][idx]) * sci->re_a[1][idx]);
  
  
  /* check for polygon in raw detof spectrum  */
  //  id_b_detof2 = cID_dEToF->Test(sci_tof2, de[0]);
    
  /*  select by what means S2 positions are to be derived:         */
  /*  ID.X2_select =0: SC21;  =1: S2 MWs                           */
  id_x2 = -9999;
  if(id->x2_select == 0) 
    {
      if(sci_b_x[2])
	id_x2 = sci_x[2]; /* pos index 2 = SC21 */
    }

  /* For old tracking with old nomenclature (06.06.18)
  if(id->x2_select == 1)
    {
     
      id_x2=999;
      if(b_tpc_xy[2] && b_tpc_xy[3])
	id_x2 = tpc_x_s2_foc;
      if(b_tpc_xy[2] && !b_tpc_xy[3])
	id_x2 = tpc_x[2];
      if(!b_tpc_xy[2] && b_tpc_xy[3])
	id_x2 = tpc_x[3];
      if(!b_tpc_xy[2] && !b_tpc_xy[3] && sci_b_x[2])
	id_x2 = sci_x[2];
     

     id_a2 = tpc_angle_x_s2_foc;
     id_y2 = tpc_y_s2_foc;
     id_b2 = tpc_angle_y_s2_foc;
  }
  */

   if(id->x2_select == 1)
    {
     
      id_x2=999;
      id_y2=999;
      id_a2=999;
      id_b2=999;
      
      if(b_tpc_xy[0] && b_tpc_xy[1] && !b_tpc_xy[2] && !b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_21_22;
        id_y2 = tpc_y_s2_foc_21_22;
	id_a2 = tpc_angle_x_s2_foc_21_22;
	id_b2 = tpc_angle_y_s2_foc_21_22;
	}

      if(!b_tpc_xy[0] && !b_tpc_xy[1] && b_tpc_xy[2] && b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_23_24;
        id_y2 = tpc_y_s2_foc_23_24;
	id_a2 = tpc_angle_x_s2_foc_23_24;
	id_b2 = tpc_angle_y_s2_foc_23_24;
	}

      if(!b_tpc_xy[0] && b_tpc_xy[1] && !b_tpc_xy[2] && b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_22_24;
        id_y2 = tpc_y_s2_foc_22_24;
	id_a2 = tpc_angle_x_s2_foc_22_24;
	id_b2 = tpc_angle_y_s2_foc_22_24;
	}

       if(b_tpc_xy[0] && b_tpc_xy[1] && b_tpc_xy[2] && b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_22_24;
        id_y2 = tpc_y_s2_foc_22_24;
	id_a2 = tpc_angle_x_s2_foc_22_24;
	id_b2 = tpc_angle_y_s2_foc_22_24;
	}

        if(!b_tpc_xy[0] && !b_tpc_xy[1] && !b_tpc_xy[2] && !b_tpc_xy[3] && sci_b_x[2] )
	{
	id_x2 = sci_x[2];
        id_y2 = 0.0;
	id_a2 = 0.0;
	id_b2 = 0.0;
	}
	
  }
                            
  /*  select by what means S4 positions are to be derived:         */
  /*  ID.X4_select =0: SC41;  =1: S4 MWs;                          */
  if(id->x4_select == 0) 
    {
      //    if(sci_b_x[5]) id_x4 = sci_x[5];  /* pos index 5 = SC41 */
      id_x4 = 0;  /* pos index 5 = SC41 */
    }
  
  if(id->x4_select == 1)
    {
      id_x4 = tpc_x_s4;
      id_a4 = tpc_angle_x_s4;
      id_y4 = tpc_y_s4;
      id_b4 = tpc_angle_y_s4;
    }

  
  /*  check that the positions are OK   */
  id_b_x2 = cID_x2->Test(id_x2);
  id_b_x4 = cID_x4->Test(id_x4);

  /*
  /// conditions for the 12C measurement

  // for 12N at S4
  if(de[0]>2200. && de[0]<4000.)hID_x4z55->Fill(id_x4);// added 2016Jun.16
  // for 12N at S2
   if(de[0]>2200. && de[0]<4000.)hID_x2z55->Fill(id_x2);// added 2016Jun.16

   //for 12B at s4 and s2
  if(de[0]>900. && de[0]<1200. )hID_x4z53->Fill(id_x4);
  if(de[0]>900. && de[0]<1200. )hID_x2z53->Fill(id_x2);
  */

  // This ones are for all nuclei as a funtion of the energy loss

  //hID_E_Xs4->Fill(id_x4,de[0]);// added by 2016Jun.16

  //hID_E_Xs2->Fill(id_x2,de[0]);// added by 2016Jun.16

  /*hID_x2a2->Fill(id_x2,id_a2);
  hID_y2b2->Fill(id_y2,id_b2);
  hID_x4a4->Fill(id_x4,id_a4);
  hID_y4b4->Fill(id_y4,id_b4); */


  /****  A/Q calculations for S41-S21 or S42-S41 (tof index 2+3) *************/ 
  Float_t f = 931.4940 / 299.792458 ;    /* the u/(c*10^-6) factor  */

  /*----------------------------------------------------------*/
  /* Determination of beta                                    */
  /* ID.TofOff(i)                   Flight time offset [ps]   */
  /* ID.Path(i)                     Flight path/c [ps]        */
  /* TOF(i)        BIN FLOAT(24),   Flight time  [ps]         */
  /*----------------------------------------------------------*/
  /* from ToF S41-S21 */
  if (sci_b_tofll2 && sci_b_tofrr2)
    {
      id_beta = id->id_path2 / (id->id_tofoff2 - sci_tof2);
      /*if(bDrawHist) 
	hID_beta->Fill(id_beta*1000.);*/
    }
  
  /*------------------------------------------------------*/
  /* Determination of Brho                                */
  /* Dispersion and magnification are still the same      */
  /* variable for S41-S21 and S42-S41, adjust in setup.C  */
  /*------------------------------------------------------*/

  /* check S2 valid conditions */
  //if (id_b_x2 && id_b_x4) {
  if (id_b_x2)
    {   
      // first half of FRS
      id_rho[0] = frs->rho0[0] * (1. - id_x2/1000./frs->dispersion[0]);   
      // second half of FRS
      id_rho[1] = frs->rho0[1] * (1. - (id_x4 - frs->magnification[1] * id_x2) / 1000. / frs->dispersion[1]) ; 
      //    id_rho[1] = frs->rho0[1] * (1. - (frs->magnification[1] * id_x2) / 1000. / frs->dispersion[1]) ; 
      
      for(int i=0;i<2;i++)
	{
	  id_brho[i] = (fabs(frs->bfield[2*i]) + fabs(frs->bfield[2*i+1]))/ 2. * id_rho[i];
	  /*if(bDrawHist) 
	    hID_BRho[i]->Fill(id_brho[i]);*/
	}
    }   
  //}


  /*--------------------------------------------------------------*/
  /* Determination of A/Q                                         */
  /*--------------------------------------------------------------*/
  /* Beta(i)       BIN FLOAT(24),   Beta = v/c                    */
  /* Gamma(i)      BIN FLOAT(24),   Gamma= sqrt(1/1-beta**2)      */
  /*--------------------------------------------------------------*/
  /* for S2-S4 */
  // if (sci_b_tofll2 && sci_b_tofrr2 && id_b_x2 && id_b_x4) {
  if (sci_b_tofll2 && sci_b_tofrr2 &&  id_b_x2)
    {
      if ((id_beta>0.0) && (id_beta<1.0))
	{
	  id_gamma = sqrt( 1. /(1. - id_beta * id_beta));
	  //id_AoQ = id_brho[1]/id_beta/id_gamma/ f - id->id_tofcorr2 * id_x4;
	  id_AoQ = id_brho[1]/id_beta/id_gamma/ f ;
	  //      std::cout <<" id_AoQ "<<id_AoQ<<std::endl; 
	  
	  //correction for id_a2, JK 16.9.11


	  id_AoQ_corr = id_AoQ - frs->a2AoQCorr * id_a2;
	  if (!b_tpc_xy[4] || !b_tpc_xy[5])
	    id_AoQ_corr = id_AoQ - frs->a4AoQCorr * id_a4;


	  /*if(bDrawHist)
	    {
	      hID_AoQ->Fill(id_AoQ);
	      hID_AoQ_corr->Fill(id_AoQ_corr);
	    }*/
	  id_b_AoQ = kTRUE;
	}
    }

  if(id_b_AoQ)
    {
      /*if(bDrawHist)
	{
	  hID_DeltaBrho_AoQ->Fill(id_AoQ,id_brho[0]-id_brho[1]);
	  hID_DeltaBrho_AoQzoom->Fill(id_AoQ,id_brho[0]-id_brho[1]);
	}*/
    }
  
  // }

  /*------------------------------------------------*/
  /* Determination of Z                             */
  /*------------------------------------------------*/
  /****  S4  (MUSIC 1)   */
  //  if((de_cor[0]>0.0) && (id_beta>0.0) && (id_beta<1.0)) {
  
  if((de[0]>0.0) && (id_beta>0.0) && (id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a[i];
	  power *= id_beta;
	}
      id_v_cor = sum;
      
      if (id_v_cor > 0.0)
	id_z = frs->primary_z * sqrt(de[0]/id_v_cor) + frs->offset_z;
      
    if ((id_z>0.0) && (id_z<100.0))
      {
	id_b_z = kTRUE;
	/*if(bDrawHist) 
	  {
	    hID_Z->Fill(id_z);
	  }*/
      }
    }

  /****  S4  (MUSIC 2)   */
  
  //  if((de_cor[0]>0.0) && (id_beta>0.0) && (id_beta<1.0)) {
  if((de[1]>0.0) && (id_beta>0.0) && (id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a2[i];
	  power *= id_beta;
	}
      id_v_cor2 = sum;
      
      if (id_v_cor2 > 0.0)
	id_z2 = frs->primary_z * sqrt(de[1]/id_v_cor2) + frs->offset_z2;
      //std::cout<<"id_z2="<<id_z2<<std::endl;
      
      if ((id_z2>0.0) && (id_z2<100.0))
	{
	  id_b_z2 = kTRUE;
	  /*if(bDrawHist) 
	    hID_Z2->Fill(id_z2);*/
	}
      /*if(bDrawHist)
	{
	  hID_Z_Z2->Fill(id_z,id_z2);
	  hID_Z_dE2->Fill(id_z,de[1]);
	}*/
    }
 

  /****  S4  (MUSIC OLD)   */
  //  if((de_cor[0]>0.0) && (id_beta>0.0) && (id_beta<1.0)) {
  
  if((de[2]>0.0) && (id_beta>0.0) && (id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a3[i];
	  power *= id_beta;
	}
      id_v_cor3 = sum;
      
      if (id_v_cor3 > 0.0)
	id_z3 = frs->primary_z * sqrt(de[2]/id_v_cor3) + frs->offset_z3;
      //std::cout<<"id_z2="<<id_z2<<std::endl;
      
      if ((id_z3>0.0) && (id_z3<100.0))
	{
	  id_b_z3 = kTRUE;
	  /*if(bDrawHist)
	    {
	      hID_Z3->Fill(id_z3);
	      hID_ZmaxRange->Fill(id_z3);
	    }*/
	}
      /*if(bDrawHist)
	hID_Z_Z3->Fill(id_z,id_z3);*/


      static const double anode_width = 10.;//cm
      double music_dX = anode_width*sqrt(id_a4*id_a4+id_b4*id_b4+1.);
      //h_dEdx_betagammaAll->Fill(id_beta*id_gamma,de[2]/music_dX);
      //h_dEdx_betagammaAllZoom->Fill(id_beta*id_gamma,de[2]/music_dX);
      double music_dEtemp0 = music_e3[0]*music->e3_gain[0] + music->e3_off[0];
      for(int i=0;i<4;++i)
	{
	  double music_dEtemp = music_e3[i]*music->e3_gain[i] + music->e3_off[i];
	  //h_dEdx_betagamma[i]->Fill(id_beta*id_gamma,music_dEtemp/music_dX);
	  /*if(i>0)
	    h_DiffdEdx_betagamma[i]->Fill(id_beta*id_gamma,(music_dEtemp-music_dEtemp0)/music_dX);*/
	}
    }
  /*------------------------------------------------*/
  /* Identification Plots                           */
  /*------------------------------------------------*/

  /****  for S2-S4  ****/

  //Float_t my_dist_TPC6= 1075; //position TPC6 in s388?
  //Float_t x4env=0;
  //Float_t y4env=0;
  //  Float_t my_dist_TPC65;
  if(id_b_AoQ)
    {
      /*if(bDrawHist)
	{ 
	  hID_x2AoQ->Fill(id_AoQ, id_x2);  
	  hID_x4AoQ->Fill(id_AoQ, id_x4);
	  //    hID_Z2_AoQ->Fill(id_AoQ, id_z2);
	  // hID_Z3_AoQ->Fill(id_AoQ, id_z3);
	  hID_x2x4->Fill(id_x2,id_x4);
	  hID_SC41dE_AoQ->Fill(id_AoQ, sci_e[5]);
	}*/

      

      
      for (int i=0;i<5;i++)
	{
	id_b_x4AoQ_Z[i] = cID_x4AoQ_Z[i]->Test(id_AoQ, id_x4); 
        }
      
      id_b_x2AoQ = cID_x2AoQ[0]->Test(id_AoQ, id_x2);
      for(int i=0;i<6;++i)
	{
	  if(cID_x2AoQ[i]->Test(id_AoQ, id_x2)==true)
	    {
	      //hMUSIC3_x2AoQ_E[0][i]->Fill(music_e3[0]);
	      //hMUSIC3_x2AoQ_E[1][i]->Fill(music_e3[1]);
	      //hMUSIC3_x2AoQ_E[2][i]->Fill(music_e3[2]);
	      //hMUSIC3_x2AoQ_E[3][i]->Fill(music_e3[3]);	      
	      //hID_x4AoQ_x2AoQgate[i]->Fill(id_AoQ, id_x4);
	      /*if (id_b_z3)
		hID_ZAoQ_x2AoQgate[i]->Fill(id_AoQ, id_z3);*/
	    }
	}
	       
      //      if (id_b_z3) // changed to Music1. YT. 2016Jun.13 19:00
      if (id_b_z) // changed to Music1. YT. 2016Jun.13 19:00   
	{   
	  if(bDrawHist)
	    { 

	      //hID_Z_Q->Fill(id_z);
	       
	      //hID_Z_AoQ->Fill(id_AoQ, id_z);// changed to Music1. YT. 2016Jun.13 19:00 
	      //hID_Z_AoQv2->Fill(id_AoQ, id_z);// changed to Music1. YT. 2016Jun.13 19:00 
	      //hID_Z_AoQ_corr->Fill(id_AoQ_corr, id_z); // changed to Music1. YT. 2016Jun.13 19:00 
	      //hID_Z_AoQ_corrZoom->Fill(id_AoQ_corr,id_z);

	      //hID_x2z->Fill(id_z, id_x2);// MUSIC1
	      //hID_x4z->Fill(id_z, id_x4);// MUSIC1
	      
	      
	      if(id_z>= id->zgate_low && id_z<= id->zgate_high) // changed to Music1. YT. 2016Jun.13 19:00
		{
		  //hID_x4AoQ_zgate->Fill(id_AoQ_corr,id_x4);
		}
	      
	      //hID_Z_Sc21E->Fill(id_z, sqrt(sci_l[2]*sci_r[2])); // changed to Music1. YT. 2016Jun.13 19:00

	      if(id_b_z2)
		{
		  //hID_Z2_AoQ->Fill(id_AoQ, id_z2);
		  if(TMath::Abs(id_z-id_z2-0.3)<0.6)
		    {
		      //hID_Z_AoQ_zsame->Fill(id_AoQ, id_z);
		    }
		}
	      
	      
	      // S395 envelop plots, tracking from position(TPC5) and angle (TPC5, TPC4)
	      /*
		if( id_b_x4) {
		for(Int_t zz=0; zz<=2600; zz+=100) {
		x4env = id_a4 * (zz-my_dist_TPC6)/1000. + tpc_x[5];
		y4env = id_b4 * (zz-my_dist_TPC6)/1000. + tpc_y[5];
		hID_xz->Fill(zz,x4env);
		hID_yz->Fill(zz,y4env);
		// hID_xzc->Fill(zz,x4env);
		//  hID_yzc->Fill(zz,y4env);
		}
		}
	      */	      


	    }
	
	  for(int i=0;i<5;i++)
	    {
	      id_b_music_z[i] = cID_Z_Z[i]->Test(id_z);   
	      if(cID_Z_AoQ[i]==nullptr)
		std::cout<<"E> cID_Z_AoQ["<<i<<"] nullptr "<<cID_Z_AoQ[i]<<" | "<<id->ID_Z_AoverQ_num[i]<<std::endl;
	      
	       id_b_z_AoQ[i] = cID_Z_AoQ[i]->Test(id_AoQ, id_z);      
	      
		// if(i==0)
		//   { 
		//     if (id_b_z_AoQ[0])
		//       { 
		// 	hMUSIC3_z_AoQ_E[0][i]->Fill(music_e3[0]);
		// 	hMUSIC3_z_AoQ_E[1][i]->Fill(music_e3[1]);
		// 	hMUSIC3_z_AoQ_E[2][i]->Fill(music_e3[2]);
		// 	hMUSIC3_z_AoQ_E[3][i]->Fill(music_e3[3]);
		// 	hID_x4c[0]->Fill(id_x4);
		// 	hID_x2c[0]->Fill(id_x2);
		      
		// 	hMUSIC3_music_z_E[0][i]->Fill(music_e3[0]);
		// 	hMUSIC3_music_z_E[1][i]->Fill(music_e3[1]);
		// 	hMUSIC3_music_z_E[2][i]->Fill(music_e3[2]);
		// 	hMUSIC3_music_z_E[3][i]->Fill(music_e3[3]);
		//       }
		//   }
	      if (id_b_z_AoQ[i])
		  {
		    /*if(bDrawHist)
		      { 
			
			hID_Z_AoQgate[i]->Fill(id_AoQ, id_z);;
			hID_Z3_gate[i]->Fill(id_z3);			
			hID_dE3_gate[i]->Fill(de[2]);

			hMUSIC3_z_AoQ_E[0][i]->Fill(music_e3[0]);
			hMUSIC3_z_AoQ_E[1][i]->Fill(music_e3[1]);
			hMUSIC3_z_AoQ_E[2][i]->Fill(music_e3[2]);
			hMUSIC3_z_AoQ_E[3][i]->Fill(music_e3[3]);

			
			hID_x4c[i]->Fill(id_x4);
			hID_x2c[i]->Fill(id_x2);
			hMUSIC3_music_z_E[0][i]->Fill(music_e3[0]);
			hMUSIC3_music_z_E[1][i]->Fill(music_e3[1]);
			hMUSIC3_music_z_E[2][i]->Fill(music_e3[2]);
			hMUSIC3_music_z_E[3][i]->Fill(music_e3[3]);*/

			// S395 envelop plots, tracking from position(TPC6) and angle (TPC5, TPC6)
			/*
			  if( id_b_x4 && i==0) {
			  for(Int_t zz=0; zz<=2600; zz+=100) {
			  x4env =  id_a4 * (zz-my_dist_TPC6)/1000. + tpc_x[5];
			  y4env =  id_b4 * (zz-my_dist_TPC6)/1000. + tpc_y[5];
			  //hID_xz->Fill(zz,x4env);
			  //hID_yz->Fill(zz,y4env);
			  hID_xzc->Fill(zz,x4env);
			  hID_yzc->Fill(zz,y4env);
			  }
			  }
			*/
			 
			  
			  //hID_betac[i]->Fill(id_beta);
			  //hID_brhoc[i]->Fill(id_brho[1]);
			
			  // hID_x_target1c[i]->Fill(tpc_x_s2_target1);
			  // hID_x_target2c[i]->Fill(tpc_x_s4_target2);
			  // hID_y_target1c[i]->Fill(tpc_y_s2_target1);
			  // hID_y_target2c[i]->Fill(tpc_y_s4_target2);
			
		      
			// if(i==1){
			//if(coin[10]==1)
			//	hID_x4c[i]->Fill(id_x4);
			//}else{
			//hID_x4c[i]->Fill(id_x4);
			//}
		      //}
		  }
	      }
	}
    }
  
  if(EventFlag==0x200 && trigger==1)
    {
      mrtof_start = mrtof_start*25./1000000.; // ch to microsec
      
      double tempStop1 = mrtof_stopDelay*25./1000000. ;
      double tempStop2 = mrtof_stop*25./1000000. ;
      
      /*h_MRtof_Start->Fill(mrtof_start);
      h_MRtof_StopDelay->Fill(tempStop1);
      h_MRtof_Stop->Fill(tempStop2);*/
      
      int stop_status=0;
      
      if(tempStop1<=0.)
	{
	  if(tempStop2>0.)
	    {
	      mrtof_stop  = tempStop2;
	      //h_MRtof_status->Fill("GoodStop2",1);
	      stop_status=1;
	    }
	  else
	    {
	      mrtof_stop = -9999.;
	      //std::cout<<"E> MRTOF Anal : both stop signal "<<
	      /*if(mrtof_start>1)
		h_MRtof_status->Fill("NoGoodStop",1);
	      else
		h_MRtof_status->Fill("NoData",1);*/
	    }
	}
      else
	{
	  if(tempStop2<=0.)
	    {
	      mrtof_stop = tempStop1-45.;
	      //h_MRtof_status->Fill("GoodStop1",1);
	      stop_status=1;
	    }
	  else
	    {
	      double diff_stop = tempStop1-tempStop2;
	      if(TMath::Abs(diff_stop-45)<1.)
		{
		  mrtof_stop = tempStop1-45.;
		  //h_MRtof_status->Fill("GoodStop1&2",1);
		  stop_status=1;
		}
	      else
		{
		  //h_MRtof_status->Fill("Stop1&2BadOverlap",1);
		  std::cout<<"E> MRtof diff stop too big from 45 microsec !"<<diff_stop<< "sorted : D="<<mrtof_stopDelay <<" "<< mrtof_stop <<" | in microsec: D="<< tempStop1 <<" "<<tempStop2<<"\n";
		}
	    }
	}

      //int diff_stopdelay = 1891482-54830;

      if(stop_status==1)
	{
	  mrtof_tof = 100 - ( mrtof_start - mrtof_stop); // add 100 microsec from mrtof trigger system 
	  //h_MRtof_tof->Fill(mrtof_tof);
	  //h_MRtof_status->Fill("GoodTOF",1);
	}
      /*else
	h_MRtof_status->Fill("BadTOF",1);*/


    }

  mrtof_si_e1=si_e1;
  mrtof_si_e2=si_e2;
  mrtof_si_e3=si_e3;
  mrtof_si_e4=si_e4;
  mrtof_si_e5=si_e5;
  
}

void FRS_Detector_System::Setup_Parameters(){

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

void FRS_Detector_System::Process_MBS(int* pdata){return;
    
  //TXRSSortEvent *srt = dynamic_cast<TXRSSortEvent*> (GetInputEvent("Calibr"));
  
  
  
/*  TXRSCalibrEvent *cal = dynamic_cast<TXRSCalibrEvent*> (GetInputEvent("Analysis"));
  TXRSAnlEvent *anl = dynamic_cast<TXRSAnlEvent*> (GetInputEvent());

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



Int_t FRS_Detector_System::getbits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*16 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
}

Int_t FRS_Detector_System::get2bits(Int_t value, int nword, int start, int length)
{
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*32 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
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

Float_t FRS_Detector_System::rand0_5() 
{
  return rand()*1./RAND_MAX;// - 0.5;   
}
