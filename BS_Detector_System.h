
#ifndef BS_DETECTOR_SYSTEM_H
#define BS_DETECTOR_SYSTEM_H

#include "BS_Detector_System.h"

#include "Riostream.h"

#include "TH1.h"
#include "TMap.h"
#include "TObject.h"

#include <cstdlib>
#include <bitset>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>

// Go4 Includes //
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"
#include "TGo4StepFactory.h"
#include "TGo4Analysis.h"
#include "TGo4WinCond.h"
#include "TGo4AnalysisObjectManager.h"
#include "TGo4EventProcessor.h"

#include "TXRSParameter.h"

#include "Detector_System.cxx"


class BS_Detector_System : public Detector_System{
typedef unsigned long long ULong64_t;

private:
    
    TGo4MbsSubEvent* psubevt;
    
    Int_t *pdata;
    Int_t len = 0;	  
    Int_t vme_chn;
    Int_t lenMax;
    
    void Setup_Parameters();
    
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
    
    
    /**************************************************************/
    /***UNPACK STUFF***/
    
    
    void FRS_Unpack(TGo4MbsSubEvent* psubevt);
    
    void FRS_Timestamp_Module(int);
    
    void FRS_Scaler_Module(int);
    
    void FRS_775_TDC_785_ADC_Modules(int);
    
    void FRS_Rest_Of_Unpacking(int);
    
    UInt_t  vme[6][21][32];         // FRS crate                                
    //UInt_t*** vme0;         // FRS crate                                
    //UInt_t*** vme1;         // TPC crate 
    //UInt_t*** vme3;         // Mtof crate
    //UInt_t*** vme4 ; // SOFIA crate temp need multiplicity for both MDPP and vftx
    
    //UInt_t*** vme5;          //Main Crate or User Crate
    
    
    
    Int_t* vme2scaler;         // User Crate Messhure
    Int_t* vme3scaler;         // User Crate Messhure
    
    Int_t** vme2s;           // User TDC (V1290) 
    Int_t** vme2s_trailing;  // User TDC (V1290) 
    Int_t** nhit5;            // multiplicity (V1290)
    
    Int_t** vme3s_MT;           // Mtof TDC (V1290) 
    Int_t** vme3s_MT_trailing;  // Mtof TDC (V1290) 
    Int_t** vme3_MT_nhit5;       // multiplicity (V1290)
    
    
    bool* firstTS;
    Long64_t* previousTimeStamp;
    Long64_t currentTimeStamp;
    
    Int_t EventFlag;
    
    Int_t         qlength;         /* From event header     */
    Int_t         qtype;           /*                       */
    Int_t         qsubtype;        /*                       */
    Int_t         qdummy;          /*                       */
    Int_t         qtrigger;        /*                       */
    Int_t         qevent_nr;       /*                       */
    
    
    /*******************************************************************/
    /***SORT STUFF***/
    
    void FRS_Sort();
    
    Long64_t StartOfSpilTime;
    Long64_t StartOfSpilTime2; //does not reset at end of extraction
    Long64_t PreviousTS;
    
    Int_t counter; 
    
    // time stamp data  
    Int_t         ts_id;                                
    Int_t*         ts_word; //for the titris time stammping
    Int_t*         tsys_word; //for the system time
    Long64_t      timestamp;  // absolute time stamp value
    Double_t      timespill;  // relative time from start of the spill 
    Double_t      timespill2;  // relative time from start of the spill does not reset at end extraction
    Double_t      systemtime_ms;
    Double_t      systemtime_s;
    
    Int_t         pattern;
    Int_t         trigger;
    
    // scaler readings     
    UInt_t* sc_long; //changed from 32 to 64 (10.07.2018)
    UInt_t* sc_long2;
    
    
    // SEETRAM calib IC energy
    //      Int_t         ic_de;           /*                          */
    
    
    // part of MW parameter
    Int_t*         mw_an;       /*  anode time              */
    Int_t*         mw_xl;       /*  Rohdaten                */
    Int_t*         mw_xr;       /*                          */
    Int_t*         mw_yu;       /*                          */ 
    Int_t*         mw_yd;       /*                          */
    
    // TPC part //(HAPOL-25/03/06) 6 TPCs each one with 2 delay lines each!!
    //7 TPCs (4 at S2, 2 at S4 and one at S3) 03.07.2018 SB
    Int_t** tpc_l;
    Int_t** tpc_r;
    Int_t** tpc_lt;
    Int_t** tpc_rt;
    
    // [index][anode_no]
    Int_t** tpc_dt;
    Int_t** tpc_a;
    
    
    
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
	    
    Int_t* tdc_sc41l;
    Int_t* tdc_sc41r;
    Int_t* tdc_sc21l;
    Int_t* tdc_sc21r;
    Int_t* tdc_sc42l;
    Int_t* tdc_sc42r;
    Int_t* tdc_sc43l;
    Int_t* tdc_sc43r;
    Int_t* tdc_sc81l;
    Int_t* tdc_sc81r;
    
    // MUSIC1 part
    Int_t*         music_e1;     /* Raw energy signals       */
    Int_t*         music_t1;     /* Raw drift time           */
    Int_t*         music_pres;   /* Music Druck              */
    Int_t*         music_temp;   /* Music Temperatur         */
    
    // MUSIC2 part
    Int_t*         music_e2;     /* Raw energy signals       */
    Int_t*         music_t2;     /* Raw drift time           */
    
    // MUSIC3 part (OLD MUSIC)
    Int_t*         music_e3;     /* Raw energy signals       */
    Int_t*        music_t3;     /* Raw drift times          */
    
    // Single Anode
    Int_t SingleAnode_adc;
    
