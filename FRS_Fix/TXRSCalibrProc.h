#ifndef TXRSCALIBRPROCESSOR_H
#define TXRSCALIBRPROCESSOR_H

#include "TXRSBasicProc.h"
#include  <iostream>

#include  "TMath.h"
#include  "TRandom3.h"

#include "TXRSCalibrEvent.h"

#include "TXRSParameter.h"

class TXRSCalibrEvent;
class TXRSSortEvent;

class TXRSCalibrProc : public TXRSBasicProc {
public:
  TXRSCalibrProc() ;
  TXRSCalibrProc(const char* name);
  virtual ~TXRSCalibrProc() ;
   
  //void FRSCalibr(TXRSCalibrEvent* tgt);
  // event processing function, default name
  Bool_t BuildEvent(TGo4EventElement* output);
  //Bool_t FillHistograms(TXRSCalibrEvent* event);
      
  void InitProcessor();
  Bool_t bDrawHist;

  Long64_t  check_total_sc41 ;
  Long64_t  check_total_sc21 ;
  Long64_t  check_total_seetram ;
  Long64_t  check_total_mrtof_start ;
  Long64_t  check_total_mrtof_stop ;
     
protected: 
  Int_t counter;
private:
  void Create_MON_Hist();
  void Create_MW_Hist();
  void Create_TPC_Hist();
  void Create_SI_Hist();
  void Create_CT_Hist();
  void Create_ElCurrent_Hist();
      
  void Process_MON_Analysis(const TXRSSortEvent& src, TXRSCalibrEvent& tgt);
  void Process_MW_Analysis(const TXRSSortEvent& src, TXRSCalibrEvent& tgt);
  void Process_TPC_Analysis(const TXRSSortEvent& src, TXRSCalibrEvent& tgt);
  void Process_SI_Analysis(const TXRSSortEvent& src, TXRSCalibrEvent& tgt);
  void Process_CT_Analysis(const TXRSSortEvent& src, TXRSCalibrEvent& tgt);
  void Process_ElCurrent_Analysis(const TXRSSortEvent& src, TXRSCalibrEvent& tgt);

  TXRSParameter* frs ;
  TMWParameter* mw ;
  TTPCParameter* tpc;
  TSIParameter* si;
  
  // from MON analysis
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
      
  // MON data declarations
      
  TH1I          *hTimeStamp;
  TH1I          *hTimeStampZoom;
  TH1I          *hTimeSpill;
  TH1I          *hMON_All;
  TH1I          *hMON_All2;
  TH1I          *hMON_scaler[64];
  TH1I          *hMON_diff[64];
  TH1I          *hMON_extr[64];
  TH1I          *hMON_seecalA[8];
  TH1I          *hMON_seecalB[8];
  TH1I          *hMON_Pattern;
  TH1I          *hMON_PatternH;

  TH1I          *hSCALER_TIME[64];
  TH1I          *hSCALER_SPILL[64];
  TH1I          *hSCALER_TIME_SHORT[64];
  TH1I          *hSCALER_SPILL_SHORT[64];

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

  

  /*
    TH1I          *hMON_TOE;
    TH1I          *hMON_DtDiff;
    TH1I          *hMON_SC01;
    TH1I          *hMON_SC21;
    TH1I          *hMON_SC41;
    TH1I          *hMON_Trigger;
    TH1I          *hMON_LAM;
    TH1I          *hMON_SE01;
    TH1I          *hMON_ICDE;
    TH1I          *hMON_IC1;
    TH1I          *hMON_IC1d;
    TH1I          *hMON_IC2;
    TH1I          *hMON_IC2d;
    TH1I          *hMON_IC3;
    TH1I          *hMON_IC3d;
  */
  // MW data declarations
      
  //**** Raw spectra
  TH1I          *hMW_AN[13];
  TH1I          *hMW_XL[13];  
  TH1I          *hMW_XR[13];
  TH1I          *hMW_YU[13];
  TH1I          *hMW_YD[13];

  //**** Sum spectra
  TH1I          *hMW_XSUM[13];
  TH1I          *hMW_YSUM[13];
      
  //**** Position spectra
  TH1I          *hMW_X[13];   
  TH1I          *hMW_Y[13];
      
  TH2I          *hMW_XY[13];
      
  TH1I          *hMW_SC21x;
  TH1I          *hMW_SC22x;
  TH1I          *hitag_stopper_x;
  TH1I          *hMW_MUSICx;
 
  TH2I          *hMW21x_Time;
      
      
  //**** Focal spectra S2
  TH1I          *hMW_xAngS2;  
  TH1I          *hMW_yAngS2;
  TH1I          *hMW_xFocS2;
  TH1I          *hMW_yFocS2;
  TH1I          *hMW_zxS2;
  TH1I          *hMW_zyS2;
  TH2I          *hMW_FocS2;
 
      
  TGo4WinCond   *cMW_XSUM[13];
  TGo4WinCond   *cMW_YSUM[13];
      

  // TPC part
  // rawdata
  TH1I *hTPC_L0[7];
  TH1I *hTPC_R0[7];
  TH1I *hTPC_LT0[7];
  TH1I *hTPC_RT0[7];
  TH1I *hTPC_L1[7];
  TH1I *hTPC_R1[7];
  TH1I *hTPC_LT1[7];
  TH1I *hTPC_RT1[7];
  // [index][anode_no]
  TH1I *hTPC_DT[7][4];
  TH1I *hTPC_A[7][4];

  /// positions and control sum
  TH1I *hTPC_XRAW0[7];
  TH1I *hTPC_XRAW1[7];
  TH1I *hTPC_YRAW[7];
  TH1I *hTPC_X[7];
  TH1I *hTPC_Y[7];
  TH2I *hcTPC_XY[7];
  TH2I *hTPC_LTRT[7];
  TH1I *hTPC_DELTAX[7];

