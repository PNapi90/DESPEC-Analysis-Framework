#ifndef FRS_DETECTOR_SYSTEM_H
#define FRS_DETECTOR_SYSTEM_H

#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TH1.h"
#include "TMap.h"
#include "TCollection.h"

#include "Riostream.h"

#include "Detector_System.cxx"

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4StepFactory.h"
#include "TGo4Analysis.h"


#include "TXRSParameter.h"
//#include "TXRSAnalysis.h"

//#include "TXRSSortEvent.h"
//#include "TXRSCalibrEvent.h"
//#include "TXRSAnlEvent.h"

#include <bitset>

#include <sstream>
#include <fstream>
#include <map>

class FRS_Detector_System : public Detector_System{

private:

  int* pdata;
  
  Int_t test;
  
  Bool_t bDrawHist = false;
  
  /**************************************************************/
  /***UNPACK STUFF***/
    
  bool firstTS[3] ;
  Long64_t previousTimeStamp[3];
  Long64_t currentTimeStamp;
  
  Int_t EventFlag;
  
  UInt_t vme0[21][32];         // FRS crate                                
  UInt_t vme1[21][32];         // TPC crate 
  UInt_t vme3[21][32];         // Mtof crate

  Int_t vme5[21][32];          //Main Crate or User Crate

  Int_t vme2scaler[32];         // User Crate Messhure
  Int_t vme3scaler[32];         // User Crate Messhure

  Int_t vme2s[32][10];           // User TDC (V1290) 
  Int_t vme2s_trailing[32][10];  // User TDC (V1290) 
  Int_t nhit5[32][2];            // multiplicity (V1290)

  Int_t vme3s_MT[32][10];           // Mtof TDC (V1290) 
  Int_t vme3s_MT_trailing[32][10];  // Mtof TDC (V1290) 
  Int_t vme3_MT_nhit5[32][2];       // multiplicity (V1290)
  
  Int_t vme4[21][32] ; // SOFIA crate temp need multiplicity for both MDPP and vftx
  
  Int_t         qlength;         /* From event header     */
  Int_t         qtype;           /*                       */
  Int_t         qsubtype;        /*                       */
  Int_t         qdummy;          /*                       */
  Int_t         qtrigger;        /*                       */
  Int_t         qevent_nr;       /*                       */


  /*******************************************************************/
  /***SORT STUFF***/

  Long64_t StartOfSpilTime;
  Long64_t StartOfSpilTime2; //does not reset at end of extraction
  Long64_t PreviousTS;
  
  Int_t counter; 

    // time stamp data  
  Int_t         ts_id;                                
  Int_t         ts_word[4]; //for the titris time stammping
  Int_t         tsys_word[3]; //for the system time
  Long64_t      timestamp;  // absolute time stamp value
  Double_t      timespill;  // relative time from start of the spill 
  Double_t      timespill2;  // relative time from start of the spill does not reset at end extraction
  Double_t      systemtime_ms;
  Double_t      systemtime_s;

  Int_t         pattern;
  Int_t         trigger;

  // scaler readings     
  UInt_t sc_long[64]; //changed from 32 to 64 (10.07.2018)
  UInt_t sc_long2[32];


  // SEETRAM calib IC energy
  //      Int_t         ic_de;           /*                          */
      
     
  // part of MW parameter
  Int_t         mw_an[13];       /*  anode time              */
  Int_t         mw_xl[13];       /*  Rohdaten                */
  Int_t         mw_xr[13];       /*                          */
  Int_t         mw_yu[13];       /*                          */ 
  Int_t         mw_yd[13];       /*                          */

  // TPC part //(HAPOL-25/03/06) 6 TPCs each one with 2 delay lines each!!
  //7 TPCs (4 at S2, 2 at S4 and one at S3) 03.07.2018 SB
  Int_t tpc_l[7][2];
  Int_t tpc_r[7][2];
  Int_t tpc_lt[7][2];
  Int_t tpc_rt[7][2];

