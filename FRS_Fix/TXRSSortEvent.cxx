#include "TXRSSortEvent.h"

#include "Riostream.h"

#include "TXRSSortProc.h"
#include "TGo4FileSource.h"


TXRSSortEvent::TXRSSortEvent() : TGo4EventElement("FRSSortEvent")//, fxProc(0), fxFileSrc(0) { 
{ }

TXRSSortEvent::TXRSSortEvent(const char* name) : TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) { 
{ }

TXRSSortEvent::~TXRSSortEvent()
{ }

Int_t TXRSSortEvent::Init() 
{ 
  Int_t rev=0;
  Clear();
  // if(CheckEventSource("TXRSSortProc")) {
  //   fxProc = (TXRSSortProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TXRSSortEvent::Fill() 
// { 
//   Clear();
//   if(fxProc) fxProc->FRSSort(this); else    // user event processing method
//     if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file
//   return 0;
// }

void TXRSSortEvent::Clear(Option_t *t)
{
  EventFlag = 0;
  
  ts_id = 0;
  for(int i=0;i<4;i++)
    ts_word[i] = 0;
  for(int i=0;i<3;i++)
    tsys_word[i] = 0;
  
  timestamp = 0;
  timespill = 0.;
  timespill2 = 0.;

  systemtime_ms = 0.;
  systemtime_s = 0.;
  
  pattern = 0;
  trigger = 0;
  
  for(int i=0;i<32;i++)
    {
      //sc_long[i] = 0;
      sc_long2[i] = 0;
    }

   for(int i=0;i<64;i++)
    {
      sc_long[i] = 0;
    }
  //  ic_de = 0;
  
  for(int i=0;i<13;i++)
    {
      mw_an[i] = 0; 
      mw_xl[i] = 0;
      mw_xr[i] = 0;
      mw_yu[i] = 0; 
      mw_yd[i] = 0;
    }

  //TPC part
  for(int i=0;i<7;i++)
    {
      tpc_l[i][0]=0;
      tpc_r[i][0]=0;
      tpc_lt[i][0]=0;
      tpc_rt[i][0]=0;
      tpc_l[i][1]=0;
      tpc_r[i][1]=0;
      tpc_lt[i][1]=0;
      tpc_rt[i][1]=0;
      for(int j=0;j<4;j++)
	{
	  tpc_dt[i][j]=0;
	  tpc_a[i][j]=0;
	}
    }
  
  de_21l = 0;
  de_21r = 0;
  de_41l = 0;
  de_41r = 0;
  de_41u = 0;
  de_41d = 0;
  de_42l = 0;
  de_42r = 0;
  de_43l = 0;
  de_43r = 0;
  de_81l = 0;
  de_81r = 0;
  de_21ld = 0; 
  de_21rd = 0;
  de_v1l = 0;
  de_v1r = 0;
  de_v2l = 0;
  de_v2r = 0;
  de_v3  = 0;
  
  dt_21l_21r = 0;
  dt_41l_41r = 0;
  dt_21l_41l = 0;
  dt_21r_41r = 0;
  dt_42l_42r = 0;
  dt_43l_43r = 0;
  dt_42l_21l = 0;
  dt_42r_21r = 0;
  dt_41u_41d = 0;
  dt_81l_81r = 0;
  dt_21l_81l = 0;
  dt_21r_81r = 0;

  // User multihit TDC
  for(int i=0;i<10;i++) {
      tdc_sc41l[i] = 0;
      tdc_sc41r[i] = 0;
      tdc_sc21l[i] = 0;
      tdc_sc21r[i] = 0;
      tdc_sc42l[i] = 0;
      tdc_sc42r[i] = 0;
      tdc_sc43l[i] = 0;
      tdc_sc43r[i] = 0;
      tdc_sc81l[i] = 0;
      tdc_sc81r[i] = 0;
      
  }
  
  for(int i=0;i<8;i++)
    {
      music_e1[i] = 0;
      music_t1[i] = 0;
      music_e2[i] = 0;
      music_t2[i] = 0;
    }

  for(int i=0;i<4;i++)
    {
      music_e3[i] = 0;
      music_t3[i] = 0;
    }

  
  music_pres[0] = 0; music_pres[1] = 0;  music_pres[2] = 0;
  music_temp[0] = 0; music_temp[1] = 0;  music_temp[2] = 0;

	
  //Si detectors
  si_adc1=0;
  si_adc2=0;
  si_adc3=0;
  si_adc4=0;
  si_adc5=0;

  for (int i=0;i<32;i++)
   {
     dssd_adc[i]=0;
   }


  SingleAnode_adc = 0;
  
  //Channeltron detectors
  ct_signal=0;
  ct_time = 0;
  ct_trigger_DU=kFALSE;
  ct_trigger_SY=kTRUE;
       
  //Elctron current
  ec_signal=0;

  mrtof_start = 0;
  mrtof_stop = 0;
  mrtof_stopDelay = 0;
  
}

ClassImp(TXRSSortEvent)
