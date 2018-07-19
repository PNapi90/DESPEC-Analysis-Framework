#include "TXRSUserProc.h"

#include "TH1.h"

#include "TXRSSortEvent.h"
#include "TXRSCalibrEvent.h"
#include "TXRSAnlEvent.h"
#include "TXRSUserEvent.h"
#include "TXRSParameter.h"


TXRSUserProc::TXRSUserProc() : TXRSBasicProc("FRSUserProc")//,evtFile(nullptr),allEvtTree(nullptr),impTree(nullptr),decTree(nullptr)
{
  Setup = dynamic_cast<TModParameter*>(GetParameter("ModPar"));
  
}

TXRSUserProc::TXRSUserProc(const char* name) : TXRSBasicProc(name)//,evtFile(nullptr),allEvtTree(nullptr),impTree(nullptr),decTree(nullptr)
{ 
  Setup = dynamic_cast<TModParameter*>(GetParameter("ModPar"));
  
  // Create_ROOT_Tree_Data();
}

TXRSUserProc::~TXRSUserProc() 
{
  //Write and close the new root file 
  //evtFile=allEvtTree->GetCurrentFile();
  // if(evtFile!=nullptr)
  //   {
  //     evtFile->Write();
  //     evtFile->Close();
  //   }
}

void TXRSUserProc::FRSUserProc(TXRSUserEvent* tgt)
{


  TXRSSortEvent *srt = dynamic_cast<TXRSSortEvent*> (GetInputEvent("Calibr"));
  TXRSCalibrEvent *cal = dynamic_cast<TXRSCalibrEvent*> (GetInputEvent("Analysis"));
  TXRSAnlEvent *anl = dynamic_cast<TXRSAnlEvent*> (GetInputEvent());

  if ((tgt==0) || (cal==0) || (srt==0) || (anl==0)) return;

  // event are flush to store tree only when a event from MRtof arrived meanwhile lastest FRS event is stored.
  if(anl->EventFlag==Setup->EventFlags[0])
    {
      CopyPrevious(*anl);
      
      tgt->SetValid(kFALSE);  // FRS event not flush alone
    }
  if(anl->EventFlag==Setup->EventFlags[1])
    {
      Process_Analysis(*srt,*cal,*anl,*tgt);
      tgt->SetValid(kTRUE);  // MRtof + lastest FRS event to be flushed. 
    }
}


// void TXRSUserProc::Create_ROOT_Tree_Data() {