  // [index][anode_no]
  Int_t tpc_dt[7][4];
  Int_t tpc_a[7][4];


      
  // SCI part
  Int_t         de_21l;          /* dE SCI21 left            */  
  Int_t         de_21r;          /* de SCI21 right           */ 
  Int_t         de_41l;          /* dE SCI41 left            */
  Int_t         de_41r;          /* dE SCI41 right           */
  Int_t         de_41u;          /* dE SCI41 up              */
  Int_t         de_41d;          /* dE SCI41 down            */
  Int_t         de_42l;          /* de SCI42 left            */
  Int_t         de_42r;          /* de SCI42 right           */
  Int_t         de_43l;          /* de SCI43 left            */
  Int_t         de_43r;          /* de SCI43 right           */
  Int_t         de_81l;          /* de SCI81 left            */
  Int_t         de_81r;          /* de SCI81 right           */  
  Int_t         de_21ld;         /* dE SCI21 left delayed    */  
  Int_t         de_21rd;         /* de SCI21 right delayed   */ 
  Int_t         de_v1l;          /* dE veto1 left            */  
  Int_t         de_v1r;          /* de veto1 right           */ 
  Int_t         de_v2l;          /* dE veto2 left            */
  Int_t         de_v2r;          /* dE veto2 right           */
  Int_t         de_v3;           /* dE veto3                 */


  Int_t         dt_21l_21r;      /*                          */ 
  Int_t         dt_41l_41r;      /*                          */ 
  Int_t         dt_21l_41l;      /*                          */
  Int_t         dt_21r_41r;      /*                          */
  Int_t         dt_42l_42r;      /*                          */
  Int_t         dt_43l_43r;      /*                          */
  Int_t         dt_42l_21l;      /*                          */
  Int_t         dt_42r_21r;      /*                          */
  Int_t         dt_41u_41d;      /*                          */
  Int_t         dt_81l_81r;
  Int_t         dt_21l_81l;
  Int_t         dt_21r_81r;

  // User multihit TDC
		
  Int_t tdc_sc41l[10];
  Int_t tdc_sc41r[10];
  Int_t tdc_sc21l[10];
  Int_t tdc_sc21r[10];
  Int_t tdc_sc42l[10];
  Int_t tdc_sc42r[10];
  Int_t tdc_sc43l[10];
  Int_t tdc_sc43r[10];
  Int_t tdc_sc81l[10];
  Int_t tdc_sc81r[10];

  // MUSIC1 part
  Int_t         music_e1[8];     /* Raw energy signals       */
  Int_t         music_t1[8];     /* Raw drift time           */
  Int_t         music_pres[3];   /* Music Druck              */
  Int_t         music_temp[3];   /* Music Temperatur         */

  // MUSIC2 part
  Int_t         music_e2[8];     /* Raw energy signals       */
  Int_t         music_t2[8];     /* Raw drift time           */
     
  // MUSIC3 part (OLD MUSIC)
  Int_t         music_e3[4];     /* Raw energy signals       */
  Int_t         music_t3[4];     /* Raw drift times          */

  // Single Anode
  Int_t SingleAnode_adc;
  
  //Si detectors
  Int_t si_adc1;
  Int_t si_adc2;
  Int_t si_adc3;
  Int_t si_adc4;
  Int_t si_adc5;
  Int_t dssd_adc[32];

  //Channeltron detector
  Int_t 	ct_signal;
  Int_t         ct_time;
  Bool_t 	ct_trigger_DU;
  Bool_t 	ct_trigger_SY;
  
  //Electron current measurement
  Int_t	ec_signal;

  // MR-TOF
  Int_t mrtof_start;
  Int_t mrtof_stop;
  Int_t mrtof_stopDelay;

 /*********************************************************************/
 /***CALIBRATION STUFF***/
       
  // MON data declarations
      
  int check_first_event[2];
  int scaler_channel_10khz;//YT 20Jun
  Long64_t  scaler_time_count[2]  ; 
  Long64_t  scaler_spill_count[2] ; //UInt_t
  Long64_t  scaler_time_check_last[2] ;//UInt_t
  Long64_t  scaler_spill_check_last[2];//UInt_t 
  Long64_t  check_increase_time[64]   ;//UInt_t 
  Long64_t  check_increase_spill[64]  ;//UInt_t
  Long64_t  scaler_increase_event[64] ;//UInt_t
  Long64_t  scaler_last_event[64] ;


