#include "TXRSUserEvent.h"

#include "Riostream.h"
#include "TGo4FileSource.h"
#include "TXRSUserProc.h"

TXRSUserEvent::TXRSUserEvent() : TGo4EventElement("FRSUserEvent")//, fxProc(0), fxFileSrc(0) 
{  }

TXRSUserEvent::TXRSUserEvent(const char* name) : TGo4EventElement(name)//, fxProc(0), fxFileSrc(0) 
{ }

TXRSUserEvent::~TXRSUserEvent() 
{ }

Int_t TXRSUserEvent::Init() { 
  Int_t rev=0;
  Clear();
  // if(CheckEventSource("TXRSUserProc")){
  //   fxProc = (TXRSUserProc*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by " << fxProc->GetName() << " ****" << std::endl;
  // } else
  // if(CheckEventSource("TGo4FileSource")) {
  //   fxFileSrc = (TGo4FileSource*)GetEventSource();
  //   std::cout << "**** " << GetName() << " will be filled by File Source ****"<< std::endl;
  // } else rev=1;
  return rev;
}

// Int_t TXRSUserEvent::Fill() { 
//   Clear();
//   if(fxProc) fxProc->FRSUserProc(this); else  // user event processing method
//   if(fxFileSrc)fxFileSrc->BuildEvent(this);   // method from framework to restore event from file
//   return 0;
// }

void TXRSUserEvent::Clear(Option_t *t) 
{
  EventFlag = 0;

  // MRTOF part :
  mrtof_start = 0;
  mrtof_stop  = 0;
  timestamp_mrtof = 0;

  mrtof_tof=0.;
  mrtof_si_e1=0.;
  mrtof_si_e2=0.;
  mrtof_si_e3=0.;
  mrtof_si_e4=0.;
  mrtof_si_e5=0.;

  // MUSIC part
  music1_anodes_cnt = 0;
  music2_anodes_cnt = 0;
  music3_anodes_cnt = 0;
  for(int i=0;i<8;++i)
    {
      music_b_e1[i]=false;
      music_b_t1[i]=false;
      music_b_e2[i]=false;
      music_b_t2[i]=false;
    }
  for(int i=0;i<4;++i)
    {
      music_b_e3[i]=false;
      music_b_t3[i]=false;
    }
  b_de1=false;
  b_de2=false;
  b_de3=false;
  for(int i=0;i<3;++i)
    {
      de[i]=0.;
      de_cor[i]=0.;
    }
  b_dt3=false;
  x1_mean=0.;
  b_decor=false;
      
  //SCI part
  for(int i=0;i<12;++i)
    {
      sci_l[i] =0.;  
      sci_r[i] =0.;  
      sci_e[i] =0.;  
      sci_tx[i]=0.;  
      sci_x[i] =0.;

      sci_b_l[i]=0.;  
      sci_b_r[i]=0.;  
      sci_b_e[i]=0.;  
      sci_b_tx[i]=0.;  
      sci_b_x[i]=0.;  
    }
  itag_42_e=0.;
  itag_43_e=0.;
  sci_u5=0.;
  sci_d5=0.;
  sci_ty5=0.;  
  sci_y5=0.;  
  sci_tofll2=0.;
  sci_tofrr2=0.;
  sci_tof2=0.;
  sci_tofll3=0.;
  sci_tofrr3=0.;
  sci_tof3=0.;
  sci_tofll4=0.;
  sci_tofrr4=0.;
  sci_tof4=0.;
  for(int i=0;i<3;++i)
    {
      sci_veto_l[i]=0.;
      sci_veto_r[i]=0.;
      sci_veto_e[i]=0.;
    }

  sci_b_u5=false;
  sci_b_d5=false;
  sci_b_ty5=false;
  sci_b_y5=false;
  sci_b_tofll2=false;
  sci_b_tofrr2=false;
  sci_b_tofll3=false;
  sci_b_tofrr3=false;
  sci_b_tofll4=false;
  sci_b_tofrr4=false;
  sci_b_detof=false;

  for(int i=0;i<3;++i)
    {
      sci_b_veto_l[i]=false;  
      sci_b_veto_e[i]=false;  
    }
  for(int i=0;i<4;++i)
    sci_b_veto_r[i]=false;
  // ID part
      
  id_x2=0.;
  id_y2=0.;
  id_a2=0.;
  id_b2=0.;
  id_x4=0.;
  id_y4=0.;
  id_a4=0.;
  id_b4=0.;
  id_x8=0.;
  id_y8=0.;
  id_a8=0.;
  id_b8=0.;
      
  id_b_x2=false;
  id_b_x4=false;
  id_b_x8=false;
  id_b_detof2=false;

  for(int i=0;i<2;++i)
    {
      id_brho[i]=0.;      /* position-corr. BRho      */
      id_rho[i]=0.;       /* Position-corrected Rho   */
    }
  id_beta=0.;        /* Beta from TOF            */
  id_beta3=0.;        /* Beta from TOF            */
  id_gamma=0.;       /* Gamma from TOF           */
  id_AoQ=0.;
  id_AoQ_corr=0.;
      
  id_v_cor=0.;	  /* Velocity correction  */
  id_v_cor2=0.;	  /* Velocity correction TUM 2 */
  id_v_cor3=0.;	  /* Velocity correction Old Music */
  id_z=0.;
  id_z2=0.;
  id_z3=0.;
  id_energy_geL=0.;
  id_tac_41_geL=0.;
  id_stopper_x=0.;
  id_energy_geL_raw=0;
  id_tac_41_geL_raw=0;
  id_trigger=0;
  id_scal_seconds=0;
  id_scal_geL=0;
  id_scal_sc21=0;
  id_scal_sc41=0;
  id_scal_sc42=0;
  id_scal_sc43=0;
  id_scal_sc81=0;


  id_b_AoQ=false;
  id_b_z=false;
  id_b_z2=false;
  id_b_z3=false;
  id_b_x2AoQ=false;
  for(int i=0;i<5;++i)
    {
      id_b_x4AoQ_Z[i]=false; 
      id_b_z_AoQ[i]=false;
      id_b_music_z[i]=false;
    }

  firsttimestamp=0;
  firstevent=false;
  

  timestamp=0;  // absolute time stamp value
  ts=0.;  // relative time from start of the spill 
  ts2=0.;  // relative time from start of the spill does not reset at end extraction

	
}

ClassImp(TXRSUserEvent)
