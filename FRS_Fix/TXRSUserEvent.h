#ifndef TXRSUSEREVENT_H
#define TXRSUSEREVENT_H

#include "TGo4EventElement.h"

class TXRSUserProc;
class TGo4FileSource;

class TXRSUserEvent : public TGo4EventElement 
{
public:
  TXRSUserEvent();
  TXRSUserEvent(const char* name);
  virtual ~TXRSUserEvent();
  /**
   * Method called by the event owner (analysis step) to fill the
   * event element from the set event source. Event source can
   * be the source of the analysis step (if this is a raw event) 
   * or the event processor (if this is a reduced event).
   */
  //virtual Int_t Fill();
  
  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */


  Int_t EventFlag;

  // MRTOF part :
  Int_t timestamp_mrtof;
  
  Int_t mrtof_start;
  Int_t mrtof_stop;
  Float_t mrtof_tof;
  Float_t mrtof_si_e1;
  Float_t mrtof_si_e2;
  Float_t mrtof_si_e3;
  Float_t mrtof_si_e4;
  Float_t mrtof_si_e5;
  
  // MUSIC part
  Int_t         music1_anodes_cnt;
  Int_t         music2_anodes_cnt;
  Int_t         music3_anodes_cnt;
  Bool_t        music_b_e1[8];
  Bool_t        music_b_t1[8];
  Bool_t        music_b_e2[8];
  Bool_t        music_b_t2[8];
  Bool_t        music_b_e3[4];
  Bool_t        music_b_t3[4];
  Bool_t        b_de1;
  Bool_t        b_de2;
  Bool_t        b_de3;
  Float_t       de[3];
  Float_t       de_cor[3];
  Bool_t        b_dt3;
  Float_t       x1_mean;
  Bool_t        b_decor;
      
  //SCI part
  Float_t       sci_l[12];  
  Float_t       sci_r[12];  
  Float_t       sci_e[12];  
  Float_t       sci_tx[12];  
  Float_t       sci_x[12];
  Float_t       itag_42_e;
  Float_t       itag_43_e;
  Float_t       sci_u5, sci_d5;
  Float_t       sci_ty5;  
  Float_t       sci_y5;  
  Float_t       sci_tofll2;
  Float_t       sci_tofrr2;
  Float_t       sci_tof2;
  Float_t       sci_tofll3;
  Float_t       sci_tofrr3;
  Float_t       sci_tof3;
  Float_t       sci_tofll4;
  Float_t       sci_tofrr4;
  Float_t       sci_tof4;  
  Float_t       sci_veto_l[3];
  Float_t       sci_veto_r[3];
  Float_t       sci_veto_e[3];


  Bool_t        sci_b_l[12];  
  Bool_t        sci_b_r[12];  
  Bool_t        sci_b_e[12];  
  Bool_t        sci_b_tx[12];  
  Bool_t        sci_b_x[12];  
  Bool_t        sci_b_u5, sci_b_d5;
  Bool_t        sci_b_ty5;
  Bool_t        sci_b_y5;
  Bool_t        sci_b_tofll2;
  Bool_t        sci_b_tofll3;
  Bool_t        sci_b_tofll4;
  Bool_t        sci_b_tofrr2;
  Bool_t        sci_b_tofrr3;
  Bool_t        sci_b_tofrr4;
  Bool_t        sci_b_detof;
  Bool_t        sci_b_veto_l[3];  
  Bool_t        sci_b_veto_r[4];  
  Bool_t        sci_b_veto_e[3];  

  // ID part
      
  Float_t       id_x2;
  Float_t       id_y2;
  Float_t       id_a2;
  Float_t       id_b2;
  Float_t       id_x4;
  Float_t       id_y4;
  Float_t       id_a4;
  Float_t       id_b4;
  Float_t       id_x8;
  Float_t       id_y8;
  Float_t       id_a8;
  Float_t       id_b8;
      
  Bool_t        id_b_x2;
  Bool_t        id_b_x4;
  Bool_t        id_b_x8;
  Bool_t        id_b_detof2;
      
  Float_t       id_brho[2];      /* position-corr. BRho      */
  Float_t       id_rho[2];       /* Position-corrected Rho   */
      
  Float_t       id_beta;        /* Beta from TOF            */
  Float_t       id_beta3;        /* Beta from TOF            */
  Float_t       id_gamma;       /* Gamma from TOF           */
  Float_t       id_AoQ;
  Float_t       id_AoQ_corr;
      
  Float_t       id_v_cor;       /* Velocity correction  */
  Float_t       id_v_cor2;      /* Velocity correction TUM 2 */
  Float_t       id_v_cor3;      /* Velocity correction Old Music */
  Float_t       id_z;
  Float_t       id_z2;
  Float_t       id_z3;
  Float_t       id_energy_geL;
  Float_t       id_tac_41_geL;
  Float_t       id_stopper_x;
  Int_t         id_energy_geL_raw;
  Int_t         id_tac_41_geL_raw;
  Int_t         id_trigger;
  Int_t         id_scal_seconds;
  Int_t         id_scal_geL;
  Int_t         id_scal_sc21;
  Int_t         id_scal_sc41;
  Int_t         id_scal_sc42;
  Int_t         id_scal_sc43;
  Int_t         id_scal_sc81;

  Bool_t        id_b_AoQ;
  Bool_t        id_b_z;
  Bool_t        id_b_z2;
  Bool_t        id_b_z3;
  Bool_t        id_b_x2AoQ;
  Bool_t        id_b_x4AoQ_Z[5]; 
  Bool_t        id_b_z_AoQ[5];
  Bool_t        id_b_music_z[5];


  Long64_t firsttimestamp;
  Bool_t firstevent;


  Long64_t      timestamp;  // absolute time stamp value
  Double_t      ts;  // relative time from start of the spill 
  Double_t      ts2;  // relative time from start of the spill does not reset at end extraction


  virtual void Clear(Option_t *t="");  
  virtual Int_t Init();  

  //Variables to be converted to Analysis ROOT Files for s323

private:
  //  TXRSUserProc   *fxProc;     //! This is processor
  //  TGo4FileSource *fxFileSrc;  //! This is file source
  ClassDef(TXRSUserEvent,1)         

};

#endif //TXRSUSEREVENT_H