  TGo4WinCond   *cMW_XSUM[13];
  TGo4WinCond   *cMW_YSUM[13];  

  ////condtition for control sum
  TGo4WinCond *cTPC_CSUM[7][4];
  TGo4PolyCond *cTPC_XY[7];

  //**** keep values from previous event
  Float_t       focx_s2m;
  Float_t       focy_s2m;
  Float_t       angle_x_s2m;
  Float_t       angle_y_s2m;
 
 
 
  Bool_t        fbFirstEvent;
  //unsigned int  scaler_save[64];
  Long64_t         scaler_save[64];
  UInt_t         firstsec;
  UInt_t         firsttenthsec;
  UInt_t         firsthundrethsec;	//mik
  UInt_t         firstcycle;
  UInt_t         firstseetram;
  int scalercycle_Sec;
  int scalercycle_TenthSec;
  int scalercycle_HundrethSec;
  int scalercycle_Cycle;
  int scalercycle_Seetram;

  Float_t       dtime;
  Float_t       dt_last;
  Float_t       dt_diff;
 
 
 
  Long64_t  check_total_sc41 ;
  Long64_t  check_total_sc21 ;
  Long64_t  check_total_seetram ;
  Long64_t  check_total_mrtof_start ;
  Long64_t  check_total_mrtof_stop ;
 
 
   UInt_t freeTrig;
  UInt_t acptTrig;
  Long64_t mon_inc[64];

  // MON part
  Int_t         seconds;
  Int_t         tenthsecs;
  Int_t         hundrethsecs;	//mik
  Int_t         extraction_cycle;
  Int_t         coin[16];
  Float_t       seetram;
      
  // MW part
  Float_t       mw_xsum[13];     /*                          */
  Float_t       mw_ysum[13];     /*                          */
      
  Float_t       mw_x[13];        /*                          */
  Float_t       mw_y[13];        /*                          */
  Float_t       mw_wire[13];     /* special for Helmut       */

  Float_t       z_x_s2;          
  Float_t       z_y_s2;          
  Float_t       z_x_s4;          
  Float_t       z_y_s4;          
  Float_t       z_x_s8;          
  Float_t       z_y_s8;          
 
  Bool_t        b_mw_xsum[13];   /*  wc on sum               */
  Bool_t        b_mw_ysum[13];   /*                          */

  Float_t       focx_s2;         /*  FRS foci                */  
  Float_t       focy_s2;         /*                          */  
  Float_t       focx_s4;         /*                          */  
  Float_t       focy_s4;         /*                          */  
  Float_t       focx_s8;         /*  FRS foci                */  
  Float_t       focy_s8;         /*                          */
  Float_t       angle_x_s2;      /*                          */
  Float_t       angle_y_s2;      /*                          */
  Float_t       angle_x_s4;      /*                          */
  Float_t       angle_y_s4;      /*                          */
  Float_t       angle_x_s8;      /*                          */
  Float_t       angle_y_s8;      /*                          */

  // TPC part
  Int_t tpc_csum[7][4];
  Float_t tpc_x[7];
  Float_t tpc_y[7];
  Bool_t b_tpc_csum[7][4];
  Bool_t b_tpc_xy[7];

  //TPCs 21 & 22 @ S2 focus
  Float_t tpc_x_s2_foc_21_22;
  Float_t tpc_y_s2_foc_21_22;
  Float_t tpc_angle_x_s2_foc_21_22;
  Float_t tpc_angle_y_s2_foc_21_22;

  //TPCs 23 & 24 @ S2 focus
  Float_t tpc_x_s2_foc_23_24;
  Float_t tpc_y_s2_foc_23_24;
  Float_t tpc_angle_x_s2_foc_23_24;
  Float_t tpc_angle_y_s2_foc_23_24;

  //TPCs 22 & 24 @ S2 focus
  Float_t tpc_x_s2_foc_22_24;
  Float_t tpc_y_s2_foc_22_24;
  Float_t tpc_angle_x_s2_foc_22_24;
  Float_t tpc_angle_y_s2_foc_22_24;

