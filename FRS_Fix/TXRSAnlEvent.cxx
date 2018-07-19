#include "TXRSAnlEvent.h"

#include "Riostream.h"
#include "TGo4FileSource.h"
#include "TXRSAnlProc.h"

TXRSAnlEvent::TXRSAnlEvent() :TGo4EventElement("FRSAnlEvent")//, fxProc(0), fxFileSrc(0) { 
{ }

TXRSAnlEvent::TXRSAnlEvent(const char* name) :TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) {
{ }

TXRSAnlEvent::~TXRSAnlEvent()
{ }

Int_t TXRSAnlEvent::Init()
{ 
  //--- check for different source types
  Int_t rev = 0;
  // if(CheckEventSource("TXRSAnlProc")) {
  //   fxProc = (TXRSAnlProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else 
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // }
  // else rev=1;
  return rev;
}


// Int_t TXRSAnlEvent::Fill() 
// {
//    Clear();
//    if(fxProc) fxProc->FRSEventAnalysis(this); else
//    if(fxFileSrc)fxFileSrc->BuildEvent(this); // method from framework to restore event from file

//    return 0;
// }

void TXRSAnlEvent::Clear(Option_t *t)
{
  // MUSIC part  
  EventFlag = 0;
  
  for(int i=0;i<8;i++)
    {
      music_b_e1[i] = kFALSE;
      music_b_t1[i] = kFALSE;
      music_b_e2[i] = kFALSE;
      music_b_t2[i] = kFALSE;
    }
  for(int i=0;i<4;i++)
    {
      music_b_t3[i] = kFALSE;
      music_b_e3[i] = kFALSE;
    }
  music1_anodes_cnt = 0;
  music2_anodes_cnt = 0;
  music3_anodes_cnt = 0;
  b_de1 = kFALSE;
  b_de2 = kFALSE;
  b_de3 = kFALSE;
  b_dt3 = kFALSE;
  de[0] = 0.;
  de[1] = 0.;
  de[2] = 0.;
  de_cor[0] = 0.;
  de_cor[1] = 0.;
  de_cor[2] = 0.;
  
  x1_mean = 0.;
  b_decor = kFALSE;
  
  // SCI part
  for(int i=0;i<12;i++)
    {
      sci_l[i] = 1.;
      sci_r[i] = 0.;
      sci_e[i] = 0.;
      sci_tx[i] = 0.;
      sci_x[i] = 0.;
      sci_b_l[i] = kFALSE;
      sci_b_r[i] = kFALSE;
      sci_b_e[i] = kFALSE;
      sci_b_tx[i] = kFALSE;
      sci_b_x[i] = kFALSE;
    }
  
  // veto SCI 
  for(int i=0;i<3;i++)
    {
      sci_veto_l[i] = 1.;
      sci_veto_r[i] = 1.;
      sci_b_veto_l[i] = kFALSE;
      sci_b_veto_r[i] = kFALSE;
      sci_b_veto_e[i] = kFALSE;
    }
  
  sci_b_veto_r[3] = kTRUE; // veto3 missing channel

  itag_42_e =0.;
  itag_43_e =0.;

  sci_u5 = 1.; sci_d5 = 1.;
  sci_b_u5 = kFALSE; sci_b_d5 = kFALSE;
  
  sci_ty5 = 0.;
  sci_b_ty5 = kFALSE;
  sci_y5 = 0.;
  sci_b_y5 = kFALSE;
  
  sci_tofll2 = 0.; sci_b_tofll2 = kFALSE;
  sci_tofrr2 = 0.; sci_b_tofrr2 = kFALSE;
  sci_tofll3 = 0.; sci_b_tofll3 = kFALSE;
  sci_tofrr3 = 0.; sci_b_tofrr3 = kFALSE;
  sci_tofll4 = 0.; sci_b_tofll4 = kFALSE;
  sci_tofrr4 = 0.; sci_b_tofrr4 = kFALSE;
  sci_tof2 = 0.; sci_tof3 = 0.; sci_tof4 = 0.;

  sci_b_detof = kFALSE;
  
  // ID part
  
  /*  some dummy values...           */
  id_x2=999.0;
  id_y2=999.0;
  id_a2=999.0;
  id_b2=999.0;
  id_x4=999.0;
  id_y4=999.0;
  id_a4=999.0;
  id_b4=999.0;
  
  id_b_x2 = kFALSE;
  id_b_x4 = kFALSE;
  id_b_detof2 = kFALSE;
  
  for (int i=0;i<2;i++)
    {
      id_brho[i] = 0.;  
      id_rho[i] = 0.;
    }
  id_beta     = 0.; 
  id_beta3     = 0.; 
  id_gamma    = 1.;
  id_AoQ      = 0.0;
  id_AoQ_corr = 0.0;
  
  id_v_cor = 0.;
  id_v_cor2 = 0.;
  id_z = 0.;
  id_z2 = 0.;
  id_z3 = 0;
  id_energy_geL = 0.;
  id_tac_41_geL = 0.;
  id_energy_geL_raw = 0;
  id_tac_41_geL_raw = 0;
  id_stopper_x = -666;
  id_trigger = 0;
  id_scal_seconds = 0;
  id_scal_geL = 0;
  id_scal_sc21 = 0;
  id_scal_sc41 = 0;
  id_scal_sc42 = 0;
  id_scal_sc43 = 0;
  id_scal_sc81 = 0;

  
  id_b_AoQ    = kFALSE;
  id_b_z      = kFALSE;
  id_b_x2AoQ  = kFALSE;
  for(int i=0;i<5;i++)
    {
      id_b_x4AoQ_Z[i] = kFALSE;  
      id_b_z_AoQ[i] = kFALSE;
      id_b_music_z[i] = kFALSE;
    }
  timestamp=0;
  ts=0;  
  ts2=0;

  
  mrtof_start = 0;
  mrtof_stop = 0;
  mrtof_tof = 0.;
  
  mrtof_si_e1 = 0;
  mrtof_si_e2 = 0;
  mrtof_si_e3 = 0;
  mrtof_si_e4 = 0;
  mrtof_si_e5 = 0;
  
}

ClassImp(TXRSAnlEvent)

