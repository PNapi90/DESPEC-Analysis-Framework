#include "TXRSUnpackProc.h"

#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"

#include "TXRSAnalysis.h"
#include "TXRSUnpackEvent.h"

#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <bitset>
//#include  <stdio.h>

//#define DEBUG

TXRSUnpackProc::TXRSUnpackProc() : TXRSBasicProc("FRSUnpackProc")
{
  frs = dynamic_cast<TXRSParameter*>(GetParameter("FRSPar"));

}

TXRSUnpackProc::TXRSUnpackProc(const char* name) :  TXRSBasicProc(name) 
{ 
  hTrigger = MakeH1I("Raw_data", "newTrigger", 16, 0.5, 16.5); 

  frs = dynamic_cast<TXRSParameter*>(GetParameter("FRSPar"));
  ModSetup = dynamic_cast<TModParameter*>(GetParameter("ModPar"));
  if(frs == nullptr)
    {
      std::cout<<"E> FRS parameters not set properly, it is nullptr !"<<std::endl;
      std::exit(-1);
    }
  if(ModSetup == nullptr)
    {
      std::cout<<"E> Electronic Module parameters not set properly, it is nullptr !"<<std::endl;
      std::exit(-1);
    }
  previousTimeStamp[0] = 0;
  previousTimeStamp[1] = 0;
  previousTimeStamp[2] = 0;
  currentTimeStamp = 0;
  
  bool remove_histos = (frs!=nullptr) && (!frs->fill_raw_histos);
  
  for(int n=0;n<32;n++)
    {

      
      hVME0_8[n]  = MakeH1ISeries("Raw_data/VME0",  8, 0, n, remove_histos);
      hVME0_9[n]  = MakeH1ISeries("Raw_data/VME0",  9, 0, n, remove_histos);
      hVME0_11[n] = MakeH1ISeries("Raw_data/VME0", 11, 0, n, remove_histos);
      hVME0_12[n] = MakeH1ISeries("Raw_data/VME0", 12, 0, n, remove_histos);
      //    hVME0_13[n] = MakeH1ISeries("Raw data/VME0", 13, 0, n, remove_histos);
      //    hVME0_14[n] = MakeH1ISeries("Raw data/VME0", 14, 0, n, remove_histos);

      hVME1_8[n]  = MakeH1ISeries("Raw_data/VME1", 8, 1, n, remove_histos);
      hVME1_9[n]  = MakeH1ISeries("Raw_data/VME1", 9, 1, n, remove_histos);
      hVME1_3[n]  = MakeH1ISeries("Raw_data/VME1", 3, 1, n, remove_histos);
      hVME1_15[n] = MakeH1ISeries("Raw_data/VME1", 15, 1, n, remove_histos);

      hVME5_11[n] = MakeH1ISeries("Raw_data/VME5", 11, 5, n, remove_histos);
      
      //hVME1_16[n] = MakeH1ISeries("Raw data/VME1", 16, 1, n, remove_histos);
      //hVME1_17[n] = MakeH1ISeries("Raw data/VME1", 17, 1, n, remove_histos);

      hVME2_TDC[n] = MakeH1ISeries3("Raw_data/VME2", 1, 2, n, remove_histos);
 
      hVME3_TDC[n] = MakeH1ISeries3("Raw_data/VME3", 2, 3, n, remove_histos);
      //hVME2_12[n] = MakeH1ISeries("Raw data/VME2", 12, 2, n, remove_histos);
      
    }

  h_VME0_8All  = MakeH2I("Raw_data/VME0","V0_Module_geo8AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME0_9All  = MakeH2I("Raw_data/VME0","V0_Module_geo9AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME0_11All = MakeH2I("Raw_data/VME0","V0_Module_geo11AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME0_12All = MakeH2I("Raw_data/VME0","V0_Module_geo12AllCh",32,0,32,512,0,4096,"#Ch","",1);
			     
  h_VME1_8All  = MakeH2I("Raw_data/VME1","V1_Module_geo8AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME1_9All  = MakeH2I("Raw_data/VME1","V1_Module_geo9AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME1_3All  = MakeH2I("Raw_data/VME1","V1_Module_geo3AllCh",32,0,32,512,0,4096,"#Ch","",1);
  h_VME1_15All = MakeH2I("Raw_data/VME1","V1_Module_geo15AllCh",32,0,32,512,0,4096,"#Ch","",1);
			     
  hVME2_TDCAll = MakeH2I("Raw_data/VME2","V2_Module_AllCh",32,0,32,1024,0,1048576,"#Ch","",1);
  hVME3_TDCAll = MakeH2I("Raw_data/VME3","V3_Module_AllCh",32,0,32,1024,0,1048576,"#Ch","",1);

  
  
  h_UnpackStatus = MakeH2I("Unpack","Status",2*21*32,0.,2*21*32,10,0.,10.,"#Ch","Status",1);
  h_TSFlagStatus = MakeH2I("Unpack","TS_flagStatus",1000,0,1000,10,0,10,"Diff_TS","Status",1);
  
  h_Multiplicity = MakeH2I("Unpack","mTDC_multiplicity",200,0,200,10,0,10,"ch mTDC","Multiplicity",1);
}

TXRSUnpackProc::~TXRSUnpackProc()
{ }



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

Bool_t TXRSUnpackProc::BuildEvent(TGo4EventElement* output) 
{
  
  TXRSUnpackEvent* tgt = dynamic_cast<TXRSUnpackEvent*> (output);
  if (tgt==nullptr)
    return kFALSE;
  tgt->SetValid(kFALSE);  // by default output event rejected

  TGo4MbsEvent *fInput = dynamic_cast<TGo4MbsEvent*> (GetInputEvent());
  if (fInput==nullptr)
    return kFALSE;

  
  /* ---------------------- ProcID = 10 ------------------ */

  /*  Check that the EVENT type and subtype are OK:  */
  //  if((fInput->GetType()!=10) || (fInput->GetSubtype()!=1))
  //   {
  //     std::cout << "Wrong event type " << fInput->GetType() << std::endl;
  //     return kFALSE;
  //     //  }else{
  //     //    std::cout << "Good event type " << fInput->GetType() << std::endl;
  //     //    return;
  //   }
  
  // If trigger 14 or 15, event fully can be skipped
  if((fInput->GetTrigger()==14) || (fInput->GetTrigger()==15))
    {
      //std::cout << "Trigger = " << fInput->GetTrigger() << ", event skipped" << std::endl;
      return kFALSE;
    }

  /*  Put the event header stuff into output event  */
  
  tgt->qlength   = fInput->GetDlen()   ;
  tgt->qtype     = fInput->GetType()   ;
  tgt->qsubtype  = fInput->GetSubtype();
  tgt->qdummy    = fInput->GetDummy()  ;
  tgt->qtrigger  = fInput->GetTrigger();
  tgt->qevent_nr = fInput->GetCount()  ;

  // Special event
  if(fInput->GetCount()!=0)
    {
      // std::cout << "qtype : " << fInput->GetType() << std::endl;  
      // std::cout << "qlength(=GetDLen) : " << tgt->qlength  << std::endl;
      // std::cout << "qsubtype : " <<  tgt->qsubtype << std::endl;

      /*  Check event length: if it's <= 4 it is empty or screwed up! */
      if(fInput->GetDlen()<=4)
	{
	  std::cout << "Wrong event length: " << fInput->GetDlen() << " =< 4" << std::endl;
	  return kFALSE;
	}
  
      /*  Event OK, now assign pointer to local subevent header! */
      //std::cout << "trigger = " << fInput->GetTrigger() << std::endl;    
      fInput->ResetIterator();
      TGo4MbsSubEvent *psubevt = nullptr;

      while ((psubevt=fInput->NextSubEvent())!= nullptr)
	{
	  // start subevents loop
#ifdef DEBUG
	  psubevt->PrintEvent();
#endif
	  //printf("TRI:%d\n",fInput->GetTrigger());
	  //printf("PROC:%d\n",psubevt->GetProcid());
	  //std::cout << "subevent control " << int(psubevt->GetControl()) << std::endl;
	  //std::cout << "cratesubevent : " << int(psubevt->GetSubcrate()) << std::endl; 
	  
	  //      if (psubevt->GetControl()==29) {  // for tpc crate
	  /*      if(psubevt->GetProcid()==20) { // for tpc crate
	  //printf("TRI:%d\n",fInput->GetTrigger());
	  //printf("PROC:%d\n",psubevt->GetProcid());
	  //std::cout << "subevent control " << int(psubevt->GetControl()) << std::endl;
	  //std::cout << "cratesubevent : " << int(psubevt->GetSubcrate()) << std::endl; 
	  
	  if (fInput->GetTrigger()==5 || fInput->GetTrigger()==6 || fInput->GetTrigger()==7 ||  fInput->GetTrigger()==8 || fInput->GetTrigger()==9 || fInput->GetTrigger()==10 || fInput->GetTrigger()==12 || fInput->GetTrigger()==13) 
	  UnpackUserSubevent(psubevt, tgt);
	  continue;
	  }
	  */
	  /* check for ProcID 10 = standard crate  */
	  //    if((psubevt->GetProcid())!=10) {
	  if( (psubevt->GetProcid()!=10) && (psubevt->GetProcid()!=15) && (psubevt->GetProcid()!=20) && (psubevt->GetProcid()!=30) && (psubevt->GetProcid()!=40) && (psubevt->GetProcid()!=50))
	    {
	      std::cout << "Wrong ProcID " << psubevt->GetProcid() << std::endl; 
	      continue; // skip non standard event
	    }
    
	  /*  Check the SUBevent type and subtype!             */
	
	  if( !( (psubevt->GetType() == 12) && (psubevt->GetSubtype() == 1) )    &&
	      !( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) ) &&
	      !( (psubevt->GetType() == 88) && (psubevt->GetSubtype() == 8800) ) )
	    {  // for all data 
	      std::cout << "getsubtype " << psubevt->GetSubtype() << std::endl;
	      std::cout << "gettype " << psubevt->GetType() << std::endl;
	      std::cout << "Wrong subevent type " << psubevt->GetType() << std::endl;
	      continue; // skip subevent SL
	    }
	  
	  if( (psubevt->GetType() == 36) && (psubevt->GetSubtype() == 3600) )
	    continue;
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

	  switch(fInput->GetTrigger())
	    { // trigger value
      
	    case 1: //FRS trigger box
	    case 2: //Ion catcher Si 1
	    case 3: //Ion catcher Si 2
	    case 4: //Ion catcher Si 3 (not used)
	    case 5: //trigger 10 Hz for scaler readout
	    case 6:
	    case 7:
	    case 8:
	    case 9:
	    case 10: 
	    case 11:
	    case 12:   // start of extraction, not used
	    case 13:
	      // end of extraction, not used 
	      Event_Extract(tgt,psubevt);
	      break;
	      // end trigger 5,6,7,8,9,10,12,13
	      
	      // start of acquisition (vme system):        
	    case 14: 
	      // stop of acquisition (vme system):         
	    case 15:
	      {
       
		// get pointer on data   
		//Int_t *pdata = psubevt->GetDataField();
	
		// special: TIME STAMP module data (4 longwords)   
		//   no header or end-of-block info, we must hardwire GEO = 20... 
		/* 
		   Int_t vme_chn = 0;
		   for (int i=0;i<4;i++) {
		   tgt->vme0[20][vme_chn++] = getbits(*pdata,1,1,16);
		   tgt->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
		   pdata++;
		   }
		*/
		break;   
	      } // end case 14-15 
	      
	    default:
	      { 
		std::cout << "Invalid trigger " << fInput->GetTrigger() << std::endl;
		return kFALSE; 
	      }
      
	    }  // switch on trigger value
	
	} // end subevents loop
	

    } // end special event

  FillHistograms(tgt); // fill histograms from output event

  
  tgt->SetValid(kTRUE); // accept event
  return kTRUE;
} // close unpack proc



Bool_t TXRSUnpackProc::Event_Extract(TXRSUnpackEvent* event_out, TGo4MbsSubEvent* psubevt, int)
{

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
	event_out->EventFlag = event_flag;
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
	      event_out->vme0[20][i] = getbits(*pdata,1,1,16);
	      tempTS += event_out->vme0[20][i]*bit_weight[i];
	      
	      //event_out->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
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

	  //printf("Event_flag_test=0x%08x\n", event_out->EventFlag);
	  	  
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
		      event_out->vme0[vme_geo][i] = *pdata;
		      //event_out->vme2scaler[i] = *pdata;

#ifdef DEBUG
		      std::cout<<" Ch "<<i<<"# "<<event_out->vme0[vme_geo][i] <<std::endl;
#endif
		      }
		      else
		    {
		      Int_t sc_data = get2bits(*pdata,1,1,26);
		      Int_t  sc_ch = get2bits(*pdata,1,27,5);
		      if(sc_ch != i)
			std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
		      event_out->vme0[vme_geo][i] = sc_data;
		      //event_out->vme2scaler[i] = sc_data;
#ifdef DEBUG
		      std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
#endif
		    }
		  pdata++; len++;
		}
	      // std::cout<<"1Hz unpack, "<<tgt->vme0[6][3]<<std::endl;
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
				event_out->vme0[vme_geo][vme_chn] = getbits(*pdata,1,1,16);

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
	      const auto MaxCh = ModSetup->Nb_Channels.find(vme_geo); 
	      for(int i=0;i<MaxCh->second;++i)
		{
		  h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
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
		  
		  event_out->vme0[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
		  Int_t vme_statusVD = getbits(*pdata,14,1,1);
		  Int_t vme_statusUN = getbits(*pdata,13,1,1);
		  Int_t vme_statusOV = getbits(*pdata,12,1,1);

		  if(vme_statusVD == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
		  else
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);

		  if(vme_statusUN == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);

		  if(vme_statusOV == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);

#ifdef DEBUG
		  std::cout<<" Ch "<<vme_chn<<" "<<event_out->vme0[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
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
    // 	  event_out->vme0[20][vme_chn++] = getbits(*pdata,1,1,16);
    // 	  event_out->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
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
	  return kFALSE;
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

		      multihit = event_out->nhit5[vme_chn][LeadingOrTrailing];
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
			      event_out->vme2s[vme_chn][multihit] = value;
			      hVME2_TDC[vme_chn]->Fill(value);
			    }
			}
		  
		      else
			{
#ifdef DEBUG
			  std::cout << " +-> tdc T value = " << value << std::endl;
#endif
			  if (value > 0)
			    event_out->vme2s_trailing[vme_chn][multihit] = value;
			}
		      event_out->nhit5[vme_chn][LeadingOrTrailing]++;
		    
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
	      event_out->vme5[20][i] = getbits(*pdata,1,1,16);
	      tempTS += event_out->vme5[20][i]*bit_weight[i];
	      //event_out->vme5[20][vme_chn++] = getbits(*pdata,2,1,16);
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

		      event_out->vme5[vme_geo][i] = *pdata;

#ifdef DEBUG
		      std::cout<<" Ch "<<i<<"# "<<event_out->vme5[vme_geo][i] <<std::endl;
#endif
		    }
		  else
		    {
		      Int_t sc_data = get2bits(*pdata,1,1,26);
		      Int_t  sc_ch = get2bits(*pdata,1,27,5);
		      if(sc_ch != i)
			std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
		      event_out->vme5[vme_geo][i] = sc_data;
#ifdef DEBUG
		      std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
#endif
		    }
		  pdata++; len++;
		}
	      // std::cout<<"1Hz unpack, "<<tgt->vme0[6][3]<<std::endl;
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
	      const auto MaxCh = ModSetup->Nb_Channels.find(vme_geo); 
	      for(int i=0;i<MaxCh->second;++i)
	      {
	         h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
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
		  
		  event_out->vme5[vme_geo][vme_chn] = getbits(*pdata,1,1,16);
		  Int_t vme_statusVD = getbits(*pdata,14,1,1);
		  Int_t vme_statusUN = getbits(*pdata,13,1,1);
		  Int_t vme_statusOV = getbits(*pdata,12,1,1);

		  if(vme_statusVD == 1)
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
		  else
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);

		  if(vme_statusUN == 1)
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);

		  if(vme_statusOV == 1)
		  h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);