  //TPCs 3 & 4 @ S2 focus (old tracking)
  Float_t tpc_x_s2_foc;
  Float_t tpc_y_s2_foc;
  Float_t tpc_angle_x_s2_foc;
  Float_t tpc_angle_y_s2_foc;
   
  //TPCs 41 & 42 @ s4 focus
  Float_t tpc_x_s4;
  Float_t tpc_y_s4;
  Float_t tpc_angle_x_s4;
  Float_t tpc_angle_y_s4;

  //TPCs 5 & 6 @ S4 OTPC (exp s388)
  Float_t tpc_x_s4_target2;
  Float_t tpc_y_s4_target2;
  

  //TPCs 3 & 4 @ S2 first Si tracking detector (exp s388)
  Float_t tpc_x_s2_target1;
  Float_t tpc_y_s2_target1;
  //      Float_t tpc_angle_x_s4_target2;
  //      Float_t tpc_angle_y_s4_target2;


  Float_t       tpc_sc21_x;      /* SC21 x                    */
  Float_t       tpc_sc41_x;      /* SC41 x                    */
  Float_t       tpc_sc21_y;      /* SC21 y                    */
  Float_t       tpc_sc41_y;      /* SC41 y                    */


  Float_t       sc21_x;          /* SC21                     */
  Float_t       sc22_x;          /* SC22 (LAND finger)       */
  Float_t       sc41_x;          /* SC41                     */
  Float_t       itag_stopper_x;  /* ITAG Stopper             */
  Float_t       sc42_x;          /* SC42                     */
  Float_t       sc43_x;          /* SC43                     */
  Float_t       sc21_y;          /* tracked SC21 Y pos       */
  Float_t       sc41_y;          /* SC41 Y                   */
  Float_t       sc42_y;          /* SC42 Y                   */
  Float_t       sc43_y;          /* SC43 Y                   */
  Float_t       sc81_x;          /* SC81          positions  */
      
  Float_t       music1_x1;       /* parameters to calibrate  */
  Float_t       music1_x2;       /* parameters to calibrate  */
  Float_t       music1_x3;       /* parameters to calibrate  */
  Float_t       music1_x4;       /* parameters to calibrate  */
  Float_t       music1_y1;       /* parameters to calibrate  */
  Float_t       music1_y2;       /* parameters to calibrate  */
  Float_t       music1_y3;       /* parameters to calibrate  */
  Float_t       music1_y4;       /* parameters to calibrate  */
  Float_t       music2_x;        /* MUSIC1,2 positions       */


  //Si detectors
  Float_t si_e1;
  Float_t si_e2;
  Float_t si_e3;
  Float_t si_e4;
  Float_t si_e5;

  Float_t dssd_e[32]; //[3][2][16]
  Float_t dssd_maxenergy; //[3] 
  Int_t dssd_maxindex;  //[3]
      
  Float_t dssd_xmaxenergy;
  Int_t dssd_xmaxindex;
  Float_t dssd_ymaxenergy;
  Int_t dssd_ymaxindex;
 
 /********************************************************************/
 /***Analysis Parameters***/
 
  //TXRSAnalysis* an;
 
  TModParameter* ModSetup;
  TMWParameter* mw;
  TTPCParameter* tpc;
  TSIParameter* si;
  TModParameter* ElecMod;
  
  TMUSICParameter* music;
  TSCIParameter* sci;
  TXRSParameter* frs;
  TIDParameter* id;
  TMRTOFMSParameter* mrtof;
  
  TXRSParameter         *fFRSPar;    //!
  TMWParameter          *fMWPar;     //!
  TTPCParameter         *fTPCPar;     //!
  TMUSICParameter       *fMUSICPar;  //!
  TSCIParameter         *fSCIPar;    //!
  TIDParameter          *fIDPar;     //!
  TSIParameter          *fSIPar;     //!
  TModParameter * ModPar; //!
  TMRTOFMSParameter* MRtofPar; //!
  
  
  
  // MUSIC data