    //Si detectors
    Int_t si_adc1;
    Int_t si_adc2;
    Int_t si_adc3;
    Int_t si_adc4;
    Int_t si_adc5;
    Int_t* dssd_adc;
    
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
    
    /*******************************************************************/
    /***CALIBRATION STUFF***/
    
    void FRS_Calib();
    
    Float_t*** lim_csum;
    
    Float_t** lim_xsum;
    Float_t** lim_ysum;
    
    /*Float_t lim_csum[4][7][2] = {{{300,1800},{350,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}},
				 {{300,1800},{300,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}},
				 {{300,1800},{300,1840},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}},
				 {{300,1800},{300,1810},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}}}*///;
    
    
    //lim_csum[0][7][2] = {{300,1800},{350,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
    //lim_csum[1][7][2] = {{300,1800},{300,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
    //lim_csum[2][7][2] = {{300,1800},{300,1840},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
    //lim_csum[3][7][2] = {{300,1800},{300,1810},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
				

    //Float_t lim_csum2[7][2]={{300,1800},{300,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
    //Float_t lim_csum3[7][2]={{300,1800},{300,1840},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
    //Float_t lim_csum4[7][2]={{300,1880},{300,1810},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}};
    
    /*Float_t lim_xsum[13][2] = {	{1,8000},  // MW11
				{1,8000},  // MW21
				{1,8000},  // MW22
   				{1,8000},  // MW31
   				{1,8000},  // MW41
   				{1,8000},  // MW42
   				{1,8000},  // MW51
  				{1,8000},  // MW61
  				{1,8000},  // MW71
  				{1,8000},  // MW81
  				{1,8000},  // MW82
  				{1,8000},  // MWB1
  				{1,8000}};  // MWB2

    Float_t lim_ysum[13][2] = { {2,8000},  // MW11
				{2,8000},  // MW21
  				{2,8000},  // MW22
  				{2,8000},  // MW31
  				{2,8000},  // MW41
 				{2,8000},  // MW42
 				{2,8000},  // MW51
 				{2,8000},  // MW61
 				{2,8000},  // MW71
 				{2,8000},  // MW81
 				{2,8000},  // MW82
 				{2,8000},  // MWB1
 				{2,8000}};  // MWB2*/
    
    
    // MON data declarations
    
    int* check_first_event;
    int scaler_channel_10khz;//YT 20Jun
    Long64_t*  scaler_time_count; 
    Long64_t*  scaler_spill_count; //UInt_t
    Long64_t*  scaler_time_check_last;//UInt_t
    Long64_t*  scaler_spill_check_last;//UInt_t 
    Long64_t*  check_increase_time;//UInt_t 
    Long64_t*  check_increase_spill;//UInt_t
    Long64_t*  scaler_increase_event;//UInt_t
    Long64_t*  scaler_last_event;
    
    
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
    
    
    
    Bool_t         fbFirstEvent;
    //unsigned int  scaler_save[64];
    Long64_t*      scaler_save;
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
    Long64_t* mon_inc;
    
    // MON part
    Int_t         seconds;
    Int_t         tenthsecs;
    Int_t         hundrethsecs;	//mik
    Int_t         extraction_cycle;
    Int_t*        coin;
    Float_t       seetram;
    
    // MW part
    Float_t*      mw_xsum;     /*                          */
    Float_t*      mw_ysum;     /*                          */
    
    Float_t*      mw_x;        /*                          */
    Float_t*      mw_y;        /*                          */
    Float_t*      mw_wire;     /* special for Helmut       */
    
    Float_t       z_x_s2;          
    Float_t       z_y_s2;          
    Float_t       z_x_s4;          
    Float_t       z_y_s4;          
    Float_t       z_x_s8;          
    Float_t       z_y_s8;          
    
    Bool_t*        b_mw_xsum;   /*  wc on sum               */
    Bool_t*        b_mw_ysum;   /*                          */
    
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
    Int_t** tpc_csum;
    Float_t* tpc_x;
    Float_t* tpc_y;
    Bool_t** b_tpc_csum;
    Bool_t* b_tpc_xy;
    
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
    
    Float_t* dssd_e; //[3][2][16]
    Float_t dssd_maxenergy; //[3] 
    Int_t dssd_maxindex;  //[3]
    
    Float_t dssd_xmaxenergy;
    Int_t dssd_xmaxindex;
    Float_t dssd_ymaxenergy;
    Int_t dssd_ymaxindex;
    
    
    /*******************************************************************/
    /***CALIBRATION STUFF***/
    
    void FRS_Anal();
    
    char name[80];
    
    TGo4WinCond  *cMusic1_E[8];
    TGo4WinCond  *cMusic1_T[8];
    TGo4WinCond  *cMusic2_E[8];
    TGo4WinCond  *cMusic2_T[8];
    TGo4WinCond  *cMusic3_T[4];
    TGo4WinCond  *cMusic3_E[4];
    TGo4WinCond  *cMusic3_dec;
    
    /*TGo4WinCond   *cID_x2;
    TGo4WinCond   *cID_x4;
    TGo4PolyCond  *cID_dEToF;
    TGo4PolyCond  *cID_x2AoQ[6];
    TGo4PolyCond  *cID_Z_AoQ[5];
    TGo4PolyCond  *cID_x4AoQ_Z[5]; 
    TGo4WinCond   *cID_Z_Z[5]; 
     
    TGo4PolyCond  *cMUSIC_13;
    
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
  
    TGo4PolyCond  *cSCI_detof;*/
    
  
  
    Int_t getbits(Int_t, int, int, int);

    Int_t get2bits(Int_t, int, int, int);
 
    Float_t rand0_5();


public:
	BS_Detector_System();
	~BS_Detector_System();
	
	//void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};
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


};



#endif