#ifdef DEBUG
		  std::cout<<" Ch "<<vme_chn<<" "<<event_out->vme5[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
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
// 		      event_out->vme1[vme_geo][i] = *pdata;
// #ifdef DEBUG
// 		      std::cout<<" Ch "<<i<<"# "<<event_out->vme0[vme_geo][i] <<std::endl;
// #endif
// 		    }
// 		  else
// 		    {
// 		      Int_t sc_data = get2bits(*pdata,1,1,26);
// 		      Int_t  sc_ch = get2bits(*pdata,1,27,5);
// 		      if(sc_ch != i)
// 			std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
// 		      event_out->vme1[vme_geo][i] = sc_data;
// #ifdef DEBUG
// 		      std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
// #endif
// 		    }
// 		  pdata++; len++;
// 		}
// 	      // std::cout<<"1Hz unpack, "<<event_out->vme0[6][3]<<std::endl;
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
				event_out->vme1[vme_geo][vme_chn] = getbits(*pdata,1,1,16);

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
	      const auto MaxCh = ModSetup->Nb_Channels.find(vme_geo); 
	      for(int i=0;i<MaxCh->second;++i)
		{
		  h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
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
		  event_out->vme1[vme_geo][vme_chn] = getbits(*pdata,1,1,16);	                 
		  //printf("DATA:%d %d %d\n",vme_geo,vme_chn,event_out->vme1[vme_geo][vme_chn] ); 
	      
		  int vme_statusVD = getbits(*pdata,14,1,1);
		  int vme_statusUN = getbits(*pdata,13,1,1);
		  int vme_statusOV = getbits(*pdata,12,1,1);

		  if(vme_statusVD == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
		  else
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);
	      
		  if(vme_statusUN == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);
	      
		  if(vme_statusOV == 1)
		    h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);

#ifdef DEBUG
		  std::cout<<" Ch "<<vme_chn<<" "<<event_out->vme1[vme_geo][vme_chn]<<" status ["<<vme_statusVD<<", "<<vme_statusUN<<", "<<vme_statusOV<<"]"<<std::endl;
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
	      event_out->vme3[20][i] = getbits(*pdata,1,1,16);
	      tempTS += event_out->vme3[20][i]*bit_weight[i];
	      //event_out->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
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
	  event_out->vme3scaler[i] = *pdata;
#ifdef DEBUG
	  std::cout<<" Ch "<<i<<"# "<<event_out->vme3scaler[i] <<std::endl;
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
		      multihit = event_out->vme3_MT_nhit5[vme_chn][LeadingOrTrailing];
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
			      event_out->vme3s_MT[vme_chn][multihit] = value;
			      //hVME2_TDC[vme_chn]->Fill(value);
			    }
			}
		  
		      else
			{
#ifdef DEBUG
			  std::cout << " +-> tdc T value = " << value << std::endl;
#endif
			  if (value > 0)
			    event_out->vme3s_MT_trailing[vme_chn][multihit] = value;
			}
		      event_out->vme3_MT_nhit5[vme_chn][LeadingOrTrailing]++;
		    
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
	      event_out->vme4[20][i] = getbits(*pdata,1,1,16);
	      tempTS += event_out->vme4[20][i]*bit_weight[i];
	      //event_out->vme0[20][vme_chn++] = getbits(*pdata,2,1,16);
	      pdata++; len++;
	    }
	  previousTimeStamp[2] = currentTimeStamp;
	  currentTimeStamp = tempTS;	  
	}

    default :
      break;
    } // end switch prodID

  return kTRUE;

}