// }
void TXRSUserProc::CopyPrevious(TXRSAnlEvent &anl) 
{
  previous.EventFlag = anl.EventFlag;
  
  for(int i=0;i<8;i++)
    {
     previous.music_b_e1[i] = anl.music_b_e1[i];
     previous.music_b_t1[i] = anl.music_b_t1[i];
     previous.music_b_e2[i] = anl.music_b_e2[i];
     previous.music_b_t2[i] = anl.music_b_t2[i];
    }
  for(int i=0;i<4;i++)
    {
      previous.music_b_t3[i] = anl.music_b_t3[i];
      previous.music_b_e3[i] = anl.music_b_e3[i];
    }
  previous.music1_anodes_cnt = anl.music1_anodes_cnt;
  previous.music2_anodes_cnt = anl.music2_anodes_cnt;
  previous.music3_anodes_cnt = anl.music3_anodes_cnt;
  previous.b_de1 = anl.b_de1;
  previous.b_de2 = anl.b_de2;
  previous.b_de3 = anl.b_de3;
  previous.b_dt3 = anl.b_dt3;
  previous.de[0] = anl.de[0];
  previous.de[1] = anl.de[1];
  previous.de[2] = anl.de[2];
  previous.de_cor[0] = anl.de_cor[0];
  previous.de_cor[1] = anl.de_cor[1];
  previous.de_cor[2] = anl.de_cor[2];
  previous.x1_mean = anl.x1_mean;
  previous.b_decor = anl.b_decor;
  
  // SCI part
  for(int i=0;i<12;i++)
    {
      previous.sci_l[i] = anl.sci_l[i] ;
      previous.sci_r[i] = anl.sci_r[i] ;
      previous.sci_e[i] = anl.sci_e[i] ;
      previous.sci_tx[i]= anl.sci_tx[i];
      previous.sci_x[i] = anl.sci_x[i] ;
      previous.sci_b_l[i] = anl.sci_b_l[i] ;
      previous.sci_b_r[i] = anl.sci_b_r[i] ;
      previous.sci_b_e[i] = anl.sci_b_e[i] ;
      previous.sci_b_tx[i]= anl.sci_b_tx[i];
      previous.sci_b_x[i] = anl.sci_b_x[i] ;
    }
  
  // veto SCI 
  for(int i=0;i<3;i++)
    {
      previous.sci_veto_l[i]  = anl.sci_veto_l[i];
      previous.sci_veto_r[i]  = anl.sci_veto_r[i];
      previous.sci_b_veto_l[i]= anl.sci_b_veto_l[i];
      previous.sci_b_veto_r[i]= anl.sci_b_veto_r[i];
      previous.sci_b_veto_e[i]= anl.sci_b_veto_e[i];
    }
  
  previous.sci_b_veto_r[3] = anl.sci_b_veto_r[3]; // veto3 missing channel

  previous.itag_42_e =anl.itag_42_e;
  previous.itag_43_e =anl.itag_43_e;

  previous.sci_u5 = anl.sci_u5;
  previous.sci_d5 = anl.sci_d5;
  previous.sci_b_u5 = anl.sci_b_u5;
  previous.sci_b_d5 = anl.sci_b_d5;
  
  previous.sci_ty5= anl.sci_ty5;
  previous.sci_y5 = anl.sci_y5 ;
  previous.sci_b_ty5 =anl.sci_b_ty5;
  previous.sci_b_y5 = anl.sci_b_y5 ;
  
  previous.sci_tofll2 = anl.sci_tofll2;
  previous.sci_tofll3 = anl.sci_tofll3;
  previous.sci_tofll4 = anl.sci_tofll4;
  previous.sci_tofrr2 = anl.sci_tofrr2;
  previous.sci_tofrr3 = anl.sci_tofrr3;
  previous.sci_tofrr4 = anl.sci_tofrr4;
  previous.sci_tof2 = anl.sci_tof2;
  previous.sci_tof3 = anl.sci_tof3;
  previous.sci_tof4 = anl.sci_tof4;
  previous.sci_b_tofll2 = anl.sci_b_tofll2;
  previous.sci_b_tofll3 = anl.sci_b_tofll3;
  previous.sci_b_tofll4 = anl.sci_b_tofll4;
  previous.sci_b_tofrr2 = anl.sci_b_tofrr2;
  previous.sci_b_tofrr3 = anl.sci_b_tofrr3;
  previous.sci_b_tofrr4 = anl.sci_b_tofrr4;

  previous.sci_b_detof = anl.sci_b_detof;
  
  // ID part
  
  /*  some dummy values...           */
  previous.id_x2=anl.id_x2;
  previous.id_y2=anl.id_y2;
  previous.id_a2=anl.id_a2;
  previous.id_b2=anl.id_b2;
  previous.id_x4=anl.id_x4;
  previous.id_y4=anl.id_y4;
  previous.id_a4=anl.id_a4;
  previous.id_b4=anl.id_b4;
  
  previous.id_b_x2 = kFALSE;
  previous.id_b_x4 = kFALSE;
  previous.id_b_detof2 = kFALSE;
  
  for (int i=0;i<2;i++)
    {
      previous.id_brho[i]= anl.id_brho[i];  
      previous.id_rho[i] = anl.id_rho[i] ;
    }
  previous.id_beta     = anl.id_beta ; 
  previous.id_beta3    = anl.id_beta3; 
  previous.id_gamma    = anl.id_gamma;
  previous.id_AoQ      = anl.id_AoQ     ;
  previous.id_AoQ_corr = anl.id_AoQ_corr;

  previous.id_v_cor = anl.id_v_cor ;
  previous.id_v_cor2= anl.id_v_cor2;
  previous.id_z  = anl.id_z ;
  previous.id_z2 = anl.id_z2;
  previous.id_z3 = anl.id_z3;
  previous.id_energy_geL = anl.id_energy_geL;
  previous.id_tac_41_geL = anl.id_tac_41_geL;
  previous.id_energy_geL_raw = anl.id_energy_geL_raw;
  previous.id_tac_41_geL_raw = anl.id_tac_41_geL_raw;
  previous.id_stopper_x = anl.id_stopper_x;
  previous.id_trigger = anl.id_trigger;
  previous.id_scal_seconds = anl.id_scal_seconds;
  previous.id_scal_geL  = anl.id_scal_geL ;
  previous.id_scal_sc21 = anl.id_scal_sc21;
  previous.id_scal_sc41 = anl.id_scal_sc41;
  previous.id_scal_sc42 = anl.id_scal_sc42;
  previous.id_scal_sc43 = anl.id_scal_sc43;

  previous.id_b_AoQ    = anl.id_b_AoQ  ;
  previous.id_b_z      = anl.id_b_z    ;
  previous.id_b_x2AoQ  = anl.id_b_x2AoQ;
  for(int i=0;i<5;i++)
    {
      previous.id_b_x4AoQ_Z[i]= anl.id_b_x4AoQ_Z[i];  
      previous.id_b_z_AoQ[i]  = anl.id_b_z_AoQ[i]  ;
      previous.id_b_music_z[i]= anl.id_b_music_z[i];
    }
  previous.timestamp=anl.timestamp;
  previous.ts=anl.ts;  
  previous.ts2=anl.ts2;  
  
}