  TGo4WinCond   *cMusic1_E[8];
  TGo4WinCond   *cMusic1_T[8];
  TGo4WinCond   *cMusic2_E[8];
  TGo4WinCond   *cMusic2_T[8];
  TGo4WinCond   *cMusic3_T[4];
  TGo4WinCond   *cMusic3_E[4];
  TGo4WinCond   *cMusic3_dec;


  // SCI data
  
  TGo4WinCond   *cSCI_L[12];
  TGo4WinCond   *cSCI_R[12];
  TGo4WinCond   *cSCI_E[12];
  TGo4WinCond   *cSCI_Tx[12];
  TGo4WinCond   *cSCI_X[12];
  
  TGo4WinCond   *cSCI_TofLL2;
  TGo4WinCond   *cSCI_TofRR2;
  TGo4WinCond   *cSCI_TofLL3;
  TGo4WinCond   *cSCI_TofRR3;
  TGo4WinCond   *cSCI_TofLL4;
  TGo4WinCond   *cSCI_TofRR4;
  
  TGo4PolyCond  *cSCI_detof; 

      
  TGo4WinCond   *cID_x2;
  TGo4WinCond   *cID_x4;
  TGo4PolyCond  *cID_dEToF;
  TGo4PolyCond  *cID_x2AoQ[6];
  TGo4PolyCond  *cID_Z_AoQ[5];
  TGo4PolyCond  *cID_x4AoQ_Z[5]; 
  TGo4WinCond   *cID_Z_Z[5]; 
     
  TGo4PolyCond  *cMUSIC_13;
 

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
  Bool_t        sci_b_tofrr2;
  Bool_t        sci_b_tofll3;
  Bool_t        sci_b_tofrr3;
  Bool_t        sci_b_tofll4;
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

  Double_t      ts;  // relative time from start of the spill 
  Double_t      ts2;  // relative time from start of the spill does not reset at end extraction

  // MRTOF part :
  Float_t mrtof_tof;

  Float_t mrtof_si_e1;
  Float_t mrtof_si_e2;
  Float_t mrtof_si_e3;
  Float_t mrtof_si_e4;
  Float_t mrtof_si_e5;


/**********************************************************************/


/*   // MUSIC PARAMETERS //
  
  Float_t* dE;
  Float_t* dE_cor;

  // SCINTILLATOR PARAMETERS //
  
  Float_t* sci_l;
  Float_t* sci_r;
  Float_t* sci_e;
  Float_t* sci_tx;
  Float_t* sci_x;
  
  
  Float_t sci_tofll2;
  Float_t sci_tofll3;
  Float_t sci_tof2;
  Float_t sci_tofrr2;
  Float_t sci_tofrr3;
  Float_t sci_tof3;
  
  // ID PARAMETERS //
  
  Float_t ID_x2;
  Float_t ID_y2;
  Float_t ID_a2;
  Float_t ID_b2;
  
  Float_t ID_x4;
  Float_t ID_y4;
  Float_t ID_a4;
  Float_t ID_b4;
  
  Float_t* ID_brho;
  Float_t* ID_rho;
  
  Float_t beta;
  Float_t beta3;
  Float_t gamma;
  Float_t AoQ;
  Float_t AoQ_corr;
  
  Float_t z;
  Float_t z2;
  Float_t z3;
  
  Float_t timestamp;
  Float_t ts;
  Float_t ts2;   */
  
  void Setup_Parameters();
  void FRS_Unpack(TGo4MbsSubEvent* psubevt);
  void FRS_Sort();
  void FRS_Calib();
  void FRS_Anal();
  
  Int_t getbits(Int_t, int, int, int);

  Int_t get2bits(Int_t, int, int, int);
 
  Float_t rand0_5();


public:
  FRS_Detector_System();
  ~FRS_Detector_System();
  //TXRSUserProc(const char* name);
  //virtual ~FRS_Detector_System() ;

    //void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*);
    void Process_FRS(TGo4MbsSubEvent* psubevt);

    void Process_MBS(int*);
    void get_Event_data(Raw_Event*);
    int* get_pdata();

	
    unsigned long** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;}
    void write(){return;};
    void set_Gain_Match_Filename(std::string){return;};

  //TModParameter* Setup ;

};

#endif //FRS_DETECTOR_SYSTEM_H