#ifdef USELESS
void TXRSUnpackProc::UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TXRSUnpackEvent* tgt)
{
  const auto it_Crate = ModSetup->MapCrates.find(psubevt->GetProcid());
  if(it_Crate == ModSetup->MapCrates.end())
    std::cout<<"E> Crate Mapping does not have this ProcID :"<<psubevt->GetProcid()<<std::endl;

  Int_t *pdata = psubevt->GetDataField();	
  Int_t len = 0;  

  if(ModSetup->Nb_Scaler > 0)
    {
      /** \note FRS SCALER module data (1 longword per channel)   
       *  This module has sequential readout therefore no channel
       *  number contained in the data longwords. 
       */
      // read the header longword and extract slot, type & length
      std::cout<<"word :"<<std::bitset<32>(*pdata)<<" "<<std::endl;
      
      Int_t vme_geo = getbits(*pdata,2,12,5);
      Int_t vme_type = getbits(*pdata,2,9,3);
      Int_t vme_nlw =  getbits(*pdata,2,3,6);
      pdata++; len++;
      if(vme_type!=4)
	std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;

      std::cout<<"Scaler :"<<vme_geo<<" "<<vme_type<<" "<<vme_nlw<<std::endl;

      // read the data 
      if (vme_nlw > 0)
	{
	  for(int i=0;i<vme_nlw;i++)
	    {
	      if(ModSetup->Scaler32bit)
		{
		  tgt->vme1[vme_geo][i] = *pdata;
		  std::cout<<" Ch "<<i<<"# "<<tgt->vme0[vme_geo][i] <<std::endl;
		}
	      else
		{
		  Int_t sc_data = get2bits(*pdata,1,1,26);
		  Int_t  sc_ch = get2bits(*pdata,1,27,5);
		  tgt->vme1[vme_geo][i] = sc_data;
		  std::cout<<" Ch "<<sc_ch<<"# "<<sc_data<<std::endl;
		}
	      pdata++; len++;
	    }
	  // std::cout<<"1Hz unpack, "<<tgt->vme0[6][3]<<std::endl;
	  // read and ignore the expected "end-of-block" longword   
	  //pdata++; len++;
	}
    }


  
  /* read the header longword and extract slot, type & length  */
  Int_t vme_geo = getbits(*pdata,2,12,5);
  //Int_t vme_type = getbits(*pdata,2,9,3);
  Int_t vme_nlw =  getbits(*pdata,1,1,6);
  pdata++; len++;


  /* read the data from scaler */
  if (vme_nlw > 0)
    {
      for(int i=0;i<vme_nlw;i++)
	{
	  tgt->vme1[vme_geo][i] = *pdata;
	  pdata++; len++;
	}
      /* read and ignore the expected "end-of-block" longword   */
      pdata++; len++;
    }

  /* for ProcID = 20 - rest of the unpacking */

  while (len < (psubevt->GetDlen()-2)/2)
    {
      
      /* read the header longword and extract slot, type & length  */
      Int_t vme_chn = 0;
      Int_t vme_geo = getbits(*pdata,2,12,5);
      Int_t vme_type = getbits(*pdata,2,9,3);
      Int_t vme_nlw =  getbits(*pdata,1,1,6);
      pdata++; len++;
      //std::cout<<"data "<<vme_geo<<" "<<vme_type<<" "<<vme_nlw;
      
      const auto it_Module = it_Crate->second.find(vme_geo);
      if(it_Module == it_Crate->second.end())
	std::cout<<"E> Crate Mapping does not have this module (vmeGEO^^) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;

      int IdMod = it_Module->second;
      //std::cout<<" idmod:"<<IdMod<<std::endl;
      
      // read the data
      if(vme_type == 6)
	{
	  // not valid data !
	  const auto MaxCh = ModSetup->Nb_Channels.find(vme_geo); 
	  for(int i=0;i<MaxCh->second;++i)
	    {
	      h_UnpackStatus->Fill(IdMod*32+i,"not valid/Header",1.);
	      //h_UnpackStatus->Fill(IdMod*MaxCh->second+i,6.);
	    }
	  
	}
      //    std::cout<<"type = "<<vme_type<<"nlw = "<<vme_nlw<<std::endl;
      /* read the data */
      if ((vme_type == 2) && (vme_nlw > 0))
	{
	  for(int i=0;i<vme_nlw;i++)
	    {  
	      vme_geo = getbits(*pdata,2,12,5);
	      vme_type = getbits(*pdata,2,9,3);
	      vme_chn = getbits(*pdata,2,1,5);
	      tgt->vme1[vme_geo][vme_chn] = getbits(*pdata,1,1,16);	                 
	      //printf("DATA:%d %d %d\n",vme_geo,vme_chn,tgt->vme1[vme_geo][vme_chn] ); 
	      
	      int vme_statusVD = getbits(*pdata,14,1,1);
	      int vme_statusUN = getbits(*pdata,13,1,1);
	      int vme_statusOV = getbits(*pdata,12,1,1);

	      if(vme_statusVD == 1)
		h_UnpackStatus->Fill(IdMod*32.+vme_chn,"valid",1.);
	      else
		h_UnpackStatus->Fill(IdMod*32.+vme_chn,"not valid/Data",1.);
	      
	      if(vme_statusUN == 1)
		h_UnpackStatus->Fill(IdMod*32.+vme_chn,"UnderThreshold",1.);
	      
	      if(vme_statusOV == 1)
		h_UnpackStatus->Fill(IdMod*32.+vme_chn,"Overflow",1.);




	      pdata++; len++;
	    }
	  
	  /* read and ignore the expected "end-of-block" longword */
	  pdata++; len++;
	}
      
    }  /* end of the while... loop  */
		
  // if ((frs!=0) && frs->fill_raw_histos)
  //   for(int i=0;i<32;i++)
  //     {
  // 	if (hVME1_8[i]) hVME1_8[i]->Fill(tgt->vme1[8][i] & 0xfff);
  // 	if (hVME1_9[i]) hVME1_9[i]->Fill(tgt->vme1[9][i] & 0xfff);
  // 	if (hVME1_16[i]) hVME1_16[i]->Fill(tgt->vme1[15][i] & 0xfff);
  // 	if (hVME1_17[i]) hVME1_17[i]->Fill(tgt->vme1[17][i] & 0xfff);
  //     }
}  
#endif


