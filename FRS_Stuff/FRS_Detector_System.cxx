#include "FRS_Detector_System.h"

#include "TH1.h"

#include "TFRSSortEvent.h"
#include "TFRSCalibrEvent.h"
#include "TFRSAnlEvent.h"
#include "TFRSUserEvent.h"
#include "TFRSParameter.h"



void FRS_Detector_System::FRS_Detector_System(){


  // MUSIC PARAMETERS //
  
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
   
  
      Float_t sci_l[i]  = 0;
      Float_t sci_r[i]  = 0;
      Float_t sci_e[i]  = 0;
      Float_t sci_tx[i] = 0;
      Float_t sci_x[i]  = 0;
      
  }
      
  
  Float_t sci_tofll2 = 0;
  Float_t sci_tofll3 = 0
  Float_t sci_tof2   = 0;
  Float_t sci_tofrr2 = 0;
  Float_t sci_tofrr3 = 0;
  Float_t sci_tof3   = 0;
  
  // ID PARAMETERS //
  
  Float_t ID_x2 = 0;
  Float_t ID_y2 = 0;
  Float_t ID_a2 = 0;
  Float_t ID_b2 = 0;
  
  Float_t ID_x4 = 0;
  Float_t ID_y4 = 0;
  Float_t ID_a4 = 0;
  Float_t ID_b4 = 0;
  
  brho = new Float_t[2];
  rho  = new Float_t[2];
  
  for(int i=0; i<2; ++i){

      Float_t ID_brho[i] = 0;
      Float_t ID_rho[i]  = 0;

  }
  
  Float_t beta      = 0;
  Float_t beta3     = 0;
  Float_t gamma     = 0;
  Float_t AoQ       = 0;
  Float_t AoQ_corr  = 0;
  
  Float_t z         = 0;
  Float_t z2        = 0;
  Float_t z3        = 0;
  
  Float_t timestamp = 0;
  Float_t ts        = 0;
  Float_t ts2       = 0;

}

/*  TFRSSortEvent *srt = dynamic_cast<TFRSSortEvent*> (GetInputEvent("Calibr"));
  TFRSCalibrEvent *cal = dynamic_cast<TFRSCalibrEvent*> (GetInputEvent("Analysis"));
  TFRSAnlEvent *anl = dynamic_cast<TFRSAnlEvent*> (GetInputEvent());

  //if ((tgt==0) || (cal==0) || (srt==0) || (anl==0)) return;

  // event are flush to store tree only when a event from MRtof arrived meanwhile lastest FRS event is stored.
  //if(anl->EventFlag==Setup->EventFlags[0])
  //  {
  //    CopyPrevious(*anl);
      
  //    tgt->SetValid(kFALSE);  // FRS event not flush alone
  //  }
  //if(anl->EventFlag==Setup->EventFlags[1])
  // {
      Process_Analysis(*srt,*cal,*anl,*RAW);
  //    tgt->SetValid(kTRUE);  // MRtof + lastest FRS event to be flushed. 
  //  }
}*/

//---------------------------------------------------------------

void FRS_Detector_System::Process_MBS(int* pdata){
    
  TFRSSortEvent *srt = dynamic_cast<TFRSSortEvent*> (GetInputEvent("Calibr"));
  TFRSCalibrEvent *cal = dynamic_cast<TFRSCalibrEvent*> (GetInputEvent("Analysis"));
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
  ts2       = anl.ts2;  

}

//---------------------------------------------------------------

void FRS_Detector_System::get_Event_data(RawEvent* RAW) {
    
    RAW->set_DATA_MUSIC(dE, dE_cor);
    RAW->set_DATA_SCI(sci_l, sci_r, sci_e, sci_tx, sci_x);
    RAW->set_DATA_SCI_ToF(sci_tofll2, sci_tofll3, sci_tof2, sci_tofrr2, sci_tofrr3, sci_tof3);
    RAW->set_DATA_ID_2_4(ID_x2, ID_y2, ID_a2, ID_b2, ID_x4, ID_y4, ID_a4, ID_b4);
    RAW->set_DATA_ID_Beta_Rho(ID_brho, ID_rho, beta, beta3, gamma);
    RAW->set_DATA_ID_Z_AoQ(AoQ, AoQ_corr, z, z2, z3);
    RAW->set_DATA_ID_Timestamp(timestamp, ts, ts2);

}

//---------------------------------------------------------------

int* FRS_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