void TXRSUserProc::Process_Analysis(TXRSSortEvent &srt, TXRSCalibrEvent &cal, TXRSAnlEvent &anl, TXRSUserEvent &tgt) 
{
  // fill tgt object from the MRTOF anl part 
  tgt.EventFlag = anl.EventFlag;
  
  tgt.timestamp_mrtof = anl.timestamp;

  tgt.mrtof_start = anl.mrtof_start;
  tgt.mrtof_stop = anl.mrtof_stop;
  tgt.mrtof_tof = anl.mrtof_tof;

  tgt.mrtof_si_e1 = anl.mrtof_si_e1;
  tgt.mrtof_si_e2 = anl.mrtof_si_e2;
  tgt.mrtof_si_e3 = anl.mrtof_si_e3;
  tgt.mrtof_si_e4 = anl.mrtof_si_e4;
  tgt.mrtof_si_e5 = anl.mrtof_si_e5;
  
  // fill the rest of tgt object from the closest FRS anl 
  for(int i=0;i<8;i++)
    {
      tgt.music_b_e1[i] = previous.music_b_e1[i];
      tgt.music_b_t1[i] = previous.music_b_t1[i];
      tgt.music_b_e2[i] = previous.music_b_e2[i];
      tgt.music_b_t2[i] = previous.music_b_t2[i];
    }
  for(int i=0;i<4;i++)
    {
      tgt.music_b_t3[i] = previous.music_b_t3[i];
      tgt.music_b_e3[i] = previous.music_b_e3[i];
    }
  tgt.music1_anodes_cnt = previous.music1_anodes_cnt;
  tgt.music2_anodes_cnt = previous.music2_anodes_cnt;
  tgt.music3_anodes_cnt = previous.music3_anodes_cnt;
  tgt.b_de1 = previous.b_de1;
  tgt.b_de2 = previous.b_de2;
  tgt.b_de3 = previous.b_de3;
  tgt.b_dt3 = previous.b_dt3;
  tgt.de[0] = previous.de[0];
  tgt.de[1] = previous.de[1];
  tgt.de[2] = previous.de[2];
  tgt.de_cor[0] = previous.de_cor[0];
  tgt.de_cor[1] = previous.de_cor[1];
  tgt.de_cor[2] = previous.de_cor[2];
  tgt.x1_mean = previous.x1_mean;
  tgt.b_decor = previous.b_decor;
  
  // SCI part
  for(int i=0;i<12;i++)
    {
      tgt.sci_l[i] = previous.sci_l[i] ;
      tgt.sci_r[i] = previous.sci_r[i] ;
      tgt.sci_e[i] = previous.sci_e[i] ;
      tgt.sci_tx[i]= previous.sci_tx[i];
      tgt.sci_x[i] = previous.sci_x[i] ;
      tgt.sci_b_l[i] = previous.sci_b_l[i] ;
      tgt.sci_b_r[i] = previous.sci_b_r[i] ;
      tgt.sci_b_e[i] = previous.sci_b_e[i] ;
      tgt.sci_b_tx[i]= previous.sci_b_tx[i];
      tgt.sci_b_x[i] = previous.sci_b_x[i] ;
    }
  
  // veto SCI 
  for(int i=0;i<3;i++)
    {
      tgt.sci_veto_l[i]  = previous.sci_veto_l[i];
      tgt.sci_veto_r[i]  = previous.sci_veto_r[i];
      tgt.sci_b_veto_l[i]= previous.sci_b_veto_l[i];
      tgt.sci_b_veto_r[i]= previous.sci_b_veto_r[i];
      tgt.sci_b_veto_e[i]= previous.sci_b_veto_e[i];
    }
  
  tgt.sci_b_veto_r[3] = previous.sci_b_veto_r[3]; // veto3 missing channel

  tgt.itag_42_e =previous.itag_42_e;
  tgt.itag_43_e =previous.itag_43_e;

  tgt.sci_u5 = previous.sci_u5;
  tgt.sci_d5 = previous.sci_d5;
  tgt.sci_b_u5 = previous.sci_b_u5;
  tgt.sci_b_d5 = previous.sci_b_d5;
  
  tgt.sci_ty5= previous.sci_ty5;
  tgt.sci_y5 = previous.sci_y5 ;
  tgt.sci_b_ty5 =previous.sci_b_ty5;
  tgt.sci_b_y5 = previous.sci_b_y5 ;
  
  tgt.sci_tofll2 = previous.sci_tofll2;
  tgt.sci_tofll3 = previous.sci_tofll3;
  tgt.sci_tofll4 = previous.sci_tofll4;
  tgt.sci_tofrr2 = previous.sci_tofrr2;
  tgt.sci_tofrr3 = previous.sci_tofrr3;
  tgt.sci_tofrr4 = previous.sci_tofrr4;
  tgt.sci_tof2 = previous.sci_tof2;
  tgt.sci_tof3 = previous.sci_tof3;
  tgt.sci_tof4 = previous.sci_tof4;
  tgt.sci_b_tofll2 = previous.sci_b_tofll2;
  tgt.sci_b_tofll3 = previous.sci_b_tofll3;
  tgt.sci_b_tofll4 = previous.sci_b_tofll4;  
  tgt.sci_b_tofrr2 = previous.sci_b_tofrr2;
  tgt.sci_b_tofrr3 = previous.sci_b_tofrr3;
  tgt.sci_b_tofrr4 = previous.sci_b_tofrr4;

  tgt.sci_b_detof = previous.sci_b_detof;
  
  // ID part
  
  /*  some dummy values...           */
  tgt.id_x2=previous.id_x2;
  tgt.id_y2=previous.id_y2;
  tgt.id_a2=previous.id_a2;
  tgt.id_b2=previous.id_b2;
  tgt.id_x4=previous.id_x4;
  tgt.id_y4=previous.id_y4;
  tgt.id_a4=previous.id_a4;
  tgt.id_b4=previous.id_b4;
  
  tgt.id_b_x2 = kFALSE;
  tgt.id_b_x4 = kFALSE;
  tgt.id_b_detof2 = kFALSE;
  
  for (int i=0;i<2;i++)
    {
      tgt.id_brho[i]= previous.id_brho[i];  
      tgt.id_rho[i] = previous.id_rho[i] ;
    }
  tgt.id_beta     = previous.id_beta ; 
  tgt.id_beta3    = previous.id_beta3; 
  tgt.id_gamma    = previous.id_gamma;
  tgt.id_AoQ      = previous.id_AoQ     ;
  tgt.id_AoQ_corr = previous.id_AoQ_corr;

  tgt.id_v_cor = previous.id_v_cor ;
  tgt.id_v_cor2= previous.id_v_cor2;
  tgt.id_z  = previous.id_z ;
  tgt.id_z2 = previous.id_z2;
  tgt.id_z3 = previous.id_z3;
  tgt.id_energy_geL = previous.id_energy_geL;
  tgt.id_tac_41_geL = previous.id_tac_41_geL;
  tgt.id_energy_geL_raw = previous.id_energy_geL_raw;
  tgt.id_tac_41_geL_raw = previous.id_tac_41_geL_raw;
  tgt.id_stopper_x = previous.id_stopper_x;
  tgt.id_trigger = previous.id_trigger;
  tgt.id_scal_seconds = previous.id_scal_seconds;
  tgt.id_scal_geL  = previous.id_scal_geL ;
  tgt.id_scal_sc21 = previous.id_scal_sc21;
  tgt.id_scal_sc41 = previous.id_scal_sc41;
  tgt.id_scal_sc42 = previous.id_scal_sc42;
  tgt.id_scal_sc43 = previous.id_scal_sc43;

  tgt.id_b_AoQ    = previous.id_b_AoQ  ;
  tgt.id_b_z      = previous.id_b_z    ;
  tgt.id_b_x2AoQ  = previous.id_b_x2AoQ;
  for(int i=0;i<5;i++)
    {
      tgt.id_b_x4AoQ_Z[i]= previous.id_b_x4AoQ_Z[i];  
      tgt.id_b_z_AoQ[i]  = previous.id_b_z_AoQ[i]  ;
      tgt.id_b_music_z[i]= previous.id_b_music_z[i];
    }
  tgt.timestamp=previous.timestamp;
  tgt.ts=previous.ts;  
  tgt.ts2=previous.ts2;  




    

}


ClassImp(TXRSUserProc)