Bool_t TXRSUnpackProc::FillHistograms(TXRSUnpackEvent* event)
{


  
  hTrigger->Fill(event->qtrigger);
  for(size_t i = 0;i<ModSetup->EventFlags.size();++i)
    if(event->EventFlag==ModSetup->EventFlags[i])
      {
	if(previousTimeStamp[i]>0)
	  {
	    h_TSFlagStatus->Fill((currentTimeStamp-previousTimeStamp[i])*1e-4,i);
	    //std::cout<<" eventFlag :"<<event->EventFlag<<" "<<currentTimeStamp-previousTimeStamp[i]<<" "<<currentTimeStamp<<"\n";
	  }
	// else
	//   std::cout<<"First eventFlag :"<<event->EventFlag<<" "<<currentTimeStamp<<"\n";
	  
      }
  if(event->EventFlag==ModSetup->EventFlags.back())
    {
      	if(previousTimeStamp[0]>0)
	  {
	    h_TSFlagStatus->Fill((currentTimeStamp-previousTimeStamp[0])*1e-4,3);
	    //std::cout<<" +-> "<<event->EventFlag<<" "<<currentTimeStamp-previousTimeStamp[0]<<"\n";
	  }
    }
  if(frs->fill_raw_histos)
    {
      
      for(int i=0;i<32;i++)
	{
	  if (hVME0_8[i]) hVME0_8[i]->Fill(event->vme0[8][i] & 0xfff);
	  if (hVME0_9[i]) hVME0_9[i]->Fill(event->vme0[9][i] & 0xfff);
	  if (hVME0_11[i]) hVME0_11[i]->Fill(event->vme0[11][i] & 0xfff);
	  if (hVME0_12[i]) hVME0_12[i]->Fill(event->vme0[12][i] & 0xfff);
	      
	  if( h_VME0_8All )
	    h_VME0_8All->Fill(i,event->vme0[8][i] & 0xfff);
	  if( h_VME0_9All )
	    h_VME0_9All->Fill(i,event->vme0[9][i] & 0xfff);
	  if( h_VME0_11All )
	    h_VME0_11All->Fill(i,event->vme0[11][i] & 0xfff);
	  if( h_VME0_12All )
	    h_VME0_12All->Fill(i,event->vme0[12][i] & 0xfff);

	  //if (hVME0_13[i]) hVME0_13[i]->Fill(event->vme0[13][i] & 0xfff);
	  //if (hVME0_14[i]) hVME0_14[i]->Fill(event->vme0[14][i] & 0xfff);
	}

      for(int i=0;i<32;i++)
	{
	  if (hVME5_11[i]) hVME5_11[i]->Fill(event->vme5[11][i] & 0xfff);  
	}

      for(int i=0; i<32; ++i)
	for(int j=0; j<10; ++j)
	  {
	    if(event->vme2s[i][j] > 0)
	      {
		if (hVME2_TDC[i])
		  hVME2_TDC[i]->Fill(event->vme2s[i][j]);
		if (hVME2_TDCAll)
		  hVME2_TDCAll->Fill(i,event->vme2s[i][j]);
		if(h_Multiplicity)
		  h_Multiplicity->Fill(i,j);
	      }

	    if(event->vme3s_MT[i][j] > 0)
	      {
		if (hVME3_TDC[i])
		  hVME3_TDC[i]->Fill(event->vme3s_MT[i][j]);
		if(hVME3_TDCAll)
		  hVME3_TDCAll->Fill(i,event->vme3s_MT[i][j]);
		if(h_Multiplicity)
		  h_Multiplicity->Fill(i+100,j);
	      }

	  }
      // 	  // from UnpackUserSubevent 
      // 	  // if ((frs!=0) && frs->fill_raw_histos)
      for(int i=0;i<32;i++)
	{
	  if (hVME1_8[i]) hVME1_8[i]->Fill(event->vme1[8][i] & 0xfff);
	  if (hVME1_9[i]) hVME1_9[i]->Fill(event->vme1[9][i] & 0xfff);
	  if (hVME1_3[i]) hVME1_3[i]->Fill(event->vme1[3][i] & 0xfff);
	  if (hVME1_15[i]) hVME1_15[i]->Fill(event->vme1[15][i] & 0xfff);
	  
	  
	  if( h_VME1_8All )
	    h_VME1_8All->Fill(i,event->vme1[8][i] & 0xfff);
	  if( h_VME1_9All )
	    h_VME1_9All->Fill(i,event->vme1[9][i] & 0xfff);
	  if( h_VME1_3All )
	    h_VME1_3All->Fill(i,event->vme1[3][i] & 0xfff);
	  if( h_VME1_15All )
	    h_VME1_15All->Fill(i,event->vme1[15][i] & 0xfff);

	}
    }

  return kTRUE;
}




ClassImp(TXRSUnpackProc)