  // CSUM[index][anode_no]
  TH1I *hTPC_CSUM[7][4];


  TH1I *hTPC_X_S2;            //Positions @ S2 focus
  TH1I *hTPC_Y_S2;
  TH1I *hTPC_AX_S2;           //Angles @ S2 focus
  TH1I *hTPC_AY_S2;

  TH1I *hTPC_X_S2_TPC_21_22;  //Positions @ S2 focus using TPCs in Vaccum (TPC21 & TPC22)
  TH1I *hTPC_Y_S2_TPC_21_22;
  TH1I *hTPC_AX_S2_TPC_21_22; //Angles @ S2 focus using TPCs in Vaccum (TPC21 & TPC 22)
  TH1I *hTPC_AY_S2_TPC_21_22;

  TH1I *hTPC_X_S2_TPC_23_24;     //Positions @ S2 focus using TPCs in air (TPC23 & TPC24)
  TH1I *hTPC_Y_S2_TPC_23_24;
  TH1I *hTPC_AX_S2_TPC_23_24;    //Angles @ S2 focus using TPCs in air (TPC23 & TPC24)
  TH1I *hTPC_AY_S2_TPC_23_24;

  TH1I *hTPC_X_S2_TPC_22_24;      //Positions @ S2 focus using TPC22 & TPC24
  TH1I *hTPC_Y_S2_TPC_22_24;
  TH1I *hTPC_AX_S2_TPC_22_24;     //Angles @ S2 focus using TPC22 & TPC24
  TH1I *hTPC_AY_S2_TPC_22_24;
  
  TH1I *hTPC_X_S4;                //Positions @ S4 focus
  TH1I *hTPC_Y_S4;
  TH1I *hTPC_AX_S4;               //Angles @ S4 focus
  TH1I *hTPC_AY_S4;
  
  TH1I *hTPC_X_S4_target2;        //Positions @ OTPC
  TH1I *hTPC_Y_S4_target2;
  TH1I *hTPC_X_S2_target1;        //Positions @ first Si tracker in S2
  TH1I *hTPC_Y_S2_target1;
  //      TH1I *hTPC_AX_S4_target2;       //Angles @ Second Target @ S4
  //      TH1I *hTPC_AY_S4_target2;

  TH2I *hTPC_XY_S4_target2;
  TH2I *hTPC_XY_S2_target1;

  TH2I *hTPC_S4X_time; //YT20Jun
  TH2I *hTPC_S4A_time; //YT20Jun
  
  TH2I *hTPC_S2X_time; //YT20Jun
  TH2I *hTPC_S2A_time; //YT20Jun

  TH2I *hTPC_S2X_TPC21_22_time; //SB21Jun
  TH2I *hTPC_S2A_TPC21_22_time; //SB21Jun

  TH2I *hTPC_S2X_TPC23_24_time; //SB21Jun
  TH2I *hTPC_S2A_TPC23_24_time; //SB21Jun

  TH2I *hTPC_S2X_TPC22_24_time; //SB21Jun
  TH2I *hTPC_S2A_TPC22_24_time; //SB21Jun
  
  //      TH2I *hTPC_XY_S4_target1d;
  //      TH2I *hTPC_XY_S4_target2d;
  //      TH1I *hTPC_X_S4_target1d;
  //      TH1I *hTPC_Y_S4_target1d;
  //      TH1I *hTPC_X_S4_target2d;
  //      TH1I *hTPC_Y_S4_target2d;

  TH2I *hTPC_XAX_S4;
  TH2I *hTPC_XS4_AX_S2;

  TH2I *hTPC_XS4_AX_TPC21_22_S2;
  TH2I *hTPC_XS4_AX_TPC23_24_S2;
  TH2I *hTPC_XS4_AX_TPC22_24_S2;

  TH1I *hTPC_SC21x;
  TH1I *hTPC_SC21y;
  TH1I *hTPC_SC41x;
  TH1I *hTPC_SC41y;
 

  ////condtition for control sum
  TGo4WinCond *cTPC_CSUM[7][4];
  TGo4PolyCond *cTPC_XY[7];

  //**** keep values from previous event
  Float_t       focx_s2m;
  Float_t       focy_s2m;
  Float_t       angle_x_s2m;
  Float_t       angle_y_s2m;


  //Si detector
  TH1I *hsi_e1;
  TH1I *hsi_e2;
  TH1I *hsi_e3;
  TH1I *hsi_e4;

  TH1I *hsi_e1Trig;
  TH1I *hsi_e2Trig;
  TH1I *hsi_e3Trig;
  TH1I *hsi_e4Trig;

  TH1I *hdssd_e[32];
  TH1I *hdssd_xmaxenergy; 
  TH1I *hdssd_xmaxindex;
  TH1I *hdssd_ymaxenergy; 
  TH1I *hdssd_ymaxindex;

  TH1I *hdssd_e_dec[32];
  TH1I *hdssd_xmaxenergy_dec; 
  TH1I *hdssd_xmaxindex_dec;
  TH1I *hdssd_ymaxenergy_dec; 
  TH1I *hdssd_ymaxindex_dec;

  TH2I *hdssd_xymaxindex;
  TH2I *hdssd_xymaxindex_dec;

  //Channeltron detector
  TH1I 	*hct_all;
  TH1I 	*hct_trigger_DU;
  TH1I 	*hct_trigger_SY;
  TH1I 	*hct_DU;
  TH1I 	*hct_SY;

  //Electron Current
  TH1I 	*helcurrent;

  ClassDef(TXRSCalibrProc,1)
};

#endif //TXRSCALIBRPROCESSOR_H
