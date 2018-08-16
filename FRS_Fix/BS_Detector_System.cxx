#include "BS_Detector_System.h"

#include "Riostream.h"

#include "TH1.h"
#include "TMap.h"
#include "TObject.h"
#include "TString.h"
#include "TObjString.h"
 
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
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4EventProcessor.h"

#include "TXRSParameter.h"

#include "Detector_System.cxx"






using namespace std;

//---------------------------------------------------------------

BS_Detector_System::BS_Detector_System(){
    
    /*for(int i; i < 6; ++i){
	vme 	= new UInt_t**[i];         // FRS crate
	vme[i]  = new UInt_t*[21];                             
    }
    
    

    //set amount of Germanium Detectors
    /*vme0 = new UInt_t*[21];         // FRS crate                                
    vme1 = new UInt_t*[21];         // TPC crate 
    vme3 = new UInt_t*[21];         // Mtof crate
    vme5 = new UInt_t*[21];          //Main Crate or User Crate
    vme4 = new UInt_t*[21] ; // SOFIA crate temp need multiplicity for both MDPP and vftx
    */
    
    
    /*for(int i; i < 21; ++i){
		                        
	vme[0][i] = new UInt_t[32];         // FRS crate                                
	vme[1][i] = new UInt_t[32];         // TPC crate 
	vme[2][i] = new UInt_t[32];         // Mtof crate
	vme[3][i] = new UInt_t[32];          //Main Crate or User Crate
	vme[4][i] = new UInt_t[32];          //Main Crate or User Crate
	vme[5][i] = new UInt_t[32];          //Main Crate or User Crate
	
    }*/
    
    vme2s = new Int_t*[32];           // User TDC (V1290) 
    vme2s_trailing = new Int_t*[32];  // User TDC (V1290) 

    nhit5 = new Int_t*[32];            // multiplicity (V1290)
    
    //nhit5[0] = new Int_t[32];            // multiplicity (V1290)
    //nhit5[1] = new Int_t[32];            // multiplicity (V1290)

    vme3_MT_nhit5 = new Int_t*[32];       // multiplicity (V1290)
    
    //vme3_MT_nhit5[0] = new Int_t[32];            // multiplicity (V1290)
    //vme3_MT_nhit5[1] = new Int_t[32];            // multiplicity (V1290)
    
    vme3s_MT = new Int_t*[32];           // Mtof TDC (V1290) 
    vme3s_MT_trailing = new Int_t*[32];  // Mtof TDC (V1290) 
    
    for(int i; i < 32; ++i){
	
	vme2s[i] 		= new Int_t[10];           // User TDC (V1290) 
	vme2s_trailing[i] 	= new Int_t[10];  // User TDC (V1290) 

	vme3s_MT[i] 		= new Int_t[10];           // Mtof TDC (V1290) 
	vme3s_MT_trailing[i] 	= new Int_t[10];  // Mtof TDC (V1290) 

	nhit5[i]		= new Int_t[2];       // multiplicity (V1290)
	vme3_MT_nhit5[i] 	= new Int_t[2];       // multiplicity (V1290)

	
	
    }
    
    
    vme2scaler = new Int_t[32];         // User Crate Messhure
    vme3scaler = new Int_t[32];         // User Crate Messhure

    firstTS = new bool[3];
    
    previousTimeStamp = new Long64_t[3];
    
    currentTimeStamp = 0;
    
    EventFlag = 0;
    
    qlength 	= 0;         /* From event header     */
    qtype 	= 0;           /*                       */
    qsubtype 	= 0;        /*                       */
    qdummy 	= 0;          /*                       */
    qtrigger 	= 0;        /*                       */
    qevent_nr 	= 0;       /*                       */

    /*******************************************************************/
    /***SORT STUFF***/
  
    StartOfSpilTime 	= 0;
    StartOfSpilTime2 	= 0; //does not reset at end of extraction
    PreviousTS 		= 0;
  
    counter = 0; 

    // time stamp data  
    ts_id = 0;                                
    ts_word = new Int_t[4]; //for the titris time stammping
    tsys_word = new Int_t[3]; //for the system time
    timestamp = 0;  // absolute time stamp value
    timespill = 0;  // relative time from start of the spill 
    timespill2 = 0;  // relative time from start of the spill does not reset at end extraction
    systemtime_ms = 0;
    systemtime_s = 0;

    pattern = 0;
    trigger = 0;

    // scaler readings     
    sc_long = new UInt_t[64]; //changed from 32 to 64 (10.07.2018)
    sc_long2 = new UInt_t[32];


    // SEETRAM calib IC energy
    //      Int_t         ic_de;           /*                          */
      
     
    // part of MW parameter
    mw_an = new Int_t[13];       /*  anode time              */
    mw_xl = new Int_t[13];       /*  Rohdaten                */
    mw_xr = new Int_t[13];       /*                          */
    mw_yu = new Int_t[13];       /*                          */ 
    mw_yd = new Int_t[13];       /*                          */

    // TPC part //(HAPOL-25/03/06) 6 TPCs each one with 2 delay lines each!!	
    //7 TPCs (4 at S2, 2 at S4 and one at S3) 03.07.2018 SB
    
    tpc_l = new Int_t*[7];
    tpc_r = new Int_t*[7];
    tpc_lt = new Int_t*[7];
    tpc_rt = new Int_t*[7];

    // [index][anode_no]
    tpc_dt = new Int_t*[7];
    tpc_a = new Int_t*[7];
    
    
    
    for(int i=0; i < 7; ++i){
	
	tpc_l[i] = new Int_t[2];
	tpc_r[i] = new Int_t[2];
	tpc_lt[i] = new Int_t[2];
	tpc_rt[i] = new Int_t[2];
	
	tpc_dt[i] = new Int_t[4];
	tpc_a[i] = new Int_t[4];
	
    }

      
    // SCI part
    de_21l  = 0;         /* dE SCI21 left            */  
    de_21r  = 0;         /* de SCI21 right           */ 
    de_41l  = 0;         /* dE SCI41 left            */
    de_41r  = 0;         /* dE SCI41 right           */
    de_41u  = 0;         /* dE SCI41 up              */
    de_41d  = 0;         /* dE SCI41 down            */
    de_42l  = 0;         /* de SCI42 left            */
    de_42r  = 0;         /* de SCI42 right           */
    de_43l  = 0;         /* de SCI43 left            */
    de_43r  = 0;         /* de SCI43 right           */
    de_81l  = 0;         /* de SCI81 left            */
    de_81r  = 0;         /* de SCI81 right           */  
    de_21ld = 0;         /* dE SCI21 left delayed    */  
    de_21rd = 0;         /* de SCI21 right delayed   */ 
    de_v1l  = 0;         /* dE veto1 left            */  
    de_v1r  = 0;         /* de veto1 right           */ 
    de_v2l  = 0;         /* dE veto2 left            */
    de_v2r  = 0;         /* dE veto2 right           */
    de_v3   = 0;         /* dE veto3                 */


    dt_21l_21r  = 0;     /*                          */ 
    dt_41l_41r  = 0;     /*                          */ 
    dt_21l_41l  = 0;     /*                          */
    dt_21r_41r  = 0;     /*                          */
    dt_42l_42r  = 0;     /*                          */
    dt_43l_43r  = 0;     /*                          */
    dt_42l_21l  = 0;     /*                          */
    dt_42r_21r  = 0;     /*                          */
    dt_41u_41d  = 0;     /*                          */
    dt_81l_81r  = 0;     /*                          */
    dt_21l_81l  = 0;     /*                          */
    dt_21r_81r  = 0;     /*                          */

    // User multihit TDC
		
    tdc_sc41l = new Int_t[10];
    tdc_sc41r = new Int_t[10];
    tdc_sc21l = new Int_t[10];
    tdc_sc21r = new Int_t[10];
    tdc_sc42l = new Int_t[10];
    tdc_sc42r = new Int_t[10];
    tdc_sc43l = new Int_t[10];
    tdc_sc43r = new Int_t[10];
    tdc_sc81l = new Int_t[10];
    tdc_sc81r = new Int_t[10];

    // MUSIC1 part
    music_e1 = new Int_t[8];     /* Raw energy signals       */
    music_t1 = new Int_t[8];     /* Raw drift time           */
    music_pres = new Int_t[3];   /* Music Druck              */
    music_temp = new Int_t[3];   /* Music Temperatur         */
    
    // MUSIC2 part
    music_e2 = new Int_t[8];     /* Raw energy signals       */
    music_t2 = new Int_t[8];     /* Raw drift time           */
    
    // MUSIC3 part (OLD MUSIC)
    music_e3 = new Int_t[4];     /* Raw energy signals       */
    music_t3 = new Int_t[4];     /* Raw drift times          */

    // Single Anode
    SingleAnode_adc = 0;
    
    //Si detectors
    si_adc1 = 0;
    si_adc2 = 0;
    si_adc3 = 0;
    si_adc4 = 0;
    si_adc5 = 0;
    dssd_adc = new Int_t[32];

    //Channeltron detector
    ct_signal = 0;
    ct_time = 0;
    ct_trigger_DU  = false;
    ct_trigger_SY = false;
    
    //Electron current measurement
    ec_signal = 0;
    
    // MR-TOF
    mrtof_start = 0;
    mrtof_stop = 0;
    mrtof_stopDelay = 0;
    
    /******************************************************************/
    /**CALIBRATION Parameters**/
    
    // MON data declarations
    
    check_first_event 		 = new int[2];
    scaler_channel_10khz = 0;//YT 20Jun
    scaler_time_count 		 = new Long64_t[2]; 
    scaler_spill_count 		 = new Long64_t[2]; //UInt_t
    scaler_time_check_last 	 = new Long64_t[2];//UInt_t
    scaler_spill_check_last 	 = new Long64_t[2];//UInt_t 
    check_increase_time 	 = new Long64_t[64];//UInt_t 
    check_increase_spill 	 = new Long64_t[64];//UInt_t
    scaler_increase_event 	 = new Long64_t[64];//UInt_t
    scaler_last_event 		 = new Long64_t[64];
    
    
    //TGo4WinCond   *cMW_XSUM[13];
    //TGo4WinCond   *cMW_YSUM[13];  
    
    ////condtition for control sum
    //TGo4WinCond *cTPC_CSUM[7][4];
    //TGo4PolyCond *cTPC_XY[7];
    
    //**** keep values from previous event
    focx_s2m 		= 0;
    focy_s2m 		= 0;
    angle_x_s2m 	= 0;
    angle_y_s2m 	= 0;
    
    
    
    fbFirstEvent = false;
    //unsigned int  scaler_save[64];
    scaler_save  = new Long64_t[64];
    
    firstsec 		= 0;
    firsttenthsec 	= 0;
    firsthundrethsec 	= 0;	//mik	
    firstcycle		= 0;
    firstseetram 	= 0;
    
    scalercycle_Sec 		= 0;
    scalercycle_TenthSec 	= 0;
    scalercycle_HundrethSec 	= 0;
    scalercycle_Cycle 		= 0;
    scalercycle_Seetram 	= 0;
    
    dtime 	= 0;
    dt_last 	= 0;
    dt_diff 	= 0;
    
    
    check_total_sc41 		= 0;
    check_total_sc21 		= 0;
    check_total_seetram 	= 0;
    check_total_mrtof_start 	= 0;
    check_total_mrtof_stop 	= 0;
    
    
    freeTrig = 0;
    acptTrig = 0;
    mon_inc  = new Long64_t[64];
    
    // MON part
    seconds 		= 0;
    tenthsecs 		= 0;
    hundrethsecs 	= 0;	//mik
    extraction_cycle 	= 0;
    coin  = new Int_t[16];
    seetram 		= 0;
    
    // MW part
    mw_xsum = new Float_t[13];     /*                          */
    mw_ysum = new Float_t[13];     /*                          */
    
    mw_x    = new Float_t[13];     /*                          */
    mw_y    = new Float_t[13];     /*                          */
    mw_wire = new Float_t[13];     /* special for Helmut       */
    
    z_x_s2 = 0;          
    z_y_s2 = 0;          
    z_x_s4 = 0;          
    z_y_s4 = 0;          
    z_x_s8 = 0;          
    z_y_s8 = 0;          
    
    b_mw_xsum = new Bool_t[13];   /*  wc on sum               */
    b_mw_ysum = new Bool_t[13];   /*                          */
    
    focx_s2 = 0;         /*  FRS foci                */  
    focy_s2 = 0;         /*                          */  
    focx_s4 = 0;         /*                          */  
    focy_s4 = 0;         /*                          */  
    focx_s8 = 0;         /*  FRS foci                */  
    focy_s8 = 0;         /*                          */
    angle_x_s2 = 0;      /*                          */
    angle_y_s2 = 0;      /*                          */
    angle_x_s4 = 0;      /*                          */
    angle_y_s4 = 0;      /*                          */
    angle_x_s8 = 0;      /*                          */
    angle_y_s8 = 0;      /*                          */
    
    // TPC part
    tpc_x    = new Float_t[7];
    tpc_y    = new Float_t[7];
    b_tpc_xy = new Bool_t[7];

    tpc_csum = new Int_t*[7];
    b_tpc_csum = new Bool_t*[7];
    
    for(int i=0; i < 7; ++i){
	
	tpc_csum[i] = new Int_t[4];
	b_tpc_csum[i] = new Bool_t[4];
	
    }
    
    //TPCs 21 & 22 @ S2 focus
    tpc_x_s2_foc_21_22 		= 0;
    tpc_y_s2_foc_21_22 		= 0;
    tpc_angle_x_s2_foc_21_22 	= 0;
    tpc_angle_y_s2_foc_21_22 	= 0;
    
    //TPCs 23 & 24 @ S2 focus
    tpc_x_s2_foc_23_24 		= 0;
    tpc_y_s2_foc_23_24 		= 0;
    tpc_angle_x_s2_foc_23_24 	= 0;
    tpc_angle_y_s2_foc_23_24 	= 0;
    
    //TPCs 22 & 24 @ S2 focus
    tpc_x_s2_foc_22_24 		= 0;
    tpc_y_s2_foc_22_24 		= 0;
    tpc_angle_x_s2_foc_22_24 	= 0;
    tpc_angle_y_s2_foc_22_24 	= 0;
    
    //TPCs 3 & 4 @ S2 focus (old tracking)
    tpc_x_s2_foc 		= 0;
    tpc_y_s2_foc 		= 0;
    tpc_angle_x_s2_foc 		= 0;
    tpc_angle_y_s2_foc 		= 0;
    
    //TPCs 41 & 42 @ s4 focus
    tpc_x_s4 			= 0;
    tpc_y_s4 			= 0;
    tpc_angle_x_s4 		= 0;
    tpc_angle_y_s4 		= 0;
    
    //TPCs 5 & 6 @ S4 OTPC (exp s388)
    tpc_x_s4_target2 		= 0;
    tpc_y_s4_target2 		= 0;
    
    
    //TPCs 3 & 4 @ S2 first Si tracking detector (exp s388)
    tpc_x_s2_target1 		= 0;
    tpc_y_s2_target1 		= 0;
    //      Float_t tpc_angle_x_s4_target2;
    //      Float_t tpc_angle_y_s4_target2;
    
    
    tpc_sc21_x = 0;      /* SC21 x                   */
    tpc_sc41_x = 0;      /* SC41 x                   */
    tpc_sc21_y = 0;      /* SC21 y                   */
    tpc_sc41_y = 0;      /* SC41 y                   */
    
    
    sc21_x = 0;          /* SC21                     */
    sc22_x = 0;          /* SC22 (LAND finger)       */
    sc41_x = 0;          /* SC41                     */
    itag_stopper_x = 0;  /* ITAG Stopper             */
    sc42_x = 0;          /* SC42                     */
    sc43_x = 0;          /* SC43                     */
    sc21_y = 0;          /* tracked SC21 Y pos       */
    sc41_y = 0;          /* SC41 Y                   */
    sc42_y = 0;          /* SC42 Y                   */
    sc43_y = 0;          /* SC43 Y                   */
    sc81_x = 0;          /* SC81          positions  */
    
    music1_x1 = 0;       /* parameters to calibrate  */
    music1_x2 = 0;       /* parameters to calibrate  */
    music1_x3 = 0;       /* parameters to calibrate  */
    music1_x4 = 0;       /* parameters to calibrate  */
    music1_y1 = 0;       /* parameters to calibrate  */
    music1_y2 = 0;       /* parameters to calibrate  */
    music1_y3 = 0;       /* parameters to calibrate  */
    music1_y4 = 0;       /* parameters to calibrate  */
    music2_x  = 0;       /* MUSIC1,2 positions       */
    
    
    //Si detectors
    si_e1 = 0;
    si_e2 = 0;
    si_e3 = 0;
    si_e4 = 0;
    si_e5 = 0;
    
    dssd_e = new Float_t[32]; //[3][2][16]
    dssd_maxenergy = 0; //[3] 
    dssd_maxindex  = 0;  //[3]
    
    dssd_xmaxenergy = 0;
    dssd_xmaxindex  = 0;
    dssd_ymaxenergy = 0;
    dssd_ymaxindex  = 0;
    
    /******************************************************************/
    /*** ANALYSIS Parameters ***/
    
    /*** MUSIC Conditions ***/
    
        // MUSIC part
	    music1_anodes_cnt = 0;
	    music2_anodes_cnt = 0;
	    music3_anodes_cnt = 0;
	    
	    music_b_e1 = new Bool_t[8];
	    music_b_t1 = new Bool_t[8];
	    music_b_e2 = new Bool_t[8];
	    music_b_t2 = new Bool_t[8];
	    music_b_e3 = new Bool_t[4];
	    music_b_t3 = new Bool_t[4];
	    
	    for(int i = 0; i < 8; ++i){
		
		music_b_e1[i] = false;
		music_b_t1[i] = false;
		music_b_e2[i] = false;
		music_b_t2[i] = false;
		if(i < 4) music_b_e3[i] = false;
		if(i < 4) music_b_t3[i] = false;

	    }
	    
	    b_de1 = false;
	    b_de2 = false;
	    b_de3 = false;
	    
	    de = new Float_t[3];
	    de_cor = new Float_t[3];
	    
	    for(int i = 0; i < 3; ++i){

	    	de[i] = 0;
		de_cor[i] = 0;
	    }
	    
	    b_dt3 = false;
	    
	    x1_mean = 0;
	    
	    b_decor = false;
    
    //SCI part
	sci_l = new Float_t[12];  
	sci_r = new Float_t[12];  
	sci_e = new Float_t[12];  
	sci_tx = new Float_t[12];  
	sci_x = new Float_t[12];
	itag_42_e = 0;
	itag_43_e = 0;
	sci_u5 = 0;
	sci_d5 = 0;
	sci_ty5 = 0;  
	sci_y5 = 0;  
	sci_tofll2 = 0;
	sci_tofrr2 = 0;
	sci_tof2 = 0;
	sci_tofll3 = 0;
	sci_tofrr3 = 0;
	sci_tof3 = 0;
	sci_tofll4 = 0;  
	sci_tofrr4 = 0;
	sci_tof4 = 0;
	
	sci_veto_l = new Float_t[3];
	sci_veto_r = new Float_t[3];
	sci_veto_e = new Float_t[3];
	
	sci_b_l = new Bool_t[12];  
	sci_b_r = new Bool_t[12];  
	sci_b_e = new Bool_t[12];  
	sci_b_tx = new Bool_t[12];  
	sci_b_x = new Bool_t[12];  
	sci_b_u5 = false;
	sci_b_d5 = false;
	sci_b_ty5 = false;
	sci_b_y5 = false;
	sci_b_tofll2 = false;
	sci_b_tofrr2 = false;
	sci_b_tofll3 = false;
	sci_b_tofrr3 = false;
	sci_b_tofll4 = false;
	sci_b_tofrr4 = false;  
	sci_b_detof = false;
	sci_b_veto_l = new Bool_t[3];  
	sci_b_veto_r = new Bool_t[4];  
	sci_b_veto_e = new Bool_t[3];
	
	for(int i; i < 12; ++i){
	    
	    sci_l[i] = 0;  
	    sci_r[i] = 0;  
	    sci_e[i] = 0;  
	    sci_tx[i] = 0;  
	    sci_x[i] = 0;
		   
	    sci_b_l[i] = false;  
	    sci_b_r[i] = false;  
	    sci_b_e[i] = false;  
	    sci_b_tx[i] = false;  
	    sci_b_x[i] = false;  
	       
	    if(i < 3){
		
		sci_veto_l[i] = 0;
		sci_veto_r[i] = 0;
		sci_veto_e[i] = 0;
		    
		sci_b_veto_l[i] = false;  
		sci_b_veto_e[i] = false;
		
	    }
	    
	    if(i < 4) sci_b_veto_r[i] = false;  

	    
	}
	
	
    
    // ID part
    
	id_x2 = 0;
	id_y2 = 0;
	id_a2 = 0;
	id_b2 = 0;
	id_x4 = 0;
	id_y4 = 0;
	id_a4 = 0;
	id_b4 = 0;
	id_x8 = 0;
	id_y8 = 0;
	id_a8 = 0;
	id_b8 = 0;
	
	id_b_x2 = false;
	id_b_x4 = false;
	id_b_x8 = false;
	id_b_detof2 = false;
	
	id_brho = new Float_t[2];      /* position-corr. BRho      */
	id_rho = new Float_t[2];       /* Position-corrected Rho   */
	
	id_brho[0] = 0;
	id_brho[1] = 0;
	id_rho[0] = 0;
	id_rho[1] = 0;
	
	id_beta = 0;        /* Beta from TOF            */
	id_beta3 = 0;        /* Beta from TOF            */
	id_gamma = 0;       /* Gamma from TOF           */
	id_AoQ = 0;
	id_AoQ_corr = 0;
	
	id_v_cor = 0;       /* Velocity correction  */
	id_v_cor2 = 0;      /* Velocity correction TUM 2 */
	id_v_cor3 = 0;      /* Velocity correction Old Music */
	id_z = 0;
	id_z2 = 0;
	id_z3 = 0;
	id_energy_geL = 0;
	id_tac_41_geL = 0;
	id_stopper_x = 0;
	id_energy_geL_raw = 0;
	id_tac_41_geL_raw = 0;
	id_trigger = 0;
	id_scal_seconds = 0;
	id_scal_geL = 0;
	id_scal_sc21 = 0;
	id_scal_sc41 = 0;
	id_scal_sc42 = 0;
	id_scal_sc43 = 0;
	id_scal_sc81 = 0;
	
	id_b_AoQ = false;
	id_b_z = false;
	id_b_z2 = false;
	id_b_z3 = false;
	id_b_x2AoQ = false;
	
	id_b_x4AoQ_Z = new Bool_t[5]; 
	id_b_z_AoQ = new Bool_t[5];
	id_b_music_z = new Bool_t[5];
	
	for(int i = 0; i < 5; i++){
	    
	    id_b_x4AoQ_Z[i] = false; 
	    id_b_z_AoQ[i] = false;
	    id_b_music_z[i] = false;
	    
	}
	
	firsttimestamp = 0;
	firstevent = false;
	
	ts = 0;  // relative time from start of the spill 
	ts2 = 0;  // relative time from start of the spill does not reset at end extraction
	
    // MRTOF part :
    
	mrtof_tof = 0;
	
	mrtof_si_e1 = 0;
	mrtof_si_e2 = 0;
	mrtof_si_e3 = 0;
	mrtof_si_e4 = 0;
	mrtof_si_e5 = 0;
	
    
    
    
    
    
    
    Setup_Conditions();
    
    Setup_Parameters();

    
}

//---------------------------------------------------------------

BS_Detector_System::~BS_Detector_System(){

    
    for(int i; i < 32; ++i){
	
	delete[] vme2s[i];           // User TDC (V1290) 
	delete[] vme2s_trailing[i];  // User TDC (V1290) 

	delete[] vme3s_MT[i];           // Mtof TDC (V1290) 
	delete[] vme3s_MT_trailing[i];  // Mtof TDC (V1290) 

	delete[] nhit5[i];       // multiplicity (V1290)
	delete[] vme3_MT_nhit5[i];       // multiplicity (V1290)

    }
    
    
    delete[] vme2scaler;         // User Crate Messhure
    delete[] vme3scaler;         // User Crate Messhure

    delete[] firstTS;
    
    delete[] previousTimeStamp;
    
    /*******************************************************************/
    /***SORT STUFF***/
  
    delete[] ts_word; //for the titris time stammping
    delete[] tsys_word; //for the system time

    // scaler readings     
    delete[] sc_long; //changed from 32 to 64 (10.07.2018)
    delete[] sc_long2;


    // part of MW parameter
    delete[]  mw_an;       /*  anode time              */
    delete[] mw_xl;       /*  Rohdaten                */
    delete[] mw_xr;       /*                          */
    delete[] mw_yu;       /*                          */ 
    delete[] mw_yd;       /*                          */

    // TPC part //(HAPOL-25/03/06) 6 TPCs each one with 2 delay lines each!!	
    //7 TPCs (4 at S2, 2 at S4 and one at S3) 03.07.2018 SB
    
    
    for(int i=0; i < 7; ++i){
	
	delete[] tpc_l[i];
	delete[] tpc_r[i];
	delete[] tpc_lt[i];
	delete[] tpc_rt[i];
	
	delete[] tpc_dt[i];
	delete[] tpc_a[i];
	
    }

      
    // User multihit TDC
		
    delete[] tdc_sc41l;
    delete[] tdc_sc41r;
    delete[] tdc_sc21l;
    delete[] tdc_sc21r;
    delete[] tdc_sc42l;
    delete[] tdc_sc42r;
    delete[] tdc_sc43l;
    delete[] tdc_sc43r;
    delete[] tdc_sc81l;
    delete[] tdc_sc81r;

    // MUSIC1 part
    delete[]  music_e1;     /* Raw energy signals       */
    delete[] music_t1;     /* Raw drift time           */
    delete[] music_pres;   /* Music Druck              */
    delete[]  music_temp;   /* Music Temperatur         */
    
    // MUSIC2 part
    delete[] music_e2;     /* Raw energy signals       */
    delete[] music_t2;     /* Raw drift time           */
    
    // MUSIC3 part (OLD MUSIC)
    delete[] music_e3;     /* Raw energy signals       */
    delete[] music_t3;     /* Raw drift times          */

    delete[] dssd_adc;

    /******************************************************************/
    /**CALIBRATION Parameters**/
    
    // MON data declarations
    
    delete[] check_first_event;
    delete[] scaler_time_count; 
    delete[] scaler_spill_count; //UInt_t
    delete[] scaler_time_check_last;//UInt_t
    delete[] scaler_spill_check_last;//UInt_t 
    delete[] check_increase_time;//UInt_t 
    delete[] check_increase_spill;//UInt_t
    delete[] scaler_increase_event;//UInt_t
    delete[] scaler_last_event;
    
    
    delete[] scaler_save;
    
    delete[] mon_inc;
    
    delete[] coin;
    
    // MW part
   delete[]  mw_xsum;     /*                          */
   delete[]  mw_ysum;     /*                          */
    
   delete[]  mw_x;        /*                          */
   delete[]  mw_y;        /*                          */
   delete[]  mw_wire;     /* special for Helmut       */
    
   delete[]  b_mw_xsum;   /*  wc on sum               */
   delete[]  b_mw_ysum;   /*                          */
    
    // TPC part
    delete[] tpc_x;
    delete[] tpc_y;
    delete[] b_tpc_xy;

    for(int i=0; i < 7; ++i){
	
	delete[] tpc_csum[i];
	delete[] b_tpc_csum[i];
	
    }
    
    
    delete[] dssd_e; //[3][2][16]
    
    /******************************************************************/
    /*** ANALYSIS Parameters ***/
    
    /*** MUSIC Conditions ***/
    
    // MUSIC part
	
	delete[] music_b_e1;
	delete[] music_b_t1;
	delete[] music_b_e2;
	delete[] music_b_t2;
	delete[] music_b_e3;
	delete[] music_b_t3;
	
	delete[] de;
	delete[] de_cor;
	
    //SCI part
	delete[] sci_l;  
	delete[] sci_r;  
	delete[]  sci_e;  
	delete[]  sci_tx;  
	delete[] sci_x;
	
	delete[]  sci_veto_l;
	delete[]  sci_veto_r;
	delete[]  sci_veto_e;
	
	delete[]  sci_b_l;  
	delete[]  sci_b_r;  
	delete[]  sci_b_e;  
	delete[]  sci_b_tx;  
	delete[]  sci_b_x;  
	delete[]  sci_b_veto_l;  
	delete[]  sci_b_veto_r;  
	delete[]  sci_b_veto_e;
	
    // ID part
    
	delete[] id_brho;      /* position-corr. BRho      */
	delete[] id_rho;       /* Position-corrected Rho   */
	
	delete[] id_b_x4AoQ_Z; 
	delete[] id_b_z_AoQ;
	delete[] id_b_music_z;
	
}

//---------------------------------------------------------------

void BS_Detector_System::Process_FRS(TGo4MbsSubEvent* psubevt){
    
    //Setup_Parameters();
    FRS_Unpack(psubevt);
    FRS_Sort();
    FRS_Calib();
    FRS_Anal();
    
    //cout<<"HOLY SHIT IT ACTUALLY FUCKING WORKED!!! "<<dt_21l_21r<<endl;
    //cout<<"HOLY FUCK IT STILL WORKS!!! "<<tpc_y[1]<<endl;
    
    //cout<<"FUCKITTY FUCK FUCKING HOLY BALLS!!! "<<sci_tofll2<<endl;
}

//-----------------------------------------------------------------//
//--------------------- UNPACKER STUFF ----------------------------//
//-----------------------------------------------------------------//

void BS_Detector_System::FRS_Unpack(TGo4MbsSubEvent* psubevent){
    
    psubevt = psubevent;

    pdata = psubevt->GetDataField();
    len = 0;	  
    //vme_chn;
    lenMax = (psubevt->GetDlen()-2)/2;
    
    const auto it_Crate = ElecMod->MapCrates.find(psubevt->GetProcid());
	if(it_Crate == ElecMod->MapCrates.end())
	std::cout<<"E> Crate Mapping does not have this ProcID :"<<psubevt->GetProcid()<<std::endl;
  
    if(ElecMod->EventFlags.size()!=0){
	
	Int_t  event_flag = *pdata++;
	len++; // 0x200
      
#ifdef DEBUG
      std::cout<<" Event FLAG :"<<std::endl;
      std::cout<<std::bitset<32>(event_flag)<<"\n";
      for(Int_t elem : ElecMod->EventFlags)
	std::cout<<std::bitset<32>(elem)<<"\n";
#endif

	bool foundFlag = false;
	for(int setupFlag : ElecMod->EventFlags)
	    if(event_flag == setupFlag)
		foundFlag = true;
    
	if(foundFlag)
	    EventFlag = event_flag;
	    
    }
    
    
    if(psubevt->GetProcid() == 10){
	

	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(5);
	
	if(psubevt->GetType() == 88){
	    

	    pdata++;
	    
	    len++;
	    
	    if(ElecMod->Nb_Scaler > 0){
		

		FRS_Scaler_Module(5);

	    }
	    
	    while (len < (psubevt->GetDlen()-2)/2)
	    {
		
		FRS_Rest_Of_Unpacking(5);

	    }   
	}
    }
    else if(psubevt->GetProcid() == 20){
	
	while(len < lenMax){
		
	    FRS_775_TDC_785_ADC_Modules(1);
		
	}
	
	while (len < (psubevt->GetDlen()-2)/2)
	{
		
	    FRS_Rest_Of_Unpacking(1);
		
	}   
	
	
    }
    else if(psubevt->GetProcid() == 30){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(0);
	
	if(psubevt->GetType() == 88){
	    	    
	    if(ElecMod->Nb_Scaler > 0){
		
		FRS_Scaler_Module(0);
		
	    }
	    
	    while(len < lenMax){
		
		FRS_775_TDC_785_ADC_Modules(0);
		
	    }
	    
	    while (len < (psubevt->GetDlen()-2)/2)
	    {
		
		FRS_Rest_Of_Unpacking(0);
		
	    }    
	    
	}
	
    }
    else if(psubevt->GetProcid() == 40){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(3);
	
	if(psubevt->GetType() == 88){

	    pdata++;
	    
	    len++; // remove 0xbaba.baba
	    
	    
	    // v820 -> no header 16 ch of 32 bits.
	    for(int i=0;i<16;++i){
		
		vme3scaler[i] = *pdata;
		
		pdata++;
		
		len++;
		
	    }
	    
	    
	    static std::bitset<32> barrier_test(0xbabababa);
	    if(std::bitset<32>(*pdata) !=barrier_test)
	    std::cout<<"E> ProcID 40 : Barrier missed !"<<std::bitset<32>(*pdata)<<" =/= 0xbabababa | "<<barrier_test<<std::endl;
	    
	    pdata++;
	    
	    len++;
	    
	    
	    if(ElecMod->Nb_TDC>0){
		
		//v1290 TDC              
		Int_t vme_geo = getbits(*pdata,1,1,5);
		Int_t vme_type = getbits(*pdata,2,12,5);
		
		pdata++;
		
		len++;
		
		Int_t multihit = 0;//, counter = 0;

		const auto it_Module = it_Crate->second.find(vme_geo);
		if(it_Module == it_Crate->second.end())
		std::cout<<"E> Crate Mapping does not have this module (vmeGEO//) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
		
		
		if (vme_type == 8){
		    
		    while (len < lenMax){
			
			vme_type = getbits(*pdata,2,12,5);
			if(vme_type==1){ // headerTDC
	
			    pdata++;
			    
			    len++;
	
			}
	
			//multihit = 0;
			
			vme_type = getbits(*pdata,2,12,5);
			
			if(vme_type == 0){
			    
			    // this indicates a TDC measurement
			    Int_t vme_chn = getbits(*pdata,2,6,5);
			    Int_t LeadingOrTrailing = getbits(*pdata,2,11,1);
			    Int_t value = getbits(*pdata,1,1,21);
			    multihit = vme3_MT_nhit5[vme_chn][LeadingOrTrailing];
			    
			    if (multihit >= 10){
				
				pdata++;
				
				len++;
				
				continue;
	
			    }
			    if(LeadingOrTrailing == 0){
				
				if (value > 0){
				    
				    vme3s_MT[vme_chn][multihit] = value;
				    //hVME2_TDC[vme_chn]->Fill(value);
				
				}
				
			    }
			    
			    else{
				
				if (value > 0) vme3s_MT_trailing[vme_chn][multihit] = value;
			    
			    }
			    
			    vme3_MT_nhit5[vme_chn][LeadingOrTrailing]++;
			    
			    pdata++;
			    
			    len++;
			    
			}
			else{
			    
			    // TDC trailer vme_type == 3
			    if(vme_type != 3 && vme_type !=16) std::cout<<"E> MTDC strange type :"<<vme_type<<std::endl;
			    
			    if(vme_type==16){
				
				Int_t vme_geoEnd = getbits(*pdata,1,1,5);
				
				if(vme_geo!=vme_geoEnd)
				    std::cout<<"E> MTDC strange end buffer header :"<<vme_type<<" "<<vme_geo<<" != "<<vme_geoEnd<<std::endl; 
				
				break;
			    }
			    
			    pdata++;
			    
			    len++;
			
			}
		    }
		}
	    }
	}
    }
    else if(psubevt->GetProcid() == 50){
	
	if(ElecMod->Nb_TimeStamp > 0 && (psubevt->GetType() == 12)) FRS_Timestamp_Module(4);
	
	
	
    }
}

//---------------------------------------------------------------

void BS_Detector_System::FRS_Timestamp_Module(int ID){
    

    // \note FRS TIME STAMP module data (3 longwords)   
    //   has no header or end-of-block info, we must hardwire GEO = 20.
    //
    
    Long64_t tempTS = 0;
    static const Long64_t bit_weight[4] = {0x1,0x10000,0x100000000,0x1000000000000};
    for (int i=0;i<4;++i){
	
	vme[ID][20][i] = getbits(*pdata,1,1,16);
	tempTS += vme[ID][20][i]*bit_weight[i];
	
	pdata++;
	
	len++;
    
    }
    
    previousTimeStamp[0] = currentTimeStamp;
    currentTimeStamp = tempTS;

}

//---------------------------------------------------------------

void BS_Detector_System::FRS_Scaler_Module(int ID){


    // \note FRS SCALER module data (1 longword per channel)   
    //  This module has sequential readout therefore no channel
    //  number contained in the data longwords. 
    //
    // read the header longword and extract slot, type & length  
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,2,3,6);
    
    pdata++;
    
    len++;
    
    
    const auto it_Crate = ElecMod->MapCrates.find(psubevt->GetProcid());
	if(it_Crate == ElecMod->MapCrates.end())
	    std::cout<<"E> Crate Mapping does not have this ProcID :"<<psubevt->GetProcid()<<std::endl;
    

    
    const auto it_Module = it_Crate->second.find(vme_geo);
	//int IdMod = it_Module->second;
	if(it_Module == it_Crate->second.end())
	    std::cout<<"E> Crate Mapping does not have this module (vmeGEO==) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
    
    
    if(vme_type!=4)
	std::cout<<"E> Scaler type missed match ! GEO"<<vme_geo<<" "<<" type 4 =/="<<vme_type<<std::endl;
    
    
    
    // read the data 
    if (vme_nlw > 0){
	
	for(int i=0;i<vme_nlw;i++){
	    
	    if(ElecMod->Scaler32bit){
		
		vme[ID][vme_geo][i] = *pdata;
	    
	    }
	    else{
		
		Int_t sc_data = get2bits(*pdata,1,1,26);
		Int_t  sc_ch = get2bits(*pdata,1,27,5);
		
		
		if(sc_ch != i)
		    std::cout<<"E> Scaler missed match channel !"<<sc_ch<<" "<<i<<" "<<psubevt->GetProcid()<<std::endl;
		    
		vme[ID][vme_geo][i] = sc_data;
	    
	    }
	    
	    pdata++; 
	    
	    len++;
	    
	}
	
    }

}

//---------------------------------------------------------------

void BS_Detector_System::FRS_775_TDC_785_ADC_Modules(int ID){
    
    
    /* read the header longword and extract slot, type & length  */
    Int_t vme_chn = 0;
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,1,9,6); 
    
    pdata++;
    
    len++;
    
    /* read the data */
    if ((vme_type == 2) && (vme_nlw > 0)){
	
	for(int i=0;i<vme_nlw;i++){  
	    
	    vme_geo = getbits(*pdata,2,12,5);
	    vme_type = getbits(*pdata,2,9,3);
	    vme_chn = getbits(*pdata,2,1,5);
	    vme[ID][vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    
	    pdata++;
	    
	    len++;
	    
	}
    
    }
    
    /* read and ignore the expected "end-of-block" longword */
    pdata++;
    
    len++;
    
}

//---------------------------------------------------------------

void BS_Detector_System::FRS_Rest_Of_Unpacking(int ID){
    
    // read the header longword and extract slot, type & length  
    
    Int_t vme_geo = getbits(*pdata,2,12,5);
    Int_t vme_type = getbits(*pdata,2,9,3);
    Int_t vme_nlw =  getbits(*pdata,1,9,6);
    
    pdata++;
    
    len++;
    
    const auto it_Crate = ElecMod->MapCrates.find(psubevt->GetProcid());
	if(it_Crate == ElecMod->MapCrates.end())
	    std::cout<<"E> Crate Mapping does not have this ProcID :"<<psubevt->GetProcid()<<std::endl;
    
    const auto it_Module = it_Crate->second.find(vme_geo);
	int IdMod = it_Module->second;
    
    if(it_Module == it_Crate->second.end())
	std::cout<<"E> Crate Mapping does not have this module (vmeGEO--) "<<vme_geo<<" in Crate :"<<psubevt->GetProcid()<<std::endl;
    
    // read the data
    if ((vme_type == 2) && (vme_nlw > 0)){
	
	for(int i=0;i<vme_nlw;i++){
	      
	    vme_geo = getbits(*pdata,2,12,5);
	    vme_type = getbits(*pdata,2,9,3);
	    vme_chn = getbits(*pdata,2,1,5);
	    
	    vme[ID][vme_geo][vme_chn] = getbits(*pdata,1,1,16);
	    //Int_t vme_statusVD = getbits(*pdata,14,1,1);
	    //Int_t vme_statusUN = getbits(*pdata,13,1,1);
	    //Int_t vme_statusOV = getbits(*pdata,12,1,1);
	    
	    pdata++;
	    
	    len++;
	
	}
	
	// read and ignore the expected "end-of-block" longword 
	pdata++;
	
	len++;
    
    }

}

//-----------------------------------------------------------------//
//----------------------- SORT STUFF ------------------------------//
//-----------------------------------------------------------------//

void BS_Detector_System::FRS_Sort(){
    

    if(EventFlag == 0x100){
	for(int i = 0; i < 4; ++i) ts_word[i] = vme[0][20][i];
    }
    else if(EventFlag == 0x200){
	for(int i = 0; i < 4; ++i) ts_word[i] = vme[3][20][i];
    }
    else if(EventFlag == 0x300){
	for(int i = 0; i < 4; ++i) ts_word[i] = vme[4][20][i];
    }

    timestamp = Long64_t(1)*ts_word[0] + Long64_t(0x10000)*ts_word[1] + Long64_t(0x100000000)*ts_word[2] + Long64_t(0x1000000000000)*ts_word[3];
    // printf("qtrigger=%d timestamp=%ld \n",qtrigger,timestamp);
    
    tsys_word[0] = Long64_t(1)*ts_word[0] + Long64_t(0x10000)*ts_word[1] ; //s time low word
    tsys_word[1] = Long64_t(0x100000000)*ts_word[2] + Long64_t(0x1000000000000)*ts_word[3] ; //s time high worid... we do not use it

    if(PreviousTS < 0) tsys_word[2] = 0;
 
    else tsys_word[2] = (timestamp - PreviousTS)*1.e-5 ; //ms time since the previous s time (ftime routine)

    systemtime_s = tsys_word[2]*1e-3; //tsys_word[0] ; 
    systemtime_ms= tsys_word[2]; 


    if (qtrigger == 12){
	
	StartOfSpilTime = timestamp;
      
	StartOfSpilTime2 = timestamp;
    }   
    else if (qtrigger == 13){
      
	StartOfSpilTime = -1;
    }
    // rest are interesting only if trigger == 1
  
    // calculate time from spill start in sec
    if (StartOfSpilTime >= 0){
	 
	timespill = (timestamp - StartOfSpilTime) * 1e-2;// microsec // 50000000.;
    }
  
    timespill2 = (timestamp - StartOfSpilTime2) * 1e-2; //microsec  // 50000000.;

    /* ### Pattern ###*/
    pattern = vme[0][5][0];
    trigger = qtrigger;
  

  /* ### scalers:  */
  /* these are treated as 32-bit integers directly  */
    for(int i = 0 ;i < 32; ++i){
	
	sc_long[i]  = vme[0][0][i]; //frs crate
	sc_long[32+i] = vme[5][8][i]; //main crate

    }

  /* ### TA Ionization Chamber dE:  */
  
    
  /* ### MW anodes:  */
  for(int i = 0; i < 13; ++i)
    mw_an[i] = vme[0][8][i] & 0xfff;
    
  /* ### MW cathodes:  */
  // from MW11 -> MW31
  for(int i = 0; i < 4; ++i)
    {  
      mw_xr[i] = vme[0][8][16+i*4] & 0xfff;
      mw_xl[i] = vme[0][8][17+i*4] & 0xfff;
      mw_yu[i] = vme[0][8][18+i*4] & 0xfff;
      mw_yd[i] = vme[0][8][19+i*4] & 0xfff;
    }
  // from MW41 -> MW51
  for(int i = 0 ;i < 3; ++i)
    {
      mw_xr[i+4] = vme[0][9][0+i*4] & 0xfff;
      mw_xl[i+4] = vme[0][9][1+i*4] & 0xfff;
      mw_yu[i+4] = vme[0][9][2+i*4] & 0xfff;
      mw_yd[i+4] = vme[0][9][3+i*4] & 0xfff;
    }
  // skip MW61
  //putting MW61 all to zero (09.07.2018)
      mw_xr[7] = 0;
      mw_xl[7] = 0;
      mw_yu[7] = 0;
      mw_yd[7] = 0;
     
  // from MW71 -> MWB2
  for(int i = 0; i < 5; ++i)
    {
      mw_xr[i+8] = vme[0][9][12+i*4] & 0xfff;
      mw_xl[i+8] = vme[0][9][13+i*4] & 0xfff;
      mw_yu[i+8] = vme[0][9][14+i*4] & 0xfff;
      mw_yd[i+8] = vme[0][9][15+i*4] & 0xfff;
    }



    //////////////////////////////////////
    //                                  //
    // 		  TPC part              //
    //                                  //
    //////////////////////////////////////
    
    //ADC
    for(int i = 0; i < 4; ++i){ // TPC 1->4 at S2 (TPC 21 & 22) in vacuum (TPC 23 & 24) in air
    	
	tpc_a[i][0]=vme[1][15][0 + i*8] & 0xfff;
	tpc_a[i][1]=vme[1][15][1 + i*8] & 0xfff;
	tpc_a[i][2]=vme[1][15][2 + i*8] & 0xfff;
	tpc_a[i][3]=vme[1][15][3 + i*8] & 0xfff;
	tpc_l[i][0]=vme[1][15][4 + i*8] & 0xfff;
	tpc_r[i][0]=vme[1][15][5 + i*8] & 0xfff;
	tpc_l[i][1]=vme[1][15][6 + i*8] & 0xfff;
	tpc_r[i][1]=vme[1][15][7 + i*8] & 0xfff;
	
	
	tpc_dt[i][0]=vme[1][8][0 + i*8] & 0xfff;
	tpc_dt[i][1]=vme[1][8][1 + i*8] & 0xfff;
	tpc_dt[i][2]=vme[1][8][2 + i*8] & 0xfff;
	tpc_dt[i][3]=vme[1][8][3 + i*8] & 0xfff;
	tpc_lt[i][0]=vme[1][8][4 + i*8] & 0xfff;
	tpc_rt[i][0]=vme[1][8][5 + i*8] & 0xfff;
	tpc_lt[i][1]=vme[1][8][6 + i*8] & 0xfff;
	tpc_rt[i][1]=vme[1][8][7 + i*8] & 0xfff;
	
	
	
    }
    for(int i = 0; i < 3; ++i){ // TPC 5 & 6 at S4 (TPC 41 & 42) in air & TPC at S3 (TPC 31) (CheckSB)
    	
	tpc_a[i+4][0]=vme[1][3][0 + i*8] & 0xfff;
	tpc_a[i+4][1]=vme[1][3][1 + i*8] & 0xfff;
	tpc_a[i+4][2]=vme[1][3][2 + i*8] & 0xfff;
	tpc_a[i+4][3]=vme[1][3][3 + i*8] & 0xfff;
	tpc_l[i+4][0]=vme[1][3][4 + i*8] & 0xfff;
	tpc_r[i+4][0]=vme[1][3][5 + i*8] & 0xfff;
	tpc_l[i+4][1]=vme[1][3][6 + i*8] & 0xfff;
	tpc_r[i+4][1]=vme[1][3][7 + i*8] & 0xfff;
	
	tpc_dt[i+4][0]=vme[1][9][0 + i*8] & 0xfff;
	tpc_dt[i+4][1]=vme[1][9][1 + i*8] & 0xfff;
	tpc_dt[i+4][2]=vme[1][9][2 + i*8] & 0xfff;
	tpc_dt[i+4][3]=vme[1][9][3 + i*8] & 0xfff;
	tpc_lt[i+4][0]=vme[1][9][4 + i*8] & 0xfff;
	tpc_rt[i+4][0]=vme[1][9][5 + i*8] & 0xfff;
	tpc_lt[i+4][1]=vme[1][9][6 + i*8] & 0xfff;
	tpc_rt[i+4][1]=vme[1][9][7 + i*8] & 0xfff;
	
    }

  
    /* ### SCI dE:  */
    de_21l = vme[5][11][2] & 0xfff;
    de_21r = vme[5][11][3] & 0xfff;
 
    de_41l = vme[5][11][0] & 0xfff;
    de_41r = vme[5][11][1] & 0xfff;
    de_42l = vme[5][11][4] & 0xfff; 
    de_42r = vme[5][11][5] & 0xfff;
    de_43l = vme[5][11][11] & 0xfff;
    de_43r = vme[5][11][12] & 0xfff;  
  
    de_81l = vme[5][11][13] & 0xfff;
    de_81r = vme[5][11][14] & 0xfff;
  
    de_v1l = 0;
    de_v1r = 0;  
    de_v2l = 0;
    de_v2r = 0;
    de_v3  = 0;

    
    /* ### SCI times:  */
    dt_21l_21r = vme[0][12][0] & 0xfff;
    dt_41l_41r = vme[0][12][1] & 0xfff;
    dt_21l_41l = vme[0][12][5] & 0xfff;
    dt_21r_41r = vme[0][12][6] & 0xfff;
    dt_42l_42r = vme[0][12][2] & 0xfff;  
    dt_42l_21l = vme[0][12][7] & 0xfff; //
    dt_42r_21r = vme[0][12][8] & 0xfff; //
    dt_43l_43r = vme[0][12][3] & 0xfff; //
    dt_81l_81r = vme[0][12][4] & 0xfff;
    dt_21l_81l = vme[0][12][9] & 0xfff;
    dt_21r_81r = vme[0][12][10] & 0xfff;

 
  /* ### SCI Multihit TDC time:  */
    for(int i = 0; i < 10; ++i){
    
	tdc_sc41l[i] = vme2s[0][i];
	tdc_sc41r[i] = vme2s[1][i];
	tdc_sc21l[i] = vme2s[2][i];
	tdc_sc21r[i] = vme2s[3][i];
	tdc_sc42l[i] = vme2s[4][i];
	tdc_sc42r[i] = vme2s[5][i];
	tdc_sc43l[i] = vme2s[6][i];
	tdc_sc43r[i] = vme2s[7][i];
	tdc_sc81l[i] = vme2s[8][i];
	tdc_sc81r[i] = vme2s[9][i];
    
    }
  

  /* ### MUSIC OLD:  */
    for(int i=0;i<4;i++){
      
	music_t3[i] = vme[1][11][16+i] & 0xfff;    //TIME
	//music_t3[i] = 0;
	music_e3[i] = vme[0][10][16+i] & 0xfff;    //ENERGY
	//music_e3[i] = 0;

    }
               
    /* ### TUM MUSIC dE:  */
    for(int i=0;i<8;i++){
	
	music_e1[i] = vme[0][11][i] & 0xfff;
	music_e2[i] = vme[0][11][8+i] & 0xfff;
	
	//music_e2[i] = vme[0][12][24+i] & 0xfff;
	
	music_t1[i] = vme2s[16+i][0] & 0xfff; //TIME
	music_t2[i] = vme2s[24+i][0] & 0xfff; //TIME
    
    }
    
    /* ### MUSIC temp & pressure:  */
    music_pres[0] = 0; 
    music_temp[0] = 0;
    
    /*  not included in readout! */
    music_pres[1] = 0;
    music_temp[1] = 0;
    
    /*  not included in readout! */
    music_pres[2] = 0;
    music_temp[2] = 0;
    
    //Single anode 
    SingleAnode_adc = vme[0][10][20] & 0xfff; 
    
    //Si detectors 
    si_adc1 = vme[0][10][24] & 0xfff; 
    si_adc2 = vme[0][10][25] & 0xfff;
    si_adc3 = vme[0][10][26] & 0xfff; 
    si_adc4 = vme[0][10][27] & 0xfff;
    si_adc5 = vme[0][10][28] & 0xfff;
    
    //Si detectors DSSD = alpha tagger
    for(int i=0;i<32;i++){
	
	dssd_adc[i] = vme[0][7][i] & 0xfff;
    
    }

    //Channeltron detectors (vme must be adjusted)
    ct_time = vme3s_MT[2][0] & 0xfff;
    ct_signal = vme2scaler[20] & 0xfff; 
    ct_trigger_DU = vme2scaler[5]& 0xfff;
    ct_trigger_SY = vme2scaler[6]& 0xfff;
    
    //Electron current measurement (vme must be adjused)
    ec_signal = vme[0][10][1]& 0xfff;
    
    // mrtof
    mrtof_start = vme3s_MT[0][0] ;//& 0x7ffff;
    mrtof_stopDelay = vme3s_MT[1][0];// & 0x7ffff;
    mrtof_stop = vme3s_MT[2][0] ;//& 0x7ffff;
    
    
}


void BS_Detector_System::FRS_Calib(){
    
  //----- from here added by YKT 23.05 --------//   
  /////// 
  ///////  scaler_time_count,  scaler_spill_count,  scaler_time_check_last,   scaler_spill_check_last,  scaler_increase_event[64]     UInt_t
  ///////  check_increase_time[i]   check_increase_spill[i],   scaler_last_event[i]    UInt_t 

  // special channels for normalization
  int scaler_channel_spill_all[2] = {8, 5}; double normalization_factor_spill_all[2] = {1.0,1.0}; //   YKT 23.05
  //int scaler_channel_time_all[2]  = {36,1}; double normalization_factor_time_all[2] = {10.0,100.}; //  36 for 10 Hz clock
  int scaler_channel_time_all[2]  = {36,1}; double normalization_factor_time_all[2] = {1.,100.}; //  36 for 10 Hz clock (normalization 100ms)
  // int scaler_channel_time_all[2]  = {38,1}; double normalization_factor_time_all[2] = {100000.0,100.}; //38 for 100kHz clock
  
  scaler_channel_10khz = 25; //YT 20Jun.
  Long64_t nmax_V830 = 0x100000000;

  //printf("Event_flag_test=0x%08x\n", EventFlag);

  int index_flag = -1;
  if(EventFlag==0x100)
    index_flag=0;
  /*else if(EventFlag==0x200)
    index_flag=1;
  else if(EventFlag==0x300)
  index_flag=1;*/
  else
    std::cout<<"E> EventFlag unknown !"<<EventFlag<<std::endl;


  int scaler_channel_spill = scaler_channel_spill_all[index_flag];
  double normalization_factor_spill = normalization_factor_spill_all[index_flag]; //   YKT 23.05
  int scaler_channel_time  = scaler_channel_time_all[index_flag];
  double normalization_factor_time = normalization_factor_time_all[index_flag] ; //  YKT 23.05 

  //UInt_t tempCurrentTime = index_flag == 0 ? sc_long[scaler_channel_time] : sc_long2[scaler_channel_time];
  UInt_t tempCurrentTime = sc_long[scaler_channel_time];
  
  //----initialize values defined in this file---
  if(1==check_first_event[index_flag]) // if first event has 0 (for all channels) >> use next event as a first event 
    {
      //      std::cout<<"Init Scaler :"<<index_flag<<" "<<EventFlag<<" | "<<check_first_event[index_flag]<<" "<<tempCurrentTime<<"\n";
      if(0!=tempCurrentTime)
	{
	  //std::cout<<" +--> set initial values ";
	  scaler_time_count[index_flag]  = 0; //UInt_t
	  scaler_spill_count[index_flag] = 0; //UInt_t
	  scaler_time_check_last[index_flag] = 0;//UInt_t
	  scaler_spill_check_last[index_flag] = 0;//UInt_t
	  for(size_t i=0; i<64; i++) //changed from 32 to 64 (10.07.2018)
	    {
	      check_increase_time[i]   =0;//UInt_t
	      check_increase_spill[i]  =0;//UInt_t
	      scaler_increase_event[i] =0;//UInt_t
	      scaler_last_event[i] = static_cast<Long64_t>(sc_long[i]);//UInt_t
	    }
	  /*
	  for(size_t i=32;i<64;++i)
	    {
	      check_increase_time[i]   =0;//UInt_t
	      check_increase_spill[i]  =0;//UInt_t
	      scaler_increase_event[i] =0;//UInt_t
	      scaler_last_event[i] = static_cast<Long64_t>(sc_long2[i-32]);//UInt_t
	    }
	  */

	  //std::cout<<" done !\n";
	}
    }
  for(int i=0; i<64; i++)
    scaler_increase_event[i]=0;  
  
  //------------ scaler_increase_event[i] and scaler_last_event[i] -----------
  //////
  // sometimes V830 data from sort is empty (all 0)
  // in such case, we skip updating { scaler_increase_event[i] and scaler_last_event[i] }
  if(0!=tempCurrentTime)
    {
      if(0==check_first_event[index_flag])
	{
	  if(index_flag==0)
	    {
	      for(size_t i=0; i<64; i++) //changed from 32 to 64 (10.07.2018)
		{
		  if(static_cast<Long64_t>(sc_long[i]) >= scaler_last_event[i])
		    scaler_increase_event[i] = sc_long[i] - scaler_last_event[i];
		  else
		    {
		      //printf("sc_long[i], scaler_last_event[i]:%d %d\n", sc_long[i] , scaler_last_event[i]);
		      scaler_increase_event[i] =  sc_long[i]  + nmax_V830 - scaler_last_event[i];
		    }
		  scaler_last_event[i]     = sc_long[i];
		}
	    }
	  /*
	  if(index_flag==1)
	    {
	      for(size_t i=32; i<64; i++)
		{
		  if(static_cast<Long64_t>(sc_long2[i-32]) >= scaler_last_event[i])
		    scaler_increase_event[i] = sc_long2[i-32] - scaler_last_event[i];
		  else
		    {
		      //printf("sc_long[i], scaler_last_event[i]:%d %d\n", sc_long[i] , scaler_last_event[i]);
		      scaler_increase_event[i] =  sc_long2[i-32]  + nmax_V830 - scaler_last_event[i];
		    }
		  scaler_last_event[i]     = sc_long2[i-32];
		}	  
		}*/
	}
    }
  //-----switch off initial event check---
  if(1==check_first_event[index_flag] && (0!=tempCurrentTime)) // if first event has 0 (for all channels) >> use next event as a first event
    { 
      check_first_event[index_flag] = 0;
    }


  // add {increase from last event} to the counters.
  if(0!=tempCurrentTime)
    {
      for(int i=0; i<64; i++)
	{
	  check_increase_spill[i] += scaler_increase_event[i];  
	  check_increase_time[i]  += scaler_increase_event[i];
	}
    }
 
  // integrated count from the beginning
  if(0!=tempCurrentTime)
    {
      scaler_time_count[index_flag]  += scaler_increase_event[scaler_channel_time]; //
      scaler_spill_count[index_flag] += scaler_increase_event[scaler_channel_spill];//
    }
  
  int scaler_time_check  = scaler_time_count[index_flag]/((int)normalization_factor_time);
  int scaler_spill_check = scaler_spill_count[index_flag];

  //  printf("scaler_time_count = %d, sc_long[4]=%d, sc_long[3]=%d, \n",scaler_time_count, sc_long[4], sc_long[3]);
 

  // when scaler_time_check is increased
  if( 0<(scaler_time_check - scaler_time_check_last[index_flag]) )
    {
      //   printf("scaler_time_check = %d, scaler_time_check_last = %d \n",scaler_time_check,scaler_time_check_last);
      //if(10<(scaler_time_check - scaler_time_check_last[index_flag])) //10 Hz clock
      if(1<(scaler_time_check - scaler_time_check_last[index_flag])) //10 Hz clock //for normalization 100ms
      //if(100000<(scaler_time_check - scaler_time_check_last[index_flag])) //100 kHz clock
	{
	  //printf("scaler_time_check - scaler_time_check_last = %d ...\n",);
	  std::cout<<"EventFlag"<<EventFlag<<"|"<<index_flag<<"scaler_time_check - scaler_time_check_last = "<<(scaler_time_check - scaler_time_check_last[index_flag])<<"...\n";
	} 
      for(int i=0; i<64; i++)
	{
	  //int x_bin = (scaler_time_check % 3000);
	  //int x_bin_short = (scaler_time_check % 300);
	  int y_set;
	  if(check_increase_time[i]>0)
	    {
	      y_set =  (int)(normalization_factor_time*((Float_t)( check_increase_time[i] ))/((Float_t)( check_increase_time[scaler_channel_time] ))); 
	    }
	  else
	    {
	      y_set = 0;
	    }
	}
      for(int i=0; i<64; i++)
	{
	  check_increase_time[i]=0;
	}//reset
      scaler_time_check_last[index_flag] = scaler_time_check;  
    }

  // when scaler_time_check is increased
  if( 0<(scaler_spill_check - scaler_spill_check_last[index_flag]) )
    {
      if(10<(scaler_spill_check - scaler_spill_check_last[index_flag]))
	{
	  //printf("scaler_spill_check - scaler_spill_check_last = %d ...\n",(scaler_spill_check - scaler_spill_check_last));
	  std::cout<<"EventFlag"<<EventFlag<<"|"<<index_flag<<"scaler_spill_check - scaler_spill_check_last = "<<(scaler_spill_check - scaler_spill_check_last[index_flag])<<"...\n";
	}
      for(int i=0; i<64; i++)
	{
	  //int x_bin = (scaler_spill_check % 300);
	  //int x_bin_short = (scaler_spill_check % 30);
	  int y_set;
	  if(check_increase_spill[i]>0)
	    {
	      y_set =  (int)(normalization_factor_spill*((Float_t)( check_increase_spill[i] ))/((Float_t)( check_increase_spill[scaler_channel_spill] )));
	    }
	  else
	    {
	      y_set = 0;
	    }
	}
      //
      if(index_flag==0)
	{
	  check_total_sc21 += check_increase_spill[7];
	  check_total_sc41 += check_increase_spill[8];
	  check_total_seetram += check_increase_spill[10];
      //printf("Total SC41 = %d,  Total SC21 = %d, Total SEETRAM = %d \n",check_total_sc41,check_total_sc21,check_total_seetram);
	}
      if(index_flag==1)
	{
	  check_total_mrtof_start += check_increase_spill[38];
	  check_total_mrtof_stop += check_increase_spill[39];
	    
	}
      for(int i=0; i<64; i++)
	{
	  check_increase_spill[i]=0;
	}//reset                                                                                                                                                       
      scaler_spill_check_last[index_flag] = scaler_spill_check;
      //           
    }
  //----- up to here added by YKT 23.05 --------// 
   
   UInt_t first[64];
  //  Int_t  first[64]; 

  if (fbFirstEvent)
    { //
      for (int i=0;i<64;i++)  //changed here from 32 to 64 (10.07.2018)
	{
	  //std::cout <<"In first event loop"<<std::endl ; 
	  scaler_save[i] = sc_long[i];
	  //		 std::cout <<"  "<<scaler_save[i] <<"  "<<sc_long[i]<<std::endl ; 
	  first[i]=sc_long[i];
	}
      /*
      for (int i=32;i<64;i++)
	{
	  scaler_save[i] = sc_long2[i-32];
	  first[i]=sc_long2[i-32];
	}
      */

      //    std::cout <<"1Hz  "<<sc_long[3]<<std::endl;
      //    std::cout <<"10Hz "<<sc_long[4]<<std::endl;  

      if(sc_long[35]!=0)
	{     
	  firstsec = sc_long[35]; // TIME //
	  firsttenthsec = sc_long[36];
	  firsthundrethsec = sc_long[38];	//mik
	  firstcycle = sc_long[34];
	  firstseetram= sc_long[2]; // new SE01
	  //      firstseetram= sc_long[9]; // old SE01

	  fbFirstEvent = kFALSE; 
	}
    }


  //  Int_t mon_inc[64];

  Double_t over_scale=  4000000000.;
  //  printf("%15f",over_scale);

  // for 1st module
  for (int i=0;i<32;i++)
    if(0!=sc_long[scaler_channel_time])
      {
	Int_t overload = 0; 
	//  if(sc_long[i]!=0){            
	if ( scaler_save[i] > static_cast<Long64_t>(sc_long[i]))
	  {
	    if (trigger==12 || trigger==13)
	      continue ;

	    //std::cout <<"Hey I got overloaded !!! channel "<<i<<std::endl ; 
	    //std::cout <<scaler_save[i] <<"  "<<sc_long[i]<<std::endl ;
	    //std::cout <<"trigger is : "<<trigger<<std::endl ; 
	    //scaler_save[i] = scaler_save[i] - 4294967295;
	    mon_inc[i] = static_cast<Long64_t>(sc_long[i])+4294967295 - scaler_save[i]; //
	    //scaler_save[i] = (Long64_t)sc_long[i]);
	    overload = 1;
	  }
	else
	  mon_inc[i] = static_cast<Long64_t>(sc_long[i]) - scaler_save[i]; //
	 
	scaler_save[i] = static_cast<Long64_t>(sc_long[i]);
	//
	//if(overload != 0)
	//std::cout<<"case overload"<<std::endl;
	if (mon_inc[i]<0)
	  {
	    //std::cout <<"  "<<i<<"  "<<mon_inc[i]<<"  "<<sc_long[i]<<"  "<<scaler_save[i]<<" | "<<overload<<std::endl ; 
	    std::cout <<"!> mon_inc["<<i<<"] -> WRONG"<<std::endl ;
	  } 


	/* save the current value so that it can be used next time around... */
	//if(mon_inc[i]>4000000000)mon_inc[i]=0; 
	//  if(mon_inc[i]>over_scale)mon_inc[i]=0; 
      
      } 

  // for 2nd module
  for (int i=32;i<64;i++)
    if(0!=sc_long[scaler_channel_time])
      {
	Int_t overload = 0;
	//if (scaler_save[i] > sc_long2[i-32])
	if (scaler_save[i] > sc_long[i])
	  { 
	    //scaler_save[i] = scaler_save[i] - 4294967295;
	    //mon_inc[i] = static_cast<Long64_t>(sc_long2[i-32]) + 4294967295 - scaler_save[i];
	    mon_inc[i] = static_cast<Long64_t>(sc_long[i]) + 4294967295 - scaler_save[i];
	    overload =1;
	  }
	else
	  //mon_inc[i] = sc_long2[i-32] - scaler_save[i];
	  mon_inc[i] = sc_long[i] - scaler_save[i];
	   
	
	if(mon_inc[i] > over_scale)
	  mon_inc[i]=0;

	//save the current value so that it can be used next time around... 
	//scaler_save[i] = sc_long2[i-32];
	scaler_save[i] = sc_long[i];

      
      }

  //Store scalers for later
  freeTrig=mon_inc[0];
  acptTrig=mon_inc[1];
  
  
  if(sc_long[35]!=0)
    {
      if(sc_long[35]<firstsec)
	++scalercycle_Sec;
      //firstsec = sc_long[3];	  
      
      if(sc_long[36]<firsttenthsec)
	++scalercycle_TenthSec;
	
      if(sc_long[38]<firsthundrethsec)		//mik
	++scalercycle_HundrethSec;
	
      if(sc_long[34]<firstcycle)
	++scalercycle_Cycle;


      seconds = sc_long[35] + 4294967295*scalercycle_Sec -firstsec;
      tenthsecs = sc_long[36] + 4294967295*scalercycle_TenthSec - firsttenthsec;
      hundrethsecs = sc_long[38] + 4294967295*scalercycle_HundrethSec - firsthundrethsec;  //mik
      extraction_cycle = sc_long[34] + 4294967295*scalercycle_Cycle - firstcycle;
    }


  for(int i=0;i<16;i++)
    {
      coin[i]=pattern>>i & 0x0001;

    }




  
  /**  for SeeTram calibration purposes:   **/


  //10HzClock = hMON_scaler[4]->Integral();

  /*   Naming conventions:  index     detector   focus #                     */
  /*                         1         MW11        1                         */
  /*                         2         MW21        2                         */
  /*                         3         MW22                                  */
  /*                         4         MW31        3                         */
  /*                         5         MW41                                  */
  /*                         6         MW42                                  */
  /*                         7         MW51                                  */
  /*                         8         MW61                                  */
   /*                        9         MW71                                  */
  /*                         10        MW81                                  */
  /*                         11        MW82                                  */
  /*                         12        MWB1                                  */
  /*                         13        MWB2                                  */
  /////////////////////////////////////////////////////////////////////////////
  int max_index = 13; //upto MWB2 (09.07.2018) /*  up to S3 */

  for(int i=0;i<max_index;i++)
    {	
      
      /********************************************/
      /* Calculate the sum spectra and conditions */
      /********************************************/
      
      /* better test first existence of xl, xr, an before filling of xsum */
      
      if(mw_an[i] && mw_xl[i] && mw_xr[i])
	{
	  // if(mw_xl[i]&&mw_xr[i]) {
	  mw_xsum[i] = 1000+(mw_xl[i] - mw_an[i]) + (mw_xr[i] - mw_an[i]);
      
	  // mw_xsum[i] = (mw_xl[i]) + (mw_xr[i]); //when an doesn't work
	}


	b_mw_xsum[i] = Check_WinCond_Multi(mw_xsum[i], lim_xsum, i);

      //if(mw_xsum[i] >= lim_xsum[i][0] && mw_xsum[i] <= lim_xsum[i][1]) b_mw_xsum[i] =  true; //cMW_XSUM[i]->Test(mw_xsum[i]);
      //else b_mw_xsum[i] =  false;
      
      
      /* better test first existence of yu, yd, an before filling of ysum */
      
      if(mw_an[i] && mw_yu[i] && mw_yd[i])
	{
	   
	  //if(mw_yu[i]&&mw_yd[i]) {
	  mw_ysum[i] = 1000+(mw_yu[i] - mw_an[i]) + (mw_yd[i] - mw_an[i]);	  
	  
	  //mw_ysum[i] = (mw_yu[i]) + (mw_yd[i]); //when an doesn't work			
	}
      
      	b_mw_ysum[i] = Check_WinCond_Multi(mw_ysum[i], lim_ysum, i);

	//if(mw_ysum[i] >= lim_ysum[i][0] && mw_ysum[i] <= lim_ysum[i][1]) b_mw_ysum[i] =  true; //cMW_YSUM[i]->Test(mw_ysum[i]);
	//else b_mw_ysum[i] =  false;
    
      /*******************************************************************/
      /* If the signals in x and y are valid, calculate position spectra */
      /*******************************************************************/
      
      if (b_mw_xsum[i])
	{
	  //      Int_t r_x = mw_xl[i] - mw_xr[i];
	  Float_t r_x = mw_xl[i] *  mw->gain_tdc[1][i] - mw_xr[i] *  mw->gain_tdc[2][i]; //14.09.05 CN+AM
	  mw_x[i] = mw->x_factor[i] * r_x + mw->x_offset[i];
	  	  
	}
    
      if (b_mw_ysum[i])
	{
	  //      Int_t r_y = mw_yd[i] - mw_yu[i];
	  Float_t r_y = mw_yd[i] *  mw->gain_tdc[4][i] - mw_yu[i] *  mw->gain_tdc[3][i]; //14.09.05 CN+AM
	  mw_y[i] = mw->y_factor[i] * r_y + mw->y_offset[i];
	}

    
      if(b_mw_xsum[i] && b_mw_ysum[i])
	{
	    
	}
      
    } // for(int i=0;i<max_index;i++)


  /*********************************************/
  /* S2 Angle and transformed position spectra */
  /*********************************************/
  
  Float_t  dist_MW21_MW22  = frs->dist_MW22  - frs->dist_MW21;
  Float_t  dist_MW22_focS2 = frs->dist_focS2 - frs->dist_MW22;
  Float_t  dist_MW22_SC21  = frs->dist_SC21  - frs->dist_MW22;
  Float_t  dist_MW22_SC22  = frs->dist_SC22  - frs->dist_MW22;
  

  if (b_mw_xsum[1] && b_mw_xsum[2])
    {
      /* MW21 and MW22 X okay */
      /*  X angle at S2  [mrad]:  */
      angle_x_s2 = (mw_x[2]-mw_x[1])/dist_MW21_MW22*1000.;

      /*  X at nominal S2 focus:  */
      focx_s2 = mw_x[2] + dist_MW22_focS2 * angle_x_s2/1000.;
      
      /*  X at SC21 position:    */
      sc21_x = mw_x[2] + dist_MW22_SC21 * angle_x_s2/1000.;

      /*  X at SC22 position:    */
      sc22_x = mw_x[2] + dist_MW22_SC22 * angle_x_s2/1000.;
      

      /* 'real' z-position of S2 X focus (cm) */
      Float_t rh = (angle_x_s2 - angle_x_s2m);
      if(fabs(rh)>1e-4)
	{
	  z_x_s2 = ((focx_s2m - focx_s2)/rh)*100. + frs->dist_focS2/10.;  
	}
    
      /* keep values for next event */
      focx_s2m = focx_s2; 
      angle_x_s2m = angle_x_s2;
    }
  
  if (b_mw_ysum[1] && b_mw_ysum[2])
    {
      /* MW21 and MW22 Y okay */
      /*  Y angle at S2 [mrad]:   */
      angle_y_s2 = (mw_y[2] - mw_y[1])/dist_MW21_MW22*1000.;
      
      /*  Y at nominal S2 focus:  */
      focy_s2 = mw_y[2] + dist_MW22_focS2 * angle_y_s2/1000.;

      /* 'real' z-position of S2 Y focus (cm) */
      Float_t rh = (angle_y_s2 - angle_y_s2m);
      if(fabs(rh)>1.e-4)
	{
	  z_y_s2 = ((focy_s2m - focy_s2)/rh)*100. + frs->dist_focS2/10.;  
	}
      
      /* keep values for next event */
      focy_s2m = focy_s2; 
      angle_y_s2m = angle_y_s2; 
    }
  
  /*if (b_mw_ysum[1] && b_mw_ysum[2] && b_mw_xsum[1] && b_mw_xsum[2])
    hMW_FocS2->Fill(focx_s2,focy_s2);*/ 


//////////////////////////////////////////////////////////////////
///  ***********TPC Analysis*************************** //////////

 //================================
  // Nomenclature for TPCs
  //================================
  // TPCs at S2
  // TPC 1 = TPC 21 (in vaccum) [i=0]
  // TPC 2 = TPC 22 (in vaccum) [i=1]
  // TPC 3 = TPC 23 (in air)    [i=2]
  // TPC 4 = TPC 24 (in air)    [i=3]

  // TPCs at S4
  // TPC 5 = TPC 41 (in air)    [i=4]
  // TPC 6 = TPC 42 (in air)    [i=5]

  //TPC at S3
  // TPC 7 = TPC 31             [i=6]
  

  for(int i=0;i<7;i++)
    {  

      int count =0;
      Int_t r_y = 0;
      
      int countx =0;
      Int_t r_x0 = 0;
      Int_t r_x1 = 0;
      
      tpc_y[i] = 0;
      tpc_x[i] = 9999;

      for(int j=0;j<4;j++)
	{

	  //// calculate control sums
	  if(j < 2)
	    tpc_csum[i][j] = (tpc_lt[i][0] + tpc_rt[i][0]- 2*tpc_dt[i][j]);
	  else
	    tpc_csum[i][j] = (tpc_lt[i][1] + tpc_rt[i][1]- 2*tpc_dt[i][j]);


	       // if((de_42l>230&&de_42l<450)||(de_42r>540&&de_42r<750)){

	  //b_tpc_csum[i][j] = cTPC_CSUM[i][j]->Test(tpc_csum[i][j]);
	  
	  b_tpc_csum[i][j] = Check_WinCond_Multi_Multi(tpc_csum[i][j], lim_csum, j, i);
	  
	  
	  //if(tpc_csum[i][j] >= lim_csum[j][i][0] && tpc_csum[i][j] <= lim_csum[j][i][1]) b_tpc_csum[i][j] = true;
	  //else b_tpc_csum[i][j] = false;
	       // }
	  
	  if(tpc_lt[i][0]==0 && tpc_rt[i][0]==0 && j<2)
	    b_tpc_csum[i][j]=0;
      
	  if(tpc_lt[i][1]==0 && tpc_rt[i][1]==0 && j>1)
	    b_tpc_csum[i][j]=0;
      
	  if (b_tpc_csum[i][j])
	    {
	      r_y += tpc_dt[i][j];
	      tpc_y[i] += tpc->y_factor[i][j]*tpc_dt[i][j] + tpc->y_offset[i][j];
	      count++;
	    }
	}

      if (b_tpc_csum[i][0] || b_tpc_csum[i][1])
	{
	  r_x0 =  tpc_lt[i][0]-tpc_rt[i][0];
	  //          r_x0 =  tpc_lt[i][0];
	  
	  tpc_x[i]=tpc->x_factor[i][0]*r_x0 + tpc->x_offset[i][0];
	  countx++;
	}
      
      if (b_tpc_csum[i][2] || b_tpc_csum[i][3])
	{
	  r_x1 =  tpc_lt[i][1]-tpc_rt[i][1];
	  if (countx == 0)
	    tpc_x[i]=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  else
	    tpc_x[i]+=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];
	  countx++;
	}

      b_tpc_xy[i] = kFALSE;  // HWE

      if (countx > 0)
	{
	  tpc_y[i] = tpc_y[i]/count;
	  tpc_x[i] = tpc_x[i]/countx;
	  //Int_t r_x=tpc_lt[i]-tpc_rt[i];
	  //tpc_x[i]=tpc->x_factor[i]*r_x + tpc->x_offset[i];
	  
	  //      if(r_x0<-40)

	  b_tpc_xy[i] = kTRUE;
	}
      
      if(countx>1)
	{
	  //float x0=tpc->x_factor[i][0]*r_x0 + tpc->x_offset[i][0];
	  //float x1=tpc->x_factor[i][1]*r_x1 + tpc->x_offset[i][1];

	}    
    }


  //*********************************************
  // *****Relative distances for tracking *******
  //*********************************************
  //Float_t dist_TPC1_TPC2 = 1.;         
  //distances at S2 for exp s388 measured 'by eye' (i.e. not very precise)! AE, 8.8.2012
  //Float_t dist_TPC4_target1 = 455.;  //8.8.12, s388: taget1 is first Si detector at S2 
  //using: TPC4->chamber= 150 mm, chamber->Si(1)= 305 mm
  //Float_t dist_TPC3_TPC4 = 610.; // exp S417+S411 Oct2014
  //Float_t dist_TPC3_focS2 = 2860 - 2013 ; //exp S411 Oct2014
  //Float_t dist_TPC3_focS2 = 0. ; //exp S417 Oct2014 focus on TPC21
  //Float_t dist_TPC5_TPC6 = 1595. - 385. ; // Oct.2014, exp s411 
  //Float_t dist_TPC6_focS4  = 2349. - 1595. ; // Oct.2014, exp s411
  //THIS SHOULD BE NOT HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //Yhis was for Uranium--- see logbook page 6
  //Float_t dist_TPC6_target2  = 1015.; // 8.08.12, exp s388 target2= OTPC entrance (check distance)
  //Float_t dist_TPC4_target1 = 455.;  // 8.8.12, s388: taget1 is first Si detector at S2 
                                     // using: TPC4->chamber= 150 mm, chamber->Si(1)= 305 mm

  //==========================
  // Distances of TPCs at S2
  //==========================

  Float_t dist_TPC21_TPC22 = 1782.5 - 604.0; //check
  Float_t dist_TPC23_TPC24 = 1782.5 - 604.0; //check
  Float_t dist_TPC22_TPC24 = 1782.5 - 604.0; //check

  Float_t dist_TPC21_focS2 = 604.-2165.; //check
  Float_t dist_TPC22_focS2 = 604.-2165.; //check
  Float_t dist_TPC23_focS2 = 604.-2165.; //check


  //Float_t dist_TPC3_TPC4 = 1782.0- 604.; //old value for old tracking 
  //Float_t dist_TPC3_focS2 = 604.-2165. ; //old value for old tracking  

  //==================================
  //Distance of TPCs at S4
  //==================================
  
  Float_t dist_TPC41_TPC42 = 1525.0 - 385.0;  //rough check
  //Float_t dist_TPC41_TPC42 = 1660.0 - 360.0;  //check

  Float_t dist_TPC42_focS4 = 3300.0 - 1525.0; //check
  //Float_t dist_TPC42_focS4 = 3300.0 - 1660.0; //check

  
  //Float_t dist_TPC5_TPC6 = 2063.5 - 382.5 ; 
  //Float_t dist_TPC6_focS4  = 2200. - 2063.5 ;
  //Float_t dist_TPC6_focS4  = 4300. - 1660. ;//before 13Jun2016 17:10
  //Float_t dist_TPC6_focS4  = 2200. - 1660. ;//for S4 focus 2200 , changed at 13Jun2016 17:10 
  //Float_t dist_TPC6_focS4  = 2200. -1135.9 +300.0 - 1660. +100.; //short focus 11jul2016
  //Float_t dist_TPC6_focS4  = 2202 - 1660.; // tensor28 mode
  //Float_t dist_TPC6_focS4  = 3300 - 1660.; // long focus

  //=====================
  // Old tracking for S2 
  //=====================
  /*
  //Position S2 tracked with TPCs 3 and 4 for focus
  if (b_tpc_xy[2]&&b_tpc_xy[3])
    {
      tpc_angle_x_s2_foc = (tpc_x[3] - tpc_x[2])/dist_TPC3_TPC4*1000.;
      tpc_angle_y_s2_foc = (tpc_y[3] - tpc_y[2])/dist_TPC3_TPC4*1000.;
      tpc_x_s2_foc = -tpc_angle_x_s2_foc * dist_TPC3_focS2/1000. + tpc_x[2];
      tpc_y_s2_foc = -tpc_angle_y_s2_foc * dist_TPC3_focS2/1000. + tpc_y[2];

 
     // Position S2 tracked with TPCs 3 and 4 to first Si tracker in exp S388
     // tpc_x_s2_target1 = tpc_angle_x_s2_foc * dist_TPC4_target1/1000. + tpc_x[3];
     // tpc_y_s2_target1 = tpc_angle_y_s2_foc * dist_TPC4_target1/1000. + tpc_y[3];
      
    }
  else  tpc_x_s2_foc= -999 ;
  */

  //=================================
  // Tracking with TPC 21 and TPC 22 
  //=================================
  
  if (b_tpc_xy[0]&&b_tpc_xy[1])
    {
      tpc_angle_x_s2_foc_21_22 = (tpc_x[1] - tpc_x[0])/dist_TPC21_TPC22*1000.;
      tpc_angle_y_s2_foc_21_22 = (tpc_y[1] - tpc_y[0])/dist_TPC21_TPC22*1000.;
      tpc_x_s2_foc_21_22 = -tpc_angle_x_s2_foc_21_22 * dist_TPC21_focS2/1000. + tpc_x[0]; //check
      tpc_y_s2_foc_21_22 = -tpc_angle_y_s2_foc_21_22 * dist_TPC21_focS2/1000. + tpc_y[0]; //check
      
    }
  else  tpc_x_s2_foc_21_22= -999 ;


  //=================================
  // Tracking with TPC 23 and TPC 24 
  //=================================
  
  if (b_tpc_xy[2]&&b_tpc_xy[3])
    {
      tpc_angle_x_s2_foc_23_24 = (tpc_x[3] - tpc_x[2])/dist_TPC23_TPC24*1000.;
      tpc_angle_y_s2_foc_23_24 = (tpc_y[3] - tpc_y[2])/dist_TPC23_TPC24*1000.;
      tpc_x_s2_foc_23_24 = -tpc_angle_x_s2_foc_23_24 * dist_TPC23_focS2/1000. + tpc_x[2]; //check
      tpc_y_s2_foc_23_24 = -tpc_angle_y_s2_foc_23_24 * dist_TPC23_focS2/1000. + tpc_y[2]; //check

    }
  else  tpc_x_s2_foc_23_24= -999 ;


  //=================================
  // Tracking with TPC 22 and TPC 24 
  //=================================
  
  if (b_tpc_xy[1]&&b_tpc_xy[3])
    {
      tpc_angle_x_s2_foc_22_24 = (tpc_x[3] - tpc_x[1])/dist_TPC22_TPC24*1000.;
      tpc_angle_y_s2_foc_22_24 = (tpc_y[3] - tpc_y[1])/dist_TPC22_TPC24*1000.;
      tpc_x_s2_foc_22_24 = -tpc_angle_x_s2_foc_22_24 * dist_TPC22_focS2/1000. + tpc_x[1]; //check
      tpc_y_s2_foc_22_24 = -tpc_angle_y_s2_foc_22_24 * dist_TPC22_focS2/1000. + tpc_y[1]; //check
      
    }
  else  tpc_x_s2_foc_22_24= -999 ;
  
  
  //=====================================================
  // Tracking with TPC 41 and TPC 42 (TPC 5 and 6) at S4  
  //=====================================================
  if (b_tpc_xy[4]&&b_tpc_xy[5])
    {
      
      tpc_angle_x_s4 = (tpc_x[5] - tpc_x[4])/dist_TPC41_TPC42*1000.;
      tpc_angle_y_s4 = (tpc_y[5] - tpc_y[4])/dist_TPC41_TPC42*1000.;
      tpc_x_s4 = tpc_angle_x_s4 * dist_TPC42_focS4/1000. + tpc_x[5];
      tpc_y_s4 = tpc_angle_y_s4 * dist_TPC42_focS4/1000. + tpc_y[5];
      
      music1_x1 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa1)/1000. + tpc_x[5];
      music1_x2 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa2)/1000. + tpc_x[5];
      music1_x3 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa3)/1000. + tpc_x[5];
      music1_x4 = tpc_angle_x_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa4)/1000. + tpc_x[5];

      music1_y1 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa1)/1000. + tpc_y[5];
      music1_y2 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa2)/1000. + tpc_y[5];
      music1_y3 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa3)/1000. + tpc_y[5];
      music1_y4 = tpc_angle_y_s4 * (frs->dist_MUSIC3+frs->dist_MUSICa4)/1000. + tpc_y[5];

      
      //S4 entrance of OTPC tracked with TPCs 5 and 6 for s388    
      //tpc_x_s4_target2 = tpc_angle_x_s4 * dist_TPC6_target2/1000. + tpc_x[5];
      //tpc_y_s4_target2 = tpc_angle_y_s4 * dist_TPC6_target2/1000. + tpc_y[5];
    
    }
  
  
  // tracking to musics and scis (S323,410 exp.)
  //Float_t dist_TPC3_SC21  = 1375.5;
  //Float_t dist_TPC6_SC41  = 510.; //8.09.11 //check AEv

  // X position at SC21 position:
  // SC21 is before TPC3 and TPC4
  //tpc_sc21_x = tpc_x[2] - dist_TPC3_SC21 * tpc_angle_x_s2_foc/1000.;
 

  // Y position at SC21
  //tpc_sc21_y = tpc_y[2] - dist_TPC3_SC21 * tpc_angle_y_s2_foc/1000.;

  
  // X position at SC41 
  // SC41 is after TPC6
  //tpc_sc41_x  = tpc_x[5] + dist_TPC6_SC41 * tpc_angle_x_s4 / 1000.;
  
  // Y position at SC41
  //tpc_sc41_y = tpc_y[5] + dist_TPC6_SC41 * tpc_angle_y_s4 /1000.;


//////////////////////////////////////////////////////////////////
///  ***********Si Analysis**************************** //////////


  // Si alpha
  si_e1 = (si->si_factor1*si_adc1)+si->si_offset1;
  si_e2 = (si->si_factor2*si_adc2)+si->si_offset2;
  si_e3 = (si->si_factor3*si_adc3)+si->si_offset3;
  si_e4 = (si->si_factor4*si_adc4)+si->si_offset4;
  si_e5 = (si->si_factor5*si_adc5)+si->si_offset5;


  //========================
  //Active Stopper Analysis
  //========================
  
  dssd_xmaxindex=-10;
  dssd_xmaxenergy=1000;

  
  //for(int i=0;i<16;i++){
  //dssd_e[15-i] = (si->dssd_factor[15-i]*(dssd_adc[i]+gRandom->Rndm()-0.5))+si->dssd_offset[15-i];	  	  }
  

  for(int i=0;i<16;i++){
   //dssd_e[i] = dssd_adc[i];
     dssd_e[i] = (si->dssd_factor[i]*dssd_adc[i])+si->dssd_offset[i];
  }


  for (int i=0 ; i<16 ; i++)
      {
        if (dssd_xmaxenergy<dssd_e[i])
           {
	     dssd_xmaxenergy=dssd_e[i];
	     dssd_xmaxindex= i;
	   }
      }
  
	
  dssd_ymaxindex=-10;
  dssd_ymaxenergy=1000;
  
  
  //for(int i=16;i<24;i++){
  //dssd_e[i+8] = (si->dssd_factor[i+8]*(dssd_adc[i]+gRandom->Rndm()-0.5))+si->dssd_offset[i+8];
  //}
	
  //for(int i=24;i<32;i++){
  //dssd_e[i-8] = (si->dssd_factor[i-8]*(dssd_adc[i]+gRandom->Rndm()-0.5))+si->dssd_offset[i-8];
  //}	
   

   for(int i=16;i<32;i++){
      // dssd_e[i] = dssd_adc[i];
	 dssd_e[i] = (si->dssd_factor[i]*dssd_adc[i])+si->dssd_offset[i];
   }


   for (int i=16 ; i<32 ; i++){
     if(dssd_ymaxenergy<dssd_e[i])
       {
	 dssd_ymaxenergy=dssd_e[i];
	 dssd_ymaxindex= i-16;
       }	

   }
    
    
}



void BS_Detector_System::FRS_Anal(){
    
  music1_anodes_cnt = 0;  
  music2_anodes_cnt = 0;  
  music3_anodes_cnt = 0;


   // Munich MUSIC 
  
  for (int i=0;i<8;i++)
    {
      /* 8 anodes of TUM MUSIC */		
      /****** first MUSIC ***** threshold changed to 5, 9/8/2012 **/
      if ( music_e1[i] > 5)
	{ 

	  music_b_e1[i] = Check_WinCond_Multi(music_e1[i], cMusic1_E, i);//cMusic1_E[i]->Test(music_e1[i]);
	  

	  if (music_b_e1[i])
	    music1_anodes_cnt++;
	}

        if (music_t1[i] > 0)
	{ 
	  music_b_t1[i] = Check_WinCond_Multi(music_t1[i], cMusic1_T, i); // cMusic1_T[i]->Test(music_t1[i]);
	}

       
      //hMUSIC1_dE1dE2->Fill(music_e1[0],music_e1[1]);

      //       /****** second MUSIC *****/
      
      if ( music_e2[i] > 5)
	{ 
	  music_b_e2[i] = Check_WinCond_Multi(music_e2[i], cMusic2_E, i);// cMusic2_E[i]->Test(music_e2[i]);
	  if (music_b_e2[i]) music2_anodes_cnt++;
	}

      if (music_t2[i] > 0)
	{ 
	  music_b_t2[i] = Check_WinCond_Multi(music_t2[i], cMusic2_T, i);// cMusic2_T[i]->Test(music_t2[i]);
	}
    }

  for (int i=0;i<4;i++)
    {
      /* 4 anodes of MUSIC OLD */		
      /****** third MUSIC *****/
      if ( music_e3[i] > 0)
	{ 

	  music_b_e3[i] = Check_WinCond_Multi(music_e3[i], cMusic3_E, i);// cMusic3_E[i]->Test(music_e3[i]);
	  if (music_b_e3[i])
	    music3_anodes_cnt++;
	}
      if (music_t3[i] > 0)
	{ 
	  music_b_t3[i] = Check_WinCond_Multi(music_t3[i], cMusic3_T, i);// cMusic3_T[i]->Test(music_t3[i]);
	}
    }
  
  // calculate truncated dE from 8 anodes, Munich MUSIC 
  
  if (music1_anodes_cnt == 8)
    {
      /* "quick-n-dirty solution according to Klaus:   */
      //      Float_t r1 = (music_e1[0] - music->e1_off[0])*(music_e1[1] - music->e1_off[1]);
      //      Float_t r2 = (music_e1[2] - music->e1_off[2])*(music_e1[3] - music->e1_off[3]);
      //      Float_t r3 = (music_e1[4] - music->e1_off[4])*(music_e1[5] - music->e1_off[5]);
      //      Float_t r4 = (music_e1[6] - music->e1_off[6])*(music_e1[7] - music->e1_off[7]);

      Float_t r1 = ((music_e1[0])*music->e1_gain[0] + music->e1_off[0])*((music_e1[1])*music->e1_gain[1] + music->e1_off[1]);
      Float_t r2 = ((music_e1[2])*music->e1_gain[2] + music->e1_off[2])*((music_e1[3])*music->e1_gain[3] + music->e1_off[3]);
      Float_t r3 = ((music_e1[4])*music->e1_gain[4] + music->e1_off[4])*((music_e1[5])*music->e1_gain[5] + music->e1_off[5]);
      Float_t r4 = ((music_e1[6])*music->e1_gain[6] + music->e1_off[6])*((music_e1[7])*music->e1_gain[7] + music->e1_off[7]);

      if ( (r1 > 0) && (r2 > 0) && (r3 > 0) && (r4 > 0) )
	{
	  b_de1 = kTRUE;
	  de[0] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );
	  de_cor[0] = de[0];
	}  
    }

       
  if (music2_anodes_cnt == 8)
    {
      
      Float_t r1 = ((music_e2[0])*music->e2_gain[0] + music->e2_off[0])*((music_e2[1])*music->e2_gain[1] + music->e2_off[1]);
      Float_t r2 = ((music_e2[2])*music->e2_gain[2] + music->e2_off[2])*((music_e2[3])*music->e2_gain[3] + music->e2_off[3]);
      Float_t r3 = ((music_e2[4])*music->e2_gain[4] + music->e2_off[4])*((music_e2[5])*music->e2_gain[5] + music->e2_off[5]);
      Float_t r4 = ((music_e2[6])*music->e2_gain[6] + music->e2_off[6])*((music_e2[7])*music->e2_gain[7] + music->e2_off[7]);

      if ( (r1 > 0) && (r2 > 0) && (r3 > 0) && (r4 > 0) )
	{
	  b_de2 = kTRUE;
	  de[1] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );
	  de_cor[1] = de[1];
	}  
    }
       

  if (music3_anodes_cnt == 4)
    {         // OLD MUSIC
      
      Float_t r1 = ((music_e3[0])*music->e3_gain[0] + music->e3_off[0])*((music_e3[1])*music->e3_gain[1] + music->e3_off[1]);
      Float_t r2 = ((music_e3[2])*music->e3_gain[2] + music->e3_off[2])*((music_e3[3])*music->e3_gain[3] + music->e3_off[3]);
      //Float_t r3 = ((music_e3[4])*music->e3_gain[4] + music->e3_off[4])*((music_e3[5])*music->e3_gain[5] + music->e3_off[5]);  //added on 22.05.2018 SB
      //Float_t r4 = ((music_e3[6])*music->e3_gain[6] + music->e3_off[6])*((music_e3[7])*music->e3_gain[7] + music->e3_off[7]);  //added on 22.05.2018 SB
      
      if ( (r1 > 0) && (r2 > 0) )
	{
	  b_de3 = kTRUE;
	  de[2] = sqrt( sqrt(r1) * sqrt(r2) ) ;  // corrrected JSW 19.03.2010
	  //de[2] = sqrt( sqrt( sqrt(r1) * sqrt(r2) ) * sqrt( sqrt(r3) * sqrt(r4) ) );   //changed on 22.05.2018 SB
	  de_cor[2] = de[2];
	} 

      if (music_b_t3[0] && music_b_t3[1] && music_b_t3[2] && music_b_t3[3]) 
	b_dt3 = kTRUE;
    


	//hMUSIC1_MUSIC2->Fill(de[0],de[1]);

      /* Position (X) correction by TPC */       //TO DO!!!
    


      //if(!music->b_selfcorr1 && b_de3) {
      //if(b_mw_xsum[4] && b_mw_xsum[5] && b_de3) {
      if(b_de3 && b_tpc_xy[4]&&b_tpc_xy[5])
	{
	  Float_t p1 = music1_x1;
	  Float_t p2 = music1_x2;
	  Float_t p3 = music1_x3;
	  Float_t p4 = music1_x4;

	  //Float_t p5 = music1_x5; //added on 22.05.2018 SB
	  //Float_t p6 = music1_x6; //added on 22.05.2018 SB
	  //Float_t p7 = music1_x7; //added on 22.05.2018 SB
	  //Float_t p8 = music1_x8; //added on 22.05.2018 SB
	  
	  x1_mean = (p1+p2+p3+p4)/4.;	// Mean position
	  //x1_mean = (p1+p2+p3+p4+p5+p6+p7+p8)/8.;	// Mean position  //added on 22.05.2018 SB
 
	  
	  Float_t power = 1., Corr = 0.;
	  for(int i=0;i<4;i++) {
	    Corr += music->pos_a1[i] * power;
	    power *= x1_mean;  
	  }
      
	  if (Corr!=0) {
	    Corr = music->pos_a1[0] / Corr;
	    de_cor[2] = de[2] * Corr;
	  }
	  
	}
	//}
      

      /* Special gate on corrected music for cleaning x2 vs. AoQ spectrum */
      b_decor = Check_WinCond(de_cor[2], cMusic3_dec);// cMusic3_dec->Test(de_cor[2]);
      
    
    }


        /*-------------------------------------------------------------------------*/
 	/* focus index: detector number                  tof index  tof path       */
 	/*       0:     Sc01                                0:     TA - S1         */
 	/*       1:     Sc11                                1:     S1 - S2         */
 	/*       2:     Sc21                                2:     S2 - S41        */
 	/*       3:     Sc21                                3:     S2 - S42        */
 	/*       4:     Sc31                                4:     S2 - 81         */
 	/*       5:     Sc41                                5:     S2 - E1         */
	/*                                                                         */
 	/*       6:     Sc42                              tof index not used up to */
 	/*       7:     Sc43 (previously Sc51)             now, only separate      */
 	/*       8:     Sc61                              variables for S2-S41 and */
 	/*       9:     ScE1 (ESR)                                S2-S42           */
 	/*      10:     Sc81                                                       */
 	/*      11:     Sc82                                                       */
 	/*-------------------------------------------------------------------------*/
    
    

  /*  Raw data  */
   sci_l[2] = de_21l;  /* 21L         */
   sci_r[2] = de_21r;  /* 21R         */
   sci_tx[2] = dt_21l_21r + rand0_5();

   sci_l[5] = de_41l;  /* 41L         */
   sci_r[5] = de_41r;  /* 41R         */
   sci_tx[5] = dt_41l_41r + rand0_5();
  
   sci_l[6] = de_42l;  /* 42L         */
   sci_r[6] = de_42r;  /* 42R         */
   sci_tx[6] = dt_42l_42r + rand0_5();

   sci_l[7] = de_43l;  /* 43L         */
   sci_r[7] = de_43r;  /* 43R         */
   sci_tx[7] = dt_43l_43r + rand0_5();

   sci_l[10] = de_81l; /* 81L         */
   sci_r[10] = de_81r; /* 81R         */
   sci_tx[10] = dt_81l_81r + rand0_5();
   

   for (int cnt=0;cnt<6;cnt++) // 
     {
       int idx = 0 ;
       //int mw_idx = 0;
       //Float_t mwx = 0;
       switch(cnt)
	 {
	 case 0:        /* SC21 */
	   idx = 2; 
	   //mw_idx = 2;
	   //mwx = sc21_x;
	   break;    
	 case 1:        /* SC21 delayed */
	   idx = 3; 
	   //mw_idx = 2;
	   //mwx = sc21_x;
	   break;    
	 case 2:        /* SC41 */
	   idx = 5; 
	   //mw_idx = 5;
	   //mwx = tpc_sc41_x;
	   break;    
	 case 3:        /* SC42 */
           idx = 6;
	   break;
	 case 4:
	   idx = 7;     /* SC43 */
	   break;
	 case 5:
	   idx = 10;    /* SC81 */
	   break;
	 default: idx = 2;
	 }   
     
       // raw spectra 
       sci_b_l[idx] = Check_WinCond(sci_l[idx], cSCI_L);// cSCI_L[idx]->Test(sci_l[idx]);
       sci_b_r[idx] = Check_WinCond(sci_r[idx], cSCI_R);// cSCI_R[idx]->Test(sci_r[idx]);

       if(sci_b_l[idx] && sci_b_r[idx])
	 {     
	   sci_e[idx] = sqrt( (sci_l[idx] - sci->le_a[0][idx]) * sci->le_a[1][idx] 
				  * (sci_r[idx] - sci->re_a[0][idx]) * sci->re_a[1][idx]);
	   
	   sci_b_e[idx] = Check_WinCond(sci_e[idx], cSCI_E);// cSCI_E[idx]->Test(sci_e[idx]);
	 }


       /*   Position in X direction:   */
       sci_b_tx[idx] = Check_WinCond(sci_tx[idx], cSCI_Tx);// cSCI_Tx[idx]->Test(sci_tx[idx]);
       if (sci_b_tx[idx])
	 {

	   /* mm-calibrated     */
	   Float_t R = sci_tx[idx] ;//+ rand0_5(); 
	   
	   Float_t power = 1., sum = 0.;
	   for(int i=0;i<7;i++)
	     {
	       sum += sci->x_a[i][idx] * power;
	       power *= R;  
	     }
	   
	   sci_x[idx] = sum;
	   
	   sci_b_x[idx] = Check_WinCond(sci_x[idx], cSCI_X);// cSCI_X[idx]->Test(sci_x[idx]);
	 }

     } // end of loop for indices 2,3,5,6,7,10   


   /***  Scintillator Tof  spectra ***/

   /* S21 - S41 */ 

   /*  Calibrated tof  */
   sci_tofll2 = dt_21l_41l*sci->tac_factor[2] - sci->tac_off[2] ;   /* S41L- S21L */
   sci_tofrr2 = dt_21r_41r*sci->tac_factor[3] - sci->tac_off[3] ;   /* S41R- S21R */
   //        sci_tofll2 = dt_21l_41l;  
   //        sci_tofrr2 = dt_21r_41r;
   //     std::cout <<" tac_factor2 "<<sci->tac_factor[2]<<std::endl; 
   //     std::cout <<" tac_factor3 "<<sci->tac_factor[3]<<std::endl;  
  
   sci_b_tofll2 = Check_WinCond(sci_tofll2, cSCI_LL2);// cSCI_TofLL2->Test(sci_tofll2);
   sci_b_tofrr2 = Check_WinCond(sci_tofrr2, cSCI_RR2);// cSCI_TofRR2->Test(sci_tofrr2);

    /*cout<<"VALUE !!!"<<sci_tofll2<<endl;
    cout<<"WINDOW !!!"<<cSCI_LL2[0]<<"   "<<cSCI_LL2[1]<<endl;
    cout<<"BREAK!"<<endl;
    cout<<"VALUE !!!"<<sci_tofrr2<<endl;
    cout<<"WINDOW !!!"<<cSCI_RR2[0]<<"   "<<cSCI_RR2[1]<<endl;
    cout<<"PAY ATTENTION !!!"<<sci_b_tofll2<<"   "<<sci_b_tofrr2<<endl;*/

   /* sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      */
   if (sci_b_tofll2 && sci_b_tofrr2)
     {      /* TOF SC41 - SC21 [ps]  */
       sci_tof2 =  (sci->tof_bll2 * sci_tofll2 + sci->tof_a2 
			+ sci->tof_brr2 * sci_tofrr2)/2.0 ;
			
			//cout<<"WE MADE IT !!!"<<sci_tof2<<endl;
      
     }
 
   /*  
    else if (sci_b_tofll2) {
    sci_tof2 =  (sci->tof_bll2 * sci_tofll2 + sci->tof_a2) ;
    
    hSCI_Tof2->Fill(sci_tof2);
    }
    else if (sci_b_tofrr2) {
    sci_tof2 =  (sci->tof_a2 + sci->tof_brr2 * sci_tofrr2);
    
    hSCI_Tof2->Fill(sci_tof2);
    }
   */


   /***  Scintillator Tof  spectra with SC21-SC42 (as a backup) ***/ //added on 03.07.2018 SB

   //  Calibrated tof  
   sci_tofll3 = dt_42l_21l*sci->tac_factor[5] - sci->tac_off[5] ;   // S42L- S21L 
   sci_tofrr3 = dt_42r_21r*sci->tac_factor[6] - sci->tac_off[6] ;   // S42R- S21R
   //     std::cout <<" tac_factor5 "<<sci->tac_factor[5]<<std::endl; 
   //     std::cout <<" tac_factor6 "<<sci->tac_factor[6]<<std::endl;  
  
   sci_b_tofll3 = Check_WinCond(sci_tofll3, cSCI_LL3);// cSCI_TofLL3->Test(sci_tofll3);
   sci_b_tofrr3 = Check_WinCond(sci_tofrr3, cSCI_RR3);// cSCI_TofRR3->Test(sci_tofrr3);

   // sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      
   if (sci_b_tofll3 && sci_b_tofrr3)
     {      // TOF SC42 - SC21 [ps]
       sci_tof3 =  (sci->tof_bll3 * sci_tofll3 + sci->tof_a3 
			+ sci->tof_brr3 * sci_tofrr3)/2.0 ;
      
     }


   /***  Scintillator Tof  spectra with SC21-SC81 ***/ //added on 03.07.2018 SB

   //  Calibrated tof  
   sci_tofll4 = dt_21l_81l*sci->tac_factor[9] - sci->tac_off[9] ;   // S81L- S21L 
   sci_tofrr4 = dt_21r_81r*sci->tac_factor[10] - sci->tac_off[10] ;   // S81R- S21R
  
   sci_b_tofll4 = Check_WinCond(sci_tofll4, cSCI_LL4);// cSCI_TofLL4->Test(sci_tofll4);
   sci_b_tofrr4 = Check_WinCond(sci_tofrr4, cSCI_RR4);// cSCI_TofRR4->Test(sci_tofrr4);

   // sum of Tof_LL and Tof_RR corrects for position in stop/start scint.      
   if (sci_b_tofll4 && sci_b_tofrr4)
     {      // TOF SC81 - SC21 [ps]
       sci_tof4 =  (sci->tof_bll4 * sci_tofll4 + sci->tof_a4 
			+ sci->tof_brr4 * sci_tofrr4)/2.0 ;
      
     }
   

   /* check for polygon in raw detof spectrum of SC41 */
   
   sci_b_detof = Check_PolyCond_X_Y(sci_tof2, sci_e[5], cSCI_detof, 4); // cSCI_detof->Test(sci_tof2, sci_e[5]);
 
  id_trigger=trigger;
  /* accumulate raw detof spectrum  */

  // sci_e[idx] = sqrt( (sci_l[idx] - sci->le_a[0][idx]) * sci->le_a[1][idx] 
  //                   * (sci_r[idx] - sci->re_a[0][idx]) * sci->re_a[1][idx]);
  
  
  /* check for polygon in raw detof spectrum  */
  //  id_b_detof2 = cID_dEToF->Test(sci_tof2, de[0]);
    
  /*  select by what means S2 positions are to be derived:         */
  /*  ID.X2_select =0: SC21;  =1: S2 MWs                           */
  id_x2 = -9999;
  if(id->x2_select == 0) 
    {
      if(sci_b_x[2])
	id_x2 = sci_x[2]; /* pos index 2 = SC21 */
    }

  /* For old tracking with old nomenclature (06.06.18)
  if(id->x2_select == 1)
    {
     
      id_x2=999;
      if(b_tpc_xy[2] && b_tpc_xy[3])
	id_x2 = tpc_x_s2_foc;
      if(b_tpc_xy[2] && !b_tpc_xy[3])
	id_x2 = tpc_x[2];
      if(!b_tpc_xy[2] && b_tpc_xy[3])
	id_x2 = tpc_x[3];
      if(!b_tpc_xy[2] && !b_tpc_xy[3] && sci_b_x[2])
	id_x2 = sci_x[2];
     

     id_a2 = tpc_angle_x_s2_foc;
     id_y2 = tpc_y_s2_foc;
     id_b2 = tpc_angle_y_s2_foc;
  }
  */

   if(id->x2_select == 1)
    {
     
      id_x2=999;
      id_y2=999;
      id_a2=999;
      id_b2=999;
      
      if(b_tpc_xy[0] && b_tpc_xy[1] && !b_tpc_xy[2] && !b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_21_22;
        id_y2 = tpc_y_s2_foc_21_22;
	id_a2 = tpc_angle_x_s2_foc_21_22;
	id_b2 = tpc_angle_y_s2_foc_21_22;
	}

      if(!b_tpc_xy[0] && !b_tpc_xy[1] && b_tpc_xy[2] && b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_23_24;
        id_y2 = tpc_y_s2_foc_23_24;
	id_a2 = tpc_angle_x_s2_foc_23_24;
	id_b2 = tpc_angle_y_s2_foc_23_24;
	}

      if(!b_tpc_xy[0] && b_tpc_xy[1] && !b_tpc_xy[2] && b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_22_24;
        id_y2 = tpc_y_s2_foc_22_24;
	id_a2 = tpc_angle_x_s2_foc_22_24;
	id_b2 = tpc_angle_y_s2_foc_22_24;
	}

       if(b_tpc_xy[0] && b_tpc_xy[1] && b_tpc_xy[2] && b_tpc_xy[3] )
	{
	id_x2 = tpc_x_s2_foc_22_24;
        id_y2 = tpc_y_s2_foc_22_24;
	id_a2 = tpc_angle_x_s2_foc_22_24;
	id_b2 = tpc_angle_y_s2_foc_22_24;
	}

        if(!b_tpc_xy[0] && !b_tpc_xy[1] && !b_tpc_xy[2] && !b_tpc_xy[3] && sci_b_x[2] )
	{
	id_x2 = sci_x[2];
        id_y2 = 0.0;
	id_a2 = 0.0;
	id_b2 = 0.0;
	}
	
  }
                            
  /*  select by what means S4 positions are to be derived:         */
  /*  ID.X4_select =0: SC41;  =1: S4 MWs;                          */
  if(id->x4_select == 0) 
    {
      //    if(sci_b_x[5]) id_x4 = sci_x[5];  /* pos index 5 = SC41 */
      id_x4 = 0;  /* pos index 5 = SC41 */
    }
  
  if(id->x4_select == 1)
    {
      id_x4 = tpc_x_s4;
      id_a4 = tpc_angle_x_s4;
      id_y4 = tpc_y_s4;
      id_b4 = tpc_angle_y_s4;
    }

  
  /*  check that the positions are OK   */
  id_b_x2 = Check_WinCond(id_x2, cID_x2);// cID_x2->Test(id_x2);
  id_b_x4 = Check_WinCond(id_x4, cID_x4);// cID_x4->Test(id_x4);

  /*
  /// conditions for the 12C measurement

  // for 12N at S4
  if(de[0]>2200. && de[0]<4000.)hID_x4z55->Fill(id_x4);// added 2016Jun.16
  // for 12N at S2
   if(de[0]>2200. && de[0]<4000.)hID_x2z55->Fill(id_x2);// added 2016Jun.16

   //for 12B at s4 and s2
  if(de[0]>900. && de[0]<1200. )hID_x4z53->Fill(id_x4);
  if(de[0]>900. && de[0]<1200. )hID_x2z53->Fill(id_x2);
  */

  // This ones are for all nuclei as a funtion of the energy loss

  //hID_E_Xs4->Fill(id_x4,de[0]);// added by 2016Jun.16

  //hID_E_Xs2->Fill(id_x2,de[0]);// added by 2016Jun.16

  /*hID_x2a2->Fill(id_x2,id_a2);
  hID_y2b2->Fill(id_y2,id_b2);
  hID_x4a4->Fill(id_x4,id_a4);
  hID_y4b4->Fill(id_y4,id_b4); */


  /****  A/Q calculations for S41-S21 or S42-S41 (tof index 2+3) *************/ 
  Float_t f = 931.4940 / 299.792458 ;    /* the u/(c*10^-6) factor  */

  /*----------------------------------------------------------*/
  /* Determination of beta                                    */
  /* ID.TofOff(i)                   Flight time offset [ps]   */
  /* ID.Path(i)                     Flight path/c [ps]        */
  /* TOF(i)        BIN FLOAT(24),   Flight time  [ps]         */
  /*----------------------------------------------------------*/
  /* from ToF S41-S21 */
  if (sci_b_tofll2 && sci_b_tofrr2)
    {
      id_beta = id->id_path2 / (id->id_tofoff2 - sci_tof2);
    }
  
  /*------------------------------------------------------*/
  /* Determination of Brho                                */
  /* Dispersion and magnification are still the same      */
  /* variable for S41-S21 and S42-S41, adjust in setup.C  */
  /*------------------------------------------------------*/

  /* check S2 valid conditions */
  //if (id_b_x2 && id_b_x4) {
  if (id_b_x2)
    {   
      // first half of FRS
      id_rho[0] = frs->rho0[0] * (1. - id_x2/1000./frs->dispersion[0]);   
      // second half of FRS
      id_rho[1] = frs->rho0[1] * (1. - (id_x4 - frs->magnification[1] * id_x2) / 1000. / frs->dispersion[1]) ; 
      //    id_rho[1] = frs->rho0[1] * (1. - (frs->magnification[1] * id_x2) / 1000. / frs->dispersion[1]) ; 
      
      for(int i=0;i<2;i++)
	{
	  id_brho[i] = (fabs(frs->bfield[2*i]) + fabs(frs->bfield[2*i+1]))/ 2. * id_rho[i];
	}
    }   
  //}


  /*--------------------------------------------------------------*/
  /* Determination of A/Q                                         */
  /*--------------------------------------------------------------*/
  /* Beta(i)       BIN FLOAT(24),   Beta = v/c                    */
  /* Gamma(i)      BIN FLOAT(24),   Gamma= sqrt(1/1-beta**2)      */
  /*--------------------------------------------------------------*/
  /* for S2-S4 */
  // if (sci_b_tofll2 && sci_b_tofrr2 && id_b_x2 && id_b_x4) {
  if (sci_b_tofll2 && sci_b_tofrr2 &&  id_b_x2)
    {
      if ((id_beta>0.0) && (id_beta<1.0))
	{
	  id_gamma = sqrt( 1. /(1. - id_beta * id_beta));
	  //id_AoQ = id_brho[1]/id_beta/id_gamma/ f - id->id_tofcorr2 * id_x4;
	  id_AoQ = id_brho[1]/id_beta/id_gamma/ f ;
	  
	  //      std::cout <<" id_AoQ "<<id_AoQ<<std::endl; 
	  
	  //correction for id_a2, JK 16.9.11


	  id_AoQ_corr = id_AoQ - frs->a2AoQCorr * id_a2;
	  if (!b_tpc_xy[4] || !b_tpc_xy[5])
	    id_AoQ_corr = id_AoQ - frs->a4AoQCorr * id_a4;


	  id_b_AoQ = kTRUE;
	}
    }

  if(id_b_AoQ)
    {
    }
  
  // }

  /*------------------------------------------------*/
  /* Determination of Z                             */
  /*------------------------------------------------*/
  /****  S4  (MUSIC 1)   */
  //  if((de_cor[0]>0.0) && (id_beta>0.0) && (id_beta<1.0)) {
  
  if((de[0]>0.0) && (id_beta>0.0) && (id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a[i];
	  power *= id_beta;
	}
      id_v_cor = sum;
      
      if (id_v_cor > 0.0)
	id_z = frs->primary_z * sqrt(de[0]/id_v_cor) + frs->offset_z;
      
    if ((id_z>0.0) && (id_z<100.0))
      {
	id_b_z = kTRUE;
      }
    }

  /****  S4  (MUSIC 2)   */
  
  //  if((de_cor[0]>0.0) && (id_beta>0.0) && (id_beta<1.0)) {
  if((de[1]>0.0) && (id_beta>0.0) && (id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a2[i];
	  power *= id_beta;
	}
      id_v_cor2 = sum;
      
      if (id_v_cor2 > 0.0)
	id_z2 = frs->primary_z * sqrt(de[1]/id_v_cor2) + frs->offset_z2;
      //std::cout<<"id_z2="<<id_z2<<std::endl;
      
      if ((id_z2>0.0) && (id_z2<100.0))
	{
	  id_b_z2 = kTRUE;
	}
    }
 

  /****  S4  (MUSIC OLD)   */
  //  if((de_cor[0]>0.0) && (id_beta>0.0) && (id_beta<1.0)) {
  
  if((de[2]>0.0) && (id_beta>0.0) && (id_beta<1.0))
    {
      Double_t power = 1., sum = 0.;
      for (int i=0;i<4;i++)
	{
	  sum += power * id->vel_a3[i];
	  power *= id_beta;
	}
      id_v_cor3 = sum;
      
      if (id_v_cor3 > 0.0)
	id_z3 = frs->primary_z * sqrt(de[2]/id_v_cor3) + frs->offset_z3;
      //std::cout<<"id_z2="<<id_z2<<std::endl;
      
      if ((id_z3>0.0) && (id_z3<100.0))
	{
	  id_b_z3 = kTRUE;
	}


      static const double anode_width = 10.;//cm
      //double music_dX = anode_width*sqrt(id_a4*id_a4+id_b4*id_b4+1.);
      //h_dEdx_betagammaAll->Fill(id_beta*id_gamma,de[2]/music_dX);
      //h_dEdx_betagammaAllZoom->Fill(id_beta*id_gamma,de[2]/music_dX);
      //double music_dEtemp0 = music_e3[0]*music->e3_gain[0] + music->e3_off[0];
      for(int i=0;i<4;++i)
	{
	  //double music_dEtemp = music_e3[i]*music->e3_gain[i] + music->e3_off[i];
	  //h_dEdx_betagamma[i]->Fill(id_beta*id_gamma,music_dEtemp/music_dX);
	  /*if(i>0)
	    h_DiffdEdx_betagamma[i]->Fill(id_beta*id_gamma,(music_dEtemp-music_dEtemp0)/music_dX);*/
	}
    }
  /*------------------------------------------------*/
  /* Identification Plots                           */
  /*------------------------------------------------*/

  /****  for S2-S4  ****/

  //Float_t my_dist_TPC6= 1075; //position TPC6 in s388?
  //Float_t x4env=0;
  //Float_t y4env=0;
  //  Float_t my_dist_TPC65;
  if(id_b_AoQ)
    {
      for (int i=0;i<5;i++)
	{
	
	//Float_t tmp_array[2] = {id_AoQ, id_x4};
	    
	id_b_x4AoQ_Z[i] = Check_PolyCond_X_Y(id_AoQ, id_x4, cID_x4AoQ_Z, 4);// cID_x4AoQ_Z[i]->Test(id_AoQ, id_x4); 
        }
    
	//Float_t tmp_array[2] = {id_AoQ, id_x2};
	
      id_b_x2AoQ = Check_PolyCond_Multi_X_Y(id_AoQ, id_x2, cID_x2AoQ, 5, 0);// cID_x2AoQ[0]->Test(id_AoQ, id_x2);
      for(int i=0;i<6;++i)
	{
	  if(Check_PolyCond_Multi_X_Y(id_AoQ, id_x2, cID_x2AoQ, 5, i)==true)// cID_x2AoQ[i]->Test(id_AoQ, id_x2)
	    {
	      //hMUSIC3_x2AoQ_E[0][i]->Fill(music_e3[0]);
	      //hMUSIC3_x2AoQ_E[1][i]->Fill(music_e3[1]);
	      //hMUSIC3_x2AoQ_E[2][i]->Fill(music_e3[2]);
	      //hMUSIC3_x2AoQ_E[3][i]->Fill(music_e3[3]);	      
	      //hID_x4AoQ_x2AoQgate[i]->Fill(id_AoQ, id_x4);
	      /*if (id_b_z3)
		hID_ZAoQ_x2AoQgate[i]->Fill(id_AoQ, id_z3);*/
	    }
	}
	       
      //      if (id_b_z3) // changed to Music1. YT. 2016Jun.13 19:00
      if (id_b_z) // changed to Music1. YT. 2016Jun.13 19:00   
	{   

	  for(int i=0;i<5;i++)
	    {
	      id_b_music_z[i] = Check_WinCond(id_z, cID_Z_Z);// cID_Z_Z[i]->Test(id_z);   
	      if(cID_Z_AoQ[i]==nullptr)
		std::cout<<"E> cID_Z_AoQ["<<i<<"] nullptr "<<cID_Z_AoQ[i]<<" | "<<id->ID_Z_AoverQ_num[i]<<std::endl;
	      
	      //Float_t tmp_array[2] = {id_AoQ, id_z};
	      
	       id_b_z_AoQ[i] = Check_PolyCond_Multi_X_Y(id_AoQ, id_z, cID_Z_AoQ, 5, i);// cID_Z_AoQ[i]->Test(id_AoQ, id_z);      
	      
		// if(i==0)
		//   { 
		//     if (id_b_z_AoQ[0])
		//       { 
		// 	hMUSIC3_z_AoQ_E[0][i]->Fill(music_e3[0]);
		// 	hMUSIC3_z_AoQ_E[1][i]->Fill(music_e3[1]);
		// 	hMUSIC3_z_AoQ_E[2][i]->Fill(music_e3[2]);
		// 	hMUSIC3_z_AoQ_E[3][i]->Fill(music_e3[3]);
		// 	hID_x4c[0]->Fill(id_x4);
		// 	hID_x2c[0]->Fill(id_x2);
		      
		// 	hMUSIC3_music_z_E[0][i]->Fill(music_e3[0]);
		// 	hMUSIC3_music_z_E[1][i]->Fill(music_e3[1]);
		// 	hMUSIC3_music_z_E[2][i]->Fill(music_e3[2]);
		// 	hMUSIC3_music_z_E[3][i]->Fill(music_e3[3]);
		//       }
		//   }
	      if (id_b_z_AoQ[i])
		  {

			// S395 envelop plots, tracking from position(TPC6) and angle (TPC5, TPC6)
			/*
			  if( id_b_x4 && i==0) {
			  for(Int_t zz=0; zz<=2600; zz+=100) {
			  x4env =  id_a4 * (zz-my_dist_TPC6)/1000. + tpc_x[5];
			  y4env =  id_b4 * (zz-my_dist_TPC6)/1000. + tpc_y[5];
			  //hID_xz->Fill(zz,x4env);
			  //hID_yz->Fill(zz,y4env);
			  hID_xzc->Fill(zz,x4env);
			  hID_yzc->Fill(zz,y4env);
			  }
			  }
			*/
			 
			  
			  //hID_betac[i]->Fill(id_beta);
			  //hID_brhoc[i]->Fill(id_brho[1]);
			
			  // hID_x_target1c[i]->Fill(tpc_x_s2_target1);
			  // hID_x_target2c[i]->Fill(tpc_x_s4_target2);
			  // hID_y_target1c[i]->Fill(tpc_y_s2_target1);
			  // hID_y_target2c[i]->Fill(tpc_y_s4_target2);
			
		      
			// if(i==1){
			//if(coin[10]==1)
			//	hID_x4c[i]->Fill(id_x4);
			//}else{
			//hID_x4c[i]->Fill(id_x4);
			//}
		      //}
		  }
	      }
	}
    }
  
  if(EventFlag==0x200 && trigger==1)
    {
      mrtof_start = mrtof_start*25./1000000.; // ch to microsec
      
      double tempStop1 = mrtof_stopDelay*25./1000000. ;
      double tempStop2 = mrtof_stop*25./1000000. ;
      
      /*h_MRtof_Start->Fill(mrtof_start);
      h_MRtof_StopDelay->Fill(tempStop1);
      h_MRtof_Stop->Fill(tempStop2);*/
      
      int stop_status=0;
      
      if(tempStop1<=0.)
	{
	  if(tempStop2>0.)
	    {
	      mrtof_stop  = tempStop2;
	      //h_MRtof_status->Fill("GoodStop2",1);
	      stop_status=1;
	    }
	  else
	    {
	      mrtof_stop = -9999.;
	      //std::cout<<"E> MRTOF Anal : both stop signal "<<
	      /*if(mrtof_start>1)
		h_MRtof_status->Fill("NoGoodStop",1);
	      else
		h_MRtof_status->Fill("NoData",1);*/
	    }
	}
      else
	{
	  if(tempStop2<=0.)
	    {
	      mrtof_stop = tempStop1-45.;
	      //h_MRtof_status->Fill("GoodStop1",1);
	      stop_status=1;
	    }
	  else
	    {
	      double diff_stop = tempStop1-tempStop2;
	      if(TMath::Abs(diff_stop-45)<1.)
		{
		  mrtof_stop = tempStop1-45.;
		  //h_MRtof_status->Fill("GoodStop1&2",1);
		  stop_status=1;
		}
	      else
		{
		  //h_MRtof_status->Fill("Stop1&2BadOverlap",1);
		  std::cout<<"E> MRtof diff stop too big from 45 microsec !"<<diff_stop<< "sorted : D="<<mrtof_stopDelay <<" "<< mrtof_stop <<" | in microsec: D="<< tempStop1 <<" "<<tempStop2<<"\n";
		}
	    }
	}

      //int diff_stopdelay = 1891482-54830;

      if(stop_status==1)
	{
	  mrtof_tof = 100 - ( mrtof_start - mrtof_stop); // add 100 microsec from mrtof trigger system 
	  //h_MRtof_tof->Fill(mrtof_tof);
	  //h_MRtof_status->Fill("GoodTOF",1);
	}
      /*else
	h_MRtof_status->Fill("BadTOF",1);*/


    }

  mrtof_si_e1=si_e1;
  mrtof_si_e2=si_e2;
  mrtof_si_e3=si_e3;
  mrtof_si_e4=si_e4;
  mrtof_si_e5=si_e5;
    
}



void BS_Detector_System::Setup_Parameters(){

  /*fFRSPar = new TXRSParameter("FRSPar");
  AddParameter(fFRSPar);

  fMWPar = new TMWParameter("MWPar");
  AddParameter(fMWPar);

  fMUSICPar = new TMUSICParameter("MUSICPar");
  AddParameter(fMUSICPar);

  fTPCPar = new TTPCParameter("TPCPar");
  AddParameter(fTPCPar);

  fSCIPar = new TSCIParameter("SCIPar");
  AddParameter(fSCIPar);

  fIDPar = new TIDParameter("IDPar");
  AddParameter(fIDPar);

  fSIPar = new TSIParameter("SIPar");
  AddParameter(fSIPar);

  ModPar = new TModParameter("ModPar");
  AddParameter(ModPar);
  
  MRtofPar = new TMRTOFMSParameter("MRTOFMSPar");
  AddParameter(MRtofPar);*/


  // look up analysis object and all parameters
  
  frs = new TXRSParameter("FRSPar");
  //AddParameter(fFRSPar);

  mw = new TMWParameter("MWPar");
  //AddParameter(fMWPar);

  music = new TMUSICParameter("MUSICPar");
  //AddParameter(fMUSICPar);

  tpc = new TTPCParameter("TPCPar");
  //AddParameter(fTPCPar);

  sci = new TSCIParameter("SCIPar");
  //AddParameter(fSCIPar);

  id = new TIDParameter("IDPar");
  //AddParameter(fIDPar);

  si = new TSIParameter("SIPar");
  //AddParameter(fSIPar);

  ElecMod = new TModParameter("ModPar");
  //AddParameter(ModPar);
  
  mrtof = new TMRTOFMSParameter("MRTOFMSPar");
  //AddParameter(MRtofPar);
  
  //ElecMod = new TModParameter("ModPar");

  
  //TModParameter* ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));

    
  /*TXRSAnalysis* an = dynamic_cast<TXRSAnalysis*> (TGo4Analysis::Instance());
  if (an==0) {
    cout << "!!!  Script should be run in FRS analysis" << endl;
  }
  
  ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));
  if (ElecMod==0) {
    cout << "!!! Parameter ModPar not found " << endl;
  }
  
  
   
  TXRSParameter* frs = dynamic_cast<TXRSParameter*> (an->GetParameter("FRSPar"));
  if (frs==0) {
    cout << "!!!  Parameter FRSPar not found" << endl;
  }

  TMWParameter* mw = dynamic_cast<TMWParameter*> (an->GetParameter("MWPar"));
  if (mw==0) {
    cout << "!!!  Parameter MWPar not found" << endl;
  }

  TMUSICParameter* music = dynamic_cast<TMUSICParameter*> (an->GetParameter("MUSICPar"));
  if (music==0) {
    cout << "!!!  Parameter MUSICPar not found" << endl;
  }

  TSCIParameter* sci = dynamic_cast<TSCIParameter*> (an->GetParameter("SCIPar"));
  if (sci==0) {
    cout << "!!!  Parameter SCIPar not found" << endl;
  }

  TIDParameter* id = dynamic_cast<TIDParameter*> (an->GetParameter("IDPar"));
  if (id==0) {
    cout << "!!!  Parameter IDPar not found" << endl;
  }
   
  TTPCParameter* tpc = dynamic_cast<TTPCParameter*> (an->GetParameter("TPCPar"));
  if (tpc==0) {
    cout << "!!!  Parameter TPCPar not found" << endl;
  }

  TSIParameter* si = dynamic_cast<TSIParameter*> (an->GetParameter("SIPar"));
  if (si==0) {
    cout << "!!!  Parameter SIPar not found" << endl;
  }

  TMRTOFMSParameter* mrtof = dynamic_cast<TMRTOFMSParameter*> (an->GetParameter("MRTOFMSPar"));
  if (mrtof==0) {
    cout << "!!!  Parameter MR-TOF-MSPar not found" << endl;
  }
  
  TModParameter* ElecMod = dynamic_cast<TModParameter*>(an->GetParameter("ModPar"));
   
  cout << endl << "setup script started" << endl;
   

  // setup part 
  an->SetupH2("ID_x4AoQ", 500, 1.3, 2.8, 200, -100, +100, "A/Q", "X4 [mm]");     
  an->SetupH2("ID_Z_AoQ", 600, 1.3, 2.8,600 , 0, 20, "A/Q", "Z"); 
  an->SetupH2("ID_Z_AoQ_corr", 500, 1.3, 2.8, 600, 0, 20, "A/Q (a2 corr)", "Z"); 
  an->SetupH2("ID_x4z", 450, 0., 20.0, 500, -100, 100, "Z", "X4 [mm]"); */

  //      ID_dEToF
  // Float_t my_cID_Z_AoQ_points[5][2] =
  // // s411_57 133I
  // //	{{ 2.5184., 55.8700},
  // //	{ 2.5184., 57.1700},
  // //	{ 2.5305., 57.1700},
  // //	{ 2.5305., 55.7700},
  // //	{ 2.5184., 55.8700}};
  // // s411_57 133Te
  // //	{{ 2.5461., 56.2410},
  // //	{ 2.5461., 55.3040},
  // //	{ 2.5612., 55.3040},
  // //	{ 2.5612., 56.2410},
  // //	{ 2.5461., 56.2410}};
  // // s411_ 213Fr
  //   {{ 2.45205, 86.7875},
  //    { 2.46124, 86.7458},
  //    { 2.46202, 88.1625},
  //    { 2.4496, 88.1417},
  //    { 2.45205, 86.7875}};
  // an->SetupPolyCond("cID_Z_AoQ(0)", 5, my_cID_Z_AoQ_points);


  /*Float_t my_cID_dEToF_points[4][2] = 
    {{    0.,    0.},
     {    0., 4000.},
     {40000., 4000.},
     {40000.,    0.}}; */
  //an->SetupPolyCond("cID_dEToF", 4, my_cID_dEToF_points);
   

  // /* 20Mg */
  // Float_t my_20mg_points[6][2]=
  //   {{1.665,        13.1000},
  //    {1.705,        12.9500},
  //    {1.705,        12.2500},
  //    {1.665,        12.1000},
  //    {1.624,        12.2500},
  //    {1.624,        12.9500}};

  //an->SetupPolyCond("cID_Z_AoQ(3)", 6, my_20mg_points);


  // setup FRS parameter

  //======
  //  MW
  //======

  mw->x_factor[0] = 0.25; // MW11 [mm/ns] 14.09.05 CN+AM 2ns/mm delay line 
  mw->x_factor[1] = 0.25; // MW21
  mw->x_factor[2] = 0.25; // MW22
  mw->x_factor[3] = 0.25; // MW31
  mw->x_factor[4] = 0.25; // MW41 
  mw->x_factor[5] = 0.25; // MW42
  mw->x_factor[8] = 0.25; // MW71
  mw->x_factor[9] = 0.25; // MW81   not modified
  mw->x_factor[10] = 0.125; // MW82 [mm/ns] 11.05.06  CN 4ns/mm delay line

   
  //   mw->x_offset[0] = -0.4; //  MW11  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[0] = 5.0; // Feb 2014
  //   mw->x_offset[1] = 2.0;  //  MW21  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[1] = -2.0; // Feb 2014
  //mw->x_offset[2] = -1.0; //  MW22  17.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[2] = -1.5; // Feb 2014
  mw->x_offset[3] = -0.205; // MW31 //10/05/06, ok also 20.01.09, ok also Feb 2014
  mw->x_offset[4] = 0.;  // MW41
  mw->x_offset[5] = -9.; // MW42 20.01.09 TESTED VALUE WITH SLITS
  mw->x_offset[8] = 1.642; // MW71 //15/05/06
  mw->x_offset[9] = 1.;   // MW81 //11/05/06
  mw->x_offset[10] = -5.; // MW82 //27-MAY-2007

  mw->y_factor[0] = 0.25; // MW11 [mm/ns] 14.09.05 CN+AM 2ns/mm delay line 
  mw->y_factor[1] = 0.25; // MW21
  mw->y_factor[2] = 0.25; // MW22
  mw->y_factor[3] = 0.25; // MW31
  mw->y_factor[4] = 0.25; // MW41
  mw->y_factor[5] = 0.25; // MW42
  mw->y_factor[8] = 0.25; // MW71
  mw->y_factor[9] = 0.25; // MW81
  mw->y_factor[10] = 0.125; // MW82  [mm/ns] 11.05.06  CN 4ns/mm delay line

   
  mw->y_offset[0] = -14.0; // MW11 27-MAY-2007 TESTED VALUE WITH SLITS, ok Feb 2014
  //   mw->y_offset[1] = -5.0;  // MW21 27-MAY-2007 TESTED VALUE WITH SLITS
  mw->y_offset[1] = 21.0;   // Feb 2014
  mw->y_offset[2] = -1.0;  // MW22 27-MAY-2007 TESTED VALUE WITH SLITS, ok Feb 2014
  mw->y_offset[3] = 0.0;   // MW31 18-MAY-2007, ok Feb 2014
  mw->y_offset[4] = 0.;  // MW41
  mw->y_offset[5] = 0.;  // MW42
  mw->y_offset[8] = -2.736;  // MW71 //15/05/06
  mw->y_offset[9] = 3.2;     // MW81 //11/05/06
  mw->y_offset[10] = 0.764;  // MW82 //11/05/06


  mw->gain_tdc[0][0] = 0.302929; //  MW11 Anode (#ch  0 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][0] = 0.303253; //  MW11 XL    (#ch 17 TDC V775a)
  mw->gain_tdc[2][0] = 0.303975; //  MW11 XR    (#ch 16 TDC V775a)
  mw->gain_tdc[3][0] = 0.308414; //  MW11 YU    (#ch 18 TDC V775a)
  mw->gain_tdc[4][0] = 0.309826; //  MW11 YD    (#ch 19 TDC V775a)

  mw->gain_tdc[0][1] = 0.306064; //  MW21 Anode (#ch  1 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][1] = 0.306958; //  MW21 XL    (#ch 21 TDC V775a)
  mw->gain_tdc[2][1] = 0.307799; //  MW21 XR    (#ch 20 TDC V775a)
  mw->gain_tdc[3][1] = 0.297774; //  MW21 YU    (#ch 22 TDC V775a)
  mw->gain_tdc[4][1] = 0.310235; //  MW21 YD    (#ch 23 TDC V775a)

  mw->gain_tdc[0][2] = 0.301179;  // MW22 Anode (#ch  2 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][2] = 0.311121; //  MW22 XL    (#ch 25 TDC V775a)
  mw->gain_tdc[2][2] = 0.303233; //  MW22 XR    (#ch 24 TDC V775a)
  mw->gain_tdc[3][2] = 0.300558; //  MW22 YU    (#ch 26 TDC V775a)
  mw->gain_tdc[4][2] = 0.301105; //  MW22 YD    (#ch 27 TDC V775a)

  mw->gain_tdc[0][3] = 0.304426; //  MW31 Anode (#ch  3 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][3] = 0.312163; //  MW31 XL    (#ch 29 TDC V775a)
  mw->gain_tdc[2][3] = 0.305609; //  MW31 XR    (#ch 28 TDC V775a)
  mw->gain_tdc[3][3] = 0.304716; //  MW31 YU    (#ch 30 TDC V775a)
  mw->gain_tdc[4][3] = 0.293695; //  MW31 YD    (#ch 31 TDC V775a)

  mw->gain_tdc[0][4] = 0.298871; //  MW41 Anode (#ch  4 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][4] = 0.284086; //  MW41 XL    (#ch 1 TDC V775b)
  mw->gain_tdc[2][4] = 0.288656; //  MW41 XR    (#ch 0 TDC V775b)
  mw->gain_tdc[3][4] = 0.286589; //  MW41 YU    (#ch 2 TDC V775b)
  mw->gain_tdc[4][4] = 0.29269;  //  MW41 YD    (#ch 3 TDC V775b)

  mw->gain_tdc[0][5] = 0.297881; //  MW42 Anode (#ch  5 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][5] = 0.287364; //  MW42 XL    (#ch 5 TDC V775b)
  mw->gain_tdc[2][5] = 0.289636; //  MW42 XR    (#ch 4 TDC V775b)
  mw->gain_tdc[3][5] = 0.291135; //  MW42 YU    (#ch 6 TDC V775b)
  mw->gain_tdc[4][5] = 0.289867; //  MW42 YD    (#ch 7 TDC V775b)

  mw->gain_tdc[0][6] = 0.307892; //  MW51 Anode (#ch  6 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][6] = 0.289894; //  MW51 XL    (#ch  9 TDC V775b)
  mw->gain_tdc[2][6] = 0.292366; //  MW51 XR    (#ch  8 TDC V775b)
  mw->gain_tdc[3][6] = 0.284708; //  MW51 YU    (#ch 10 TDC V775b)
  mw->gain_tdc[4][6] = 0.28186;  //  MW51 YD    (#ch 11 TDC V775b)

  mw->gain_tdc[0][7] = 0.298266; //  MW61 Anode (#ch  7 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][7] = 0.311; //  MW61 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][7] = 0.305; //  MW61 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][7] = 0.337; //  MW61 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][7] = 0.289; //  MW61 YD    (#ch ? TDC V775b)

  mw->gain_tdc[0][8] = 0.303602; //  MW71 Anode (#ch  8 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][8] = 0.300082; //  MW71 XL    (#ch 13 TDC V775b)
  mw->gain_tdc[2][8] = 0.286092; //  MW71 XR    (#ch 12 TDC V775b)
  mw->gain_tdc[3][8] = 0.294287; //  MW71 YU    (#ch 14 TDC V775b)
  mw->gain_tdc[4][8] = 0.291341; //  MW71 YD    (#ch 15 TDC V775b)

  mw->gain_tdc[0][9] = 0.306041; //  MW81 Anode (#ch  9 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][9] = 0.288468; //  MW81 XL    (#ch 17 TDC V775b)
  mw->gain_tdc[2][9] = 0.293831; //  MW81 XR    (#ch 16 TDC V775b)
  mw->gain_tdc[3][9] = 0.281296; //  MW81 YU    (#ch 18 TDC V775b)
  mw->gain_tdc[4][9] = 0.279099; //  MW81 YD    (#ch 19 TDC V775b)

  mw->gain_tdc[0][10] = 0.31314;  //  MW82 Anode (#ch 10 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][10] = 0.287279; //  MW82 XL    (#ch 21 TDC V775b)
  mw->gain_tdc[2][10] = 0.284028; //  MW82 XR    (#ch 20 TDC V775b)
  mw->gain_tdc[3][10] = 0.28051;  //  MW82 YU    (#ch 22 TDC V775b)
  mw->gain_tdc[4][10] = 0.28743;  //  MW82 YD    (#ch 23 TDC V775b)

  mw->gain_tdc[0][11] = 0.299973; //  MWB21 Anode (#ch 11 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][11] = 0.311; //  MWB21 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][11] = 0.305; //  MWB21 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][11] = 0.337; //  MWB21 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][11] = 0.289; //  MWB21 YD    (#ch ? TDC V775b)

  mw->gain_tdc[0][12] = 0.306923; //  MWB22 Anode (#ch 12 TDC V775a) // 13.01.2008
  mw->gain_tdc[1][12] = 0.311; //  MWB22 XL    (#ch ? TDC V775b)
  mw->gain_tdc[2][12] = 0.305; //  MWB22 XR    (#ch ? TDC V775b)
  mw->gain_tdc[3][12] = 0.337; //  MWB22 YU    (#ch ? TDC V775b)
  mw->gain_tdc[4][12] = 0.289; //  MWB22 YD    (#ch ? TDC V775b)


  // frs->dist_focS2   = 2280.0; // [mm] used in exp s441_oct12
  // frs->dist_focS2   = 3450.0; // [mm] 8.8.2012 for s388: s271 optics with focus 1170 mm after S2 midplane
  //   frs->dist_focS2   = 1228.0; // [mm] 8.8.2012 for s388 position calculated at S2 X-slits
  frs->dist_focS2   = 2013.; // [mm] at wedge position S411
  //frs->dist_focS2   = 2860.; // [mm] at wedge position S417 for dE

  frs->dist_MW21    =  604.0; // 
  frs->dist_MW22    = 1782.5; // 
  frs->dist_SC21    = 1554.5; // 
  //   frs->dist_MW41    = 1140.0; // ok on air
  //   frs->dist_MW42    = 2600.0; // ok on air

  frs->dist_focS4   = 2349.0; // at SC411   

  //**S323,410 exp.
  frs->dist_MUSIC1  = 555.0;  // TUM1
  frs->dist_MUSIC2  = 1210.0; // TUM2 , now removed
  frs->dist_MUSIC3  = 1210.0; // Music2 to be adjusted
 
  frs->dist_MUSICa1 = 52.5;  // do not change
  frs->dist_MUSICa2 = 157.5; // do not change
  frs->dist_MUSICa3 = 262.5; // do not change
  frs->dist_MUSICa4 = 367.5; // do not change

  //=========
  // MUSICs
  //=========

  //MUSIC41
  music->e1_off[0]   = 0.; //MUSIC41 offsets
  music->e1_off[1]   = 0.; 
  music->e1_off[2]   = 0.;
  music->e1_off[3]   = 0.;
  music->e1_off[4]   = 0.;
  music->e1_off[5]   = 0.;
  music->e1_off[6]   = 0.;
  music->e1_off[7]   = 0.;

  music->e1_gain[0]   = 1.; // MUSIC41 gains
  music->e1_gain[1]   = 1.; 
  music->e1_gain[2]   = 1.;
  music->e1_gain[3]   = 1.;
  music->e1_gain[4]   = 1.;
  music->e1_gain[5]   = 1.;
  music->e1_gain[6]   = 1.;
  music->e1_gain[7]   = 1.;

  //MUSIC42
  music->e2_off[0]   = 0.; //MUSIC42 offsets
  music->e2_off[1]   = 0.; 
  music->e2_off[2]   = 0.;
  music->e2_off[3]   = 0.;
  music->e2_off[4]   = 0.;
  music->e2_off[5]   = 0.;
  music->e2_off[6]   = 0.;
  music->e2_off[7]   = 0.;

  music->e2_gain[0]   = 1.; //MUSIC42 gains
  music->e2_gain[1]   = 1.; 
  music->e2_gain[2]   = 1.;
  music->e2_gain[3]   = 1.;
  music->e2_gain[4]   = 1.;
  music->e2_gain[5]   = 1.;
  music->e2_gain[6]   = 1.;
  music->e2_gain[7]   = 1.;

  //MUSIC43
  music->e3_off[0]   = 0.; //MUSIC3 offsets
  music->e3_off[1]   = 0.; 
  music->e3_off[2]   = 0.;
  music->e3_off[3]   = 0.;
  
  music->e3_gain[0]   = 1.; // MUSIC3 gains
  music->e3_gain[1]   = 1.; 
  music->e3_gain[2]   = 1.;
  music->e3_gain[3]   = 1.;

  music->pos_a1[0]   = 1.0;   // C0...Cn position correction not used
  music->pos_a1[1]   = 0.0;
  music->pos_a1[2]   = 0.0;
  music->pos_a1[3]   = 0.0;
  music->pos_a1[4]   = 0.0;
  music->pos_a1[5]   = 0.0;
  music->pos_a1[6]   = 0.0;


  //=========
  //  TPCs
  //=========
  
  //TPC 1 at S2 (TPC 21) in vaccuum  
  tpc->x_factor[0][0] = 0.070623;
  tpc->x_factor[0][1] = 0.07248;
  tpc->y_factor[0][0] =-0.035723;
  tpc->y_factor[0][1] =-0.034725;
  tpc->y_factor[0][2] =-0.0364399;
  tpc->y_factor[0][3] =-0.035037; 
	 
  tpc->x_offset[0][0] =-0.5;
  tpc->x_offset[0][1] =-1.058;
  tpc->y_offset[0][0] = 38.1838;
  tpc->y_offset[0][1] = 38.37;
  tpc->y_offset[0][2] = 39.1557;
  tpc->y_offset[0][3] = 39.097;
  

  //TPC 2 at S2 (TPC 22) in vaccuum
  tpc->x_factor[1][0] = 0.0716;
  tpc->x_factor[1][1] = 0.070329;
  tpc->y_factor[1][0] = -0.0362752;
  tpc->y_factor[1][1] = -0.0365182;
  tpc->y_factor[1][2] = -0.0357276;
  tpc->y_factor[1][3] = -0.0360721;  

  tpc->x_offset[1][0] = -0.81;
  tpc->x_offset[1][1] =  1.9;
  tpc->y_offset[1][0] =  36.77;
  tpc->y_offset[1][1] =  38.0;
  tpc->y_offset[1][2] =  36.123;
  tpc->y_offset[1][3] =  37.13;
  

  //TPC 3 at S2 (TPC 23) in air 
  tpc->x_factor[2][0] = 0.070128;  //L-R time 0
  tpc->x_factor[2][1] = 0.071489;  //L-R time 1
  tpc->y_factor[2][0] = -0.036481; //drift time 0
  tpc->y_factor[2][1] = -0.036863; //drift time 1
  tpc->y_factor[2][2] = -0.037120; //drift time 2
  tpc->y_factor[2][3] = -0.037125; //drift time 3

  //tpc->x_offset[2][0] = 0.81+3.24-0.14; //21.10.2014
  //tpc->x_offset[2][1] = 0.03+3.24-0.14; //21.10.2014
  //tpc->y_offset[2][0] = 19.2;
  //tpc->y_offset[2][1] = 19.7;
  //tpc->y_offset[2][2] = 18.9;
  //tpc->y_offset[2][3] = 19.2;
  
  tpc->x_offset[2][0] = 0.81-4.9; //2016Jul9
  tpc->x_offset[2][1] = 0.03-4.9; //2016Jul9 
  tpc->y_offset[2][0] = 19.2+49.8;//2016Jul9  
  tpc->y_offset[2][1] = 19.7+49.8;//2016Jul9  
  tpc->y_offset[2][2] = 18.9+49.8;//2016Jul9  
  tpc->y_offset[2][3] = 19.2+49.8;//2016Jul9
  

  //TPC 4 at S2 (TPC 24) in air
  tpc->x_factor[3][0] = 0.068179;
  tpc->x_factor[3][1] = 0.066689;
  tpc->y_factor[3][0] = -0.0412;
  tpc->y_factor[3][1] = -0.0411;
  tpc->y_factor[3][2] = -0.0404;
  tpc->y_factor[3][3] = -0.0418;

  //tpc->x_offset[3][0] = -0.37+6.0-1.3; //21.10.2014
  //tpc->x_offset[3][1] = 2.3+6.0-1.3; //21.10.2014
  //tpc->y_offset[3][0] = 28.0;
  //tpc->y_offset[3][1] = 28.4;
  //tpc->y_offset[3][2] = 28.4;
  //tpc->y_offset[3][3] = 27.8;

  tpc->x_offset[3][0] = -0.37+6.0-1.3 -4.8;//2016Jul9
  tpc->x_offset[3][1] = 2.3+6.0-1.3-4.8;//2016Jul9
  tpc->y_offset[3][0] = 28.0+48.9;//2016Jul9
  tpc->y_offset[3][1] = 28.4+48.9;//2016Jul9
  tpc->y_offset[3][2] = 28.4+48.9;//2016Jul9
  tpc->y_offset[3][3] = 27.8+48.9; //2016Jul9
  

  //TPC 5  at S4 (TPC 41) in air
  tpc->x_factor[4][0] = (0.069442);
  tpc->x_factor[4][1] = (0.070509);
  tpc->y_factor[4][0] = (-0.045657);
  tpc->y_factor[4][1] = (-0.044335);
  tpc->y_factor[4][2] = (-0.043486);
  tpc->y_factor[4][3] = (-0.042131);

  tpc->x_offset[4][0] = (-0.556613);
  tpc->x_offset[4][1] = (-4.097965);
  tpc->y_offset[4][0] = (78.432506);
  tpc->y_offset[4][1] = (76.420553);
  tpc->y_offset[4][2] = (75.495579);
  tpc->y_offset[4][3] = (73.756001);

  
  //tpc->x_factor[4][0] = 0.0701334;
  //tpc->x_factor[4][1] = 0.0719196;
  //tpc->y_factor[4][0] =-0.03644;
  //tpc->y_factor[4][1] =-0.03624;
  //tpc->y_factor[4][2] =-0.03709;
  //tpc->y_factor[4][3] =-0.03729;

  //tpc->x_offset[4][0] = 0.86+5.4-4.1; //21.10.2014
  //tpc->x_offset[4][1] =-2.35+5.4-4.1; //21.10.2014
  //tpc->y_offset[4][0] = 30.6;
  //tpc->y_offset[4][1] = 30.9;
  //tpc->y_offset[4][2] = 32.2;
  //tpc->y_offset[4][3] = 32.1;
 

  //TPC 6 at S4 (TPC 42) in air
  tpc->x_factor[5][0] = (0.069366);
  tpc->x_factor[5][1] = (0.069374);
  tpc->y_factor[5][0] = (-0.038537);
  tpc->y_factor[5][1] = (-0.038826);
  tpc->y_factor[5][2] = (-0.038093);
  tpc->y_factor[5][3] = (-0.037369);

  tpc->x_offset[5][0] = (-0.067895);
  tpc->x_offset[5][1] = (0.789764);
  tpc->y_offset[5][0] = (53.681955);
  tpc->y_offset[5][1] = (50.091274);
  tpc->y_offset[5][2] = (51.295231);
  tpc->y_offset[5][3] = (50.501582);
  
  
  //tpc->x_factor[5][0] = 0.071256;
  //tpc->x_factor[5][1] = 0.072858;
  //tpc->y_factor[5][0] =-0.04215;
  //tpc->y_factor[5][1] =-0.04161;
  //tpc->y_factor[5][2] =-0.04100;
  //tpc->y_factor[5][3] =-0.04025;

  //tpc->x_offset[5][0] = -0.4-0.3+0.35; //21.10.2014
  //tpc->x_offset[5][1] = 1.1-0.3+0.35; //21.10.2014
  //tpc->y_offset[5][0] = 22.6;
  //tpc->y_offset[5][1] = 21.9;
  //tpc->y_offset[5][2] = 20.7;
  //tpc->y_offset[5][3] = 20.4;
  

  //TPC at S3 (TPC 31)
  tpc->x_factor[6][0] = 1.;
  tpc->x_factor[6][1] = 1.;
  tpc->y_factor[6][0] = 1.;
  tpc->y_factor[6][1] = 1.;
  tpc->y_factor[6][2] = 1.;
  tpc->y_factor[6][3] = 1.;

  tpc->x_offset[6][0] = 0.;
  tpc->x_offset[6][1] = 0.;
  tpc->y_offset[6][0] = 0.;
  tpc->y_offset[6][1] = 0.;
  tpc->y_offset[6][2] = 0.;
  tpc->y_offset[6][3] = 0.;

  //===========
  // Plastics
  //===========

  //index 2 for Sc21  
  //sci->x_a[0][2] =  -2065.2-27+55;  //  SC21 calibration ch->mm 27.05.2016
  //sci->x_a[1][2] =  0.73; //  27.05.2016 
  sci->x_a[0][2] =  -1.0*(-2065.2-27.+55.-29.);  // YT 2016/Jun/14, See Logbook p212-214
  sci->x_a[1][2] =  -1.0*(0.73); //  YT 2016/Jun/14, See Logbook p212-214         
  sci->x_a[2][2] =  0.000000;  // 
  sci->x_a[3][2] =  0.000000;  //                            
  sci->x_a[4][2] =  0.000000;  //                             
  sci->x_a[5][2] =  0.000000;  //                             
  sci->x_a[6][2] =  0.000000;  //  
  
  /*
  // index 2 for Sc21  
  sci->x_a[0][2] = 1184.51;  //  SC21 calibration ch->mm 
  sci->x_a[1][2] =  -0.5206; //  s323 test run: Xe fragments run
  sci->x_a[2][2] =  0.000000;  // 
  sci->x_a[3][2] =  0.000000;  //                            
  sci->x_a[4][2] =  0.000000;  //                             
  sci->x_a[5][2] =  0.000000;  //                             
  sci->x_a[6][2] =  0.000000;  //    
  */

  // index 5 for Sc41
  sci->x_a[0][5] = -5.84328*330.;  //  SC41 calibration ch->mm 
  sci->x_a[1][5] = 0.00558294;  //
  sci->x_a[2][5] = 0.000000;   //
  sci->x_a[3][5] = 0.000000;   //                            
  sci->x_a[4][5] = 0.000000;   //                             
  sci->x_a[5][5] = 0.000000;   //                             
  sci->x_a[6][5] = 0.000000;   //    

  // index 6 for Sc42
  sci->x_a[0][6] = 0.; //   SC42 calibration ch->mm 
  sci->x_a[1][6] = 1.; // 
  sci->x_a[2][6] = 0.000000;  // 
  sci->x_a[3][6] = 0.000000;  //                            
  sci->x_a[4][6] = 0.000000;  //                             
  sci->x_a[5][6] = 0.000000;  //                             
  sci->x_a[6][6] = 0.000000;  //

   // index 7 for Sc43
  sci->x_a[0][7] = 0.; //   SC43 calibration ch->mm 
  sci->x_a[1][7] = 1.; // 
  sci->x_a[2][7] = 0.000000;  // 
  sci->x_a[3][7] = 0.000000;  //                            
  sci->x_a[4][7] = 0.000000;  //                             
  sci->x_a[5][7] = 0.000000;  //                             
  sci->x_a[6][7] = 0.000000;  //

   // index 10 for Sc81
  sci->x_a[0][10] = 0.; //   SC81 calibration ch->mm 
  sci->x_a[1][10] = 1.; // 
  sci->x_a[2][10] = 0.000000;  // 
  sci->x_a[3][10] = 0.000000;  //                            
  sci->x_a[4][10] = 0.000000;  //                             
  sci->x_a[5][10] = 0.000000;  //                             
  sci->x_a[6][10] = 0.000000;  // 


  // TOF
  /*
  sci->tac_off[0] = 12027.1;  //SC21L-R  
  sci->tac_off[1] = 5843.28;  //SC41L-R 
  sci->tac_off[2] = 7718.14;  //SC41L-SC21L
  sci->tac_off[3] = 8452.87;  //SC41R-SC21R
  sci->tac_off[4] = 0.;  //SC42L-R
  sci->tac_off[5] = 0.;  //SC42L-SC21L
  sci->tac_off[6] = 0.;  //SC42R-SC21R
  sci->tac_off[7] = 0.;  //SC43L-R
  sci->tac_off[8] = 0.;  //SC81L-R
  sci->tac_off[9] = 0.;  //SC81L-SC21L
  sci->tac_off[10] = 0.;  //SC81R-SC21R
  */

  sci->tac_off[0] = 2327.95;  //SC21L-R  
  sci->tac_off[1] = 1447.37;  //SC41L-R 
  sci->tac_off[2] = 1833.28;  //SC41L-SC21L
  sci->tac_off[3] = 1972.59;  //SC41R-SC21R
  sci->tac_off[4] = 2073.4;  //SC42L-R
  sci->tac_off[5] = 1542.25;  //SC42L-SC21L
  sci->tac_off[6] = 1729.58;  //SC42R-SC21R
  sci->tac_off[7] = 1441.13;  //SC43L-R
  sci->tac_off[8] = 1777.77;  //SC81L-R
  sci->tac_off[9] = 3020.83;  //SC81L-SC21L
  sci->tac_off[10] = 1740.21;  //SC81R-SC21R



  // TAC calibration factors assumed for agata
  //   sci->tac_factor[0] = 5.571;   //SC21L-R [ps/ch]  , range 25 ns
  //   sci->tac_factor[1] = 5.604;   //SC41L-R [ps/ch]  , range 25 ns  
  // sci->tac_factor[0] = 5.89623;   //SC21L-R [ps/ch]  , range 25 ns, CH 19.10.2014
  // sci->tac_factor[1] = 5.51268;   //SC41L-R [ps/ch]  , range 25 ns, CH 19.10.2014
  // sci->tac_factor[4] = 5.609;      //SC42L-R [ps/ch]  , range 25 ns

  // //   sci->tac_factor[2] = 16.809;  // SC41L-SC21L [ps/ch] , range 75 ns
  // //   sci->tac_factor[3] = 17.149;  // SC41R-SC21R [ps/ch] , range 75 ns 
  // sci->tac_factor[2] = 10.55619;  // SC41L-SC21L [ps/ch] , range 50 ns, CH 19.10.2014
  // sci->tac_factor[3] = 11.17427;  // SC41R-SC21R [ps/ch] , range 50 ns, CH 19.10.2014 

  /*
  sci->tac_factor[0] = 10.4297;   //SC21L-R [ps/ch], range 10 ns 
  sci->tac_factor[1] = 5.58294;   //SC41L-R [ps/ch], range 10 ns
  sci->tac_factor[2] = 20.8056;   //SC41L-SC21L [ps/ch], range 10 ns
  sci->tac_factor[3] = 21.806;    //SC41R-SC21R [ps/ch], range 10 ns
  sci->tac_factor[4] = 1.;        //SC42L-R
  sci->tac_factor[5] = 1.;        //SC42L-SC21L
  sci->tac_factor[6] = 1.;        //SC42R-SC21R
  sci->tac_factor[7] = 1.;        //SC43L-R
  sci->tac_factor[8] = 1.;        //SC81L-R
  sci->tac_factor[9] = 1.;        //SC81L-SC21L
  sci->tac_factor[10] = 1.;        //SC81R-SC21R
  */

  sci->tac_factor[0] = 20.6895;   //SC21L-R [ps/ch], range 10 ns 
  sci->tac_factor[1] = 26.3158;   //SC41L-R [ps/ch], range 10 ns
  sci->tac_factor[2] = 22.0238;   //SC41L-SC21L [ps/ch], range 10 ns
  sci->tac_factor[3] = 21.6549;    //SC41R-SC21R [ps/ch], range 10 ns
  sci->tac_factor[4] = 20.8798;        //SC42L-R
  sci->tac_factor[5] = 21.3739;        //SC42L-SC21L
  sci->tac_factor[6] = 21.0525;        //SC42R-SC21R
  sci->tac_factor[7] = 21.8067;        //SC43L-R
  sci->tac_factor[8] = 20.911;        //SC81L-R
  sci->tac_factor[9] = 20.8333;        //SC81L-SC21L
  sci->tac_factor[10] = 20.1292;        //SC81R-SC21R

  sci->tof_bll2  = 1.;    // not used online [ps/ch]
  sci->tof_brr2  = 1.;    // not used online
  sci->tof_bll3  = 1.;    // not used online 
  sci->tof_brr3  = 1.;    // not used online
  sci->tof_bll4  = 1.;    // not used online 
  sci->tof_brr4  = 1.;    // not used online

  sci->tof_a2 = 0.; // [ps] offset   Tof S41-S21
  sci->tof_a3 = 0.; // [ps] offset   Tof S42-S21
  sci->tof_a4 = 0.; // [ps] offset   Tof S81-S21


  //   id->id_tofoff2  = 197689.0; // [ps]  S41-S21 from 9.Oct on because of TAC-LL shift
  //   id->id_path2    = 119120.0;

  // used for ToF S2-S4, extracted from agata calib
  //id->id_tofoff2  = 175940.0; // [ps]  S411 21.10.2014
  //id->id_path2    = 126160.0; // path/c [ps]  S411_48 21.10.2014 
  //id->id_tofoff2  = 170680.0; // [ps]  S411 22.10.2014
  //id->id_tofoff2  = 170470.0; // [ps]  S411 shifted 25.10.2014
  //id->id_path2    = 121730.0; // path/c [ps]  S411_48 22.10.2014 

  //  id->id_tofoff2  = 202971  ; // [ps]  S411_86 fit with only one point shifted 25
  //  id->id_path2    = 125329.2; // path/c [ps]  S411 Uranium data -8.6cm

  id->id_tofoff2  = 200120.  ; // [ps]  S411_149 with 12C
  id->id_path2    = 120450. ; // path/c [ps]  old from Xe in June 2016

  id->id_tofoff3  = 210000.;   // [ps] //check
  id->id_path3    = 120000.;   // path/c [ps] //check

  id->id_tofoff4  = 210000.;   // [ps]  //check 
  id->id_path4    = 120000.;   // path/c [ps]  //check

  id->ID_Z_AoverQ_num[0]=5; 
  id->ID_Z_AoverQ_num[1]=5; 
  id->ID_Z_AoverQ_num[2]=5; 
  id->ID_Z_AoverQ_num[3]=5; 
  id->ID_Z_AoverQ_num[4]=5; 

  id->ID_Z_AoverQ[0][0][0]=2.208      ; id->ID_Z_AoverQ[0][0][1]=1180;
  id->ID_Z_AoverQ[0][1][0]=2.208      ; id->ID_Z_AoverQ[0][1][1]=0;
  id->ID_Z_AoverQ[0][2][0]=2.220      ; id->ID_Z_AoverQ[0][2][1]=0;
  id->ID_Z_AoverQ[0][3][0]=2.220      ; id->ID_Z_AoverQ[0][3][1]=1180;
  id->ID_Z_AoverQ[0][4][0]=2.208      ; id->ID_Z_AoverQ[0][4][1]=1180;

  id->ID_Z_AoverQ[1][0][0]=2.208+0.012; id->ID_Z_AoverQ[1][0][1]=1180; 
  id->ID_Z_AoverQ[1][1][0]=2.208+0.012; id->ID_Z_AoverQ[1][1][1]=0; 
  id->ID_Z_AoverQ[1][2][0]=2.220+0.012; id->ID_Z_AoverQ[1][2][1]=0; 
  id->ID_Z_AoverQ[1][3][0]=2.220+0.012; id->ID_Z_AoverQ[1][3][1]=1180; 
  id->ID_Z_AoverQ[1][4][0]=2.208+0.012; id->ID_Z_AoverQ[1][4][1]=1180; 

  id->ID_Z_AoverQ[2][0][0]=2.208+0.024; id->ID_Z_AoverQ[2][0][1]=1180; 
  id->ID_Z_AoverQ[2][1][0]=2.208+0.024; id->ID_Z_AoverQ[2][1][1]=60; 
  id->ID_Z_AoverQ[2][2][0]=2.220+0.024; id->ID_Z_AoverQ[2][2][1]=60; 
  id->ID_Z_AoverQ[2][3][0]=2.220+0.024; id->ID_Z_AoverQ[2][3][1]=1180; 
  id->ID_Z_AoverQ[2][4][0]=2.208+0.024; id->ID_Z_AoverQ[2][4][1]=1180; 

  id->ID_Z_AoverQ[3][0][0]=2.208+0.036; id->ID_Z_AoverQ[3][0][1]=1180; 
  id->ID_Z_AoverQ[3][1][0]=2.208+0.036; id->ID_Z_AoverQ[3][1][1]=60; 
  id->ID_Z_AoverQ[3][2][0]=2.220+0.036; id->ID_Z_AoverQ[3][2][1]=60; 
  id->ID_Z_AoverQ[3][3][0]=2.220+0.036; id->ID_Z_AoverQ[3][3][1]=1180; 
  id->ID_Z_AoverQ[3][4][0]=2.208+0.036; id->ID_Z_AoverQ[3][4][1]=1180; 

  id->ID_Z_AoverQ[4][0][0]=2.208+0.048; id->ID_Z_AoverQ[4][0][1]=1180; 
  id->ID_Z_AoverQ[4][1][0]=2.208+0.048; id->ID_Z_AoverQ[4][1][1]=60; 
  id->ID_Z_AoverQ[4][2][0]=2.220+0.048; id->ID_Z_AoverQ[4][2][1]=60; 
  id->ID_Z_AoverQ[4][3][0]=2.220+0.048; id->ID_Z_AoverQ[4][3][1]=1180; 
  id->ID_Z_AoverQ[4][4][0]=2.208+0.048; id->ID_Z_AoverQ[4][4][1]=1180; 


  id->ID_x2AoverQ_num[0]=5; 
  id->ID_x2AoverQ_num[1]=5; 
  id->ID_x2AoverQ_num[2]=5; 
  id->ID_x2AoverQ_num[3]=5; 
  id->ID_x2AoverQ_num[4]=5; 
  id->ID_x2AoverQ_num[5]=5; 

// <<<<<<< Updated upstream
//   id->ID_x2AoverQ[0][0][0]=2.208      ; id->ID_x2AoverQ[0][0][1]=-100;
//   id->ID_x2AoverQ[0][1][0]=2.208      ; id->ID_x2AoverQ[0][1][1]=100;
//   id->ID_x2AoverQ[0][2][0]=2.220      ; id->ID_x2AoverQ[0][2][1]=100;
//   id->ID_x2AoverQ[0][3][0]=2.220      ; id->ID_x2AoverQ[0][3][1]=-100;
//   id->ID_x2AoverQ[0][4][0]=2.208      ; id->ID_x2AoverQ[0][4][1]=-100;
                                      
//   id->ID_x2AoverQ[1][0][0]=2.208+0.012; id->ID_x2AoverQ[1][0][1]=-100; 
//   id->ID_x2AoverQ[1][1][0]=2.208+0.012; id->ID_x2AoverQ[1][1][1]=100; 
//   id->ID_x2AoverQ[1][2][0]=2.220+0.012; id->ID_x2AoverQ[1][2][1]=100; 
//   id->ID_x2AoverQ[1][3][0]=2.220+0.012; id->ID_x2AoverQ[1][3][1]=-100; 
//   id->ID_x2AoverQ[1][4][0]=2.208+0.012; id->ID_x2AoverQ[1][4][1]=-100; 
                                      
//   id->ID_x2AoverQ[2][0][0]=2.208+0.024; id->ID_x2AoverQ[2][0][1]=-100; 
//   id->ID_x2AoverQ[2][1][0]=2.208+0.024; id->ID_x2AoverQ[2][1][1]=100; 
//   id->ID_x2AoverQ[2][2][0]=2.220+0.024; id->ID_x2AoverQ[2][2][1]=100; 
//   id->ID_x2AoverQ[2][3][0]=2.220+0.024; id->ID_x2AoverQ[2][3][1]=-100; 
//   id->ID_x2AoverQ[2][4][0]=2.208+0.024; id->ID_x2AoverQ[2][4][1]=-100; 
                                      
//   id->ID_x2AoverQ[3][0][0]=2.208+0.036; id->ID_x2AoverQ[3][0][1]=-100; 
//   id->ID_x2AoverQ[3][1][0]=2.208+0.036; id->ID_x2AoverQ[3][1][1]=100; 
//   id->ID_x2AoverQ[3][2][0]=2.220+0.036; id->ID_x2AoverQ[3][2][1]=100; 
//   id->ID_x2AoverQ[3][3][0]=2.220+0.036; id->ID_x2AoverQ[3][3][1]=-100; 
//   id->ID_x2AoverQ[3][4][0]=2.208+0.036; id->ID_x2AoverQ[3][4][1]=-100; 
                                      
//   id->ID_x2AoverQ[4][0][0]=2.208+0.048; id->ID_x2AoverQ[4][0][1]=-100; 
//   id->ID_x2AoverQ[4][1][0]=2.208+0.048; id->ID_x2AoverQ[4][1][1]=100; 
//   id->ID_x2AoverQ[4][2][0]=2.220+0.048; id->ID_x2AoverQ[4][2][1]=100; 
//   id->ID_x2AoverQ[4][3][0]=2.220+0.048; id->ID_x2AoverQ[4][3][1]=-100; 
//   id->ID_x2AoverQ[4][4][0]=2.208+0.048; id->ID_x2AoverQ[4][4][1]=-100; 
// =======


 id->ID_x2AoverQ[0][0][0]=2.24433; id->ID_x2AoverQ[0][0][1]=42.5864;
 id->ID_x2AoverQ[0][1][0]=2.17429; id->ID_x2AoverQ[0][1][1]=-68.2431;
 id->ID_x2AoverQ[0][2][0]=2.18351; id->ID_x2AoverQ[0][2][1]=-70.9073;
 id->ID_x2AoverQ[0][3][0]=2.2573; id->ID_x2AoverQ[0][3][1]=40.9879;
 id->ID_x2AoverQ[0][4][0]=2.24433;id->ID_x2AoverQ[0][4][1]=42.5864;
 
  id->ID_x2AoverQ[1][0][0]=2.27782; id->ID_x2AoverQ[1][0][1]=58.1797; 
  id->ID_x2AoverQ[1][1][0]=2.18477; id->ID_x2AoverQ[1][1][1]=-66.2442; 
  id->ID_x2AoverQ[1][2][0]=2.19417; id->ID_x2AoverQ[1][2][1]=-69.7005; 
  id->ID_x2AoverQ[1][3][0]=2.29192; id->ID_x2AoverQ[1][3][1]=57.0277; 
  id->ID_x2AoverQ[1][4][0]=2.27782; id->ID_x2AoverQ[1][4][1]=58.1797; 

  id->ID_x2AoverQ[2][0][0]=2.28597; id->ID_x2AoverQ[2][0][1]=40.8986; 
  id->ID_x2AoverQ[2][1][0]=2.19548; id->ID_x2AoverQ[2][1][1]=-69.1244; 
  id->ID_x2AoverQ[2][2][0]=2.21129; id->ID_x2AoverQ[2][2][1]=-69.1244; 
  id->ID_x2AoverQ[2][3][0]=2.30014; id->ID_x2AoverQ[2][3][1]=40.3226; 
  id->ID_x2AoverQ[2][4][0]=2.28597; id->ID_x2AoverQ[2][4][1]=40.8986; 

  id->ID_x2AoverQ[3][0][0]=2.30468; id->ID_x2AoverQ[3][0][1]=41.0484; 
  id->ID_x2AoverQ[3][1][0]=2.20712; id->ID_x2AoverQ[3][1][1]=-73.4407; 
  id->ID_x2AoverQ[3][2][0]=2.22237; id->ID_x2AoverQ[3][2][1]=-73.1567; 
  id->ID_x2AoverQ[3][3][0]=2.32009; id->ID_x2AoverQ[3][3][1]=41.0484; 
  id->ID_x2AoverQ[3][4][0]=2.30468; id->ID_x2AoverQ[3][4][1]=41.0484; 
  
  id->ID_x2AoverQ[4][0][0]=2.32064; id->ID_x2AoverQ[4][0][1]=39.6964; 
  id->ID_x2AoverQ[4][1][0]=2.22332; id->ID_x2AoverQ[4][1][1]=-73.875; 
  id->ID_x2AoverQ[4][2][0]=2.23886; id->ID_x2AoverQ[4][2][1]=-73.517; 
  id->ID_x2AoverQ[4][3][0]=2.33663; id->ID_x2AoverQ[4][3][1]=39.6964; 
  id->ID_x2AoverQ[4][4][0]=2.32064; id->ID_x2AoverQ[4][4][1]=39.6964; 
  
  id->ID_x2AoverQ[5][0][0]=2.26732; id->ID_x2AoverQ[5][0][1]=84.9736; 
  id->ID_x2AoverQ[5][1][0]=2.35488; id->ID_x2AoverQ[5][1][1]=78.52  ; 
  id->ID_x2AoverQ[5][2][0]=2.34538; id->ID_x2AoverQ[5][2][1]=75.078 ; 
  id->ID_x2AoverQ[5][3][0]=2.24458; id->ID_x2AoverQ[5][3][1]=82.0336; 
  id->ID_x2AoverQ[5][4][0]=2.26732; id->ID_x2AoverQ[5][4][1]=84.9736; 

  // infinite gating

  // Change the magnetic field here                                                                                   
  frs->bfield[0] = 0.74845;      // FRS D3 field [Tesla]
  frs->bfield[1] = 0.73574;      // FRS D4 field [Tesla]  
  frs->bfield[2] = 0.7178;       // FRS D3 field [Tesla]
  frs->bfield[3] = 0.73174;      // FRS D4 field [Tesla] 

  // from 12C
  frs->bfield[0] = 0.7340;      // FRS D3 field [Tesla] 
  frs->bfield[1] = 0.72174;      // FRS D4 field [Tesla] 
  frs->bfield[2] = 0.7071;       // FRS D3 field [Tesla] 
  frs->bfield[3] = 0.7186;      // FRS D4 field [Tesla]

  frs->dispersion[0] = -6.474266; // RUN81-TA2B-220CM 27.05.2016
  frs->dispersion[1] =  7.7064; // RUN81-TA2B-220CM 27.05.2016
  frs->magnification[1] = 1.1903;   // RUN81-TA2B-220CM 27.05.2016


  // 13.06.2016 S411_86 with 124Xe
  //  frs->rho0[0]   = 11.1340;    // FRS (D2) radius [m]   27.05.2016 
  //  frs->rho0[1]   = 11.287;   // FRS (D3+D4)/2 radius [m]  27.05.2016 
  
  // 14.06.2016 adjusted for S411_87 with 124Xe+thick degrader  
  //frs->rho0[0]   = 11.1340;    // FRS (D2) radius [m]   
  //frs->rho0[1]   = 11.2415;   // FRS (D3+D4)/2 radius [m] 

  //26.06.2016 = new effective radius
  //frs->rho0[0]   = 11.1349;    // FRS (D2) radius [m]   
  //frs->rho0[1]   = 11.377;   // FRS (D3+D4)/2 radius [m] 

  //05.07.2016 = new effective radius S411_149 12C
  frs->rho0[0]   = 11.1374;    // FRS (D1+D2)/2 radius [m]   
  frs->rho0[1]   = 11.3516;    // FRS (D3+D4)/2 radius [m]


  //  frs->rho0[0]   = 11.22374;    // FRS (D2) radius [m]    
  //  frs->rho0[1]   = 11.282095;   // FRS (D3+D4)/2 radius [m]   

  //   frs->rho0[0]   = 11.203 ;   // FRS (D1+D2)/2 radius [m]    
  //   frs->rho0[1]   = 11.2641 ;   // FRS (D3+D4)/2 radius [m]   16.09.11


  frs->primary_z = 12.*0.32;  // C beam adjusted for S411_150 (B,Be,Li) 
  frs->offset_z  =  0.85 ;
  frs->offset_z2  = 3.0 ;
  frs->offset_z3  = 0.0 ;

  frs->a2AoQCorr = 0.00095;
  frs->a4AoQCorr = 0.0000;//0.0015
  
  frs->fill_raw_histos = true;

	
  // MUSIC 1 velocity correction 
  //id->vel_a[0] =  11612 ;  // S411 oct2014 first days
  id->vel_a[0] =  2750;//11673 *1.096   // shifted calib 25.10.2104  
  id->vel_a[1] =  -2200; // -17462*1.096 
  id->vel_a[2] =   0.0; //7682 *1.096
  id->vel_a[3] =   0.0; 

  // MUSIC 2 velocity correction 
  //id->vel_a2[0] =  19577; // S411 oct2014 first days
  id->vel_a2[0] =  19602 ;  // shifted calib 25.10.2104
  id->vel_a2[1] =  -34442;
  id->vel_a2[2] =  17273;
  id->vel_a2[3] =  0.0; 
	
  // MUSIC 3 velocity correction
  id->vel_a3[0] =  13951.37; //
  id->vel_a3[1] =  -38369.9; 
  id->vel_a3[2] =  28396.46;
  id->vel_a3[3] =  0.0;    
  
//  id->vel_a3[0] =  0.0; //
//  id->vel_a3[1] =  1.0; 
//  id->vel_a3[2] =  0.0;
//  id->vel_a3[3] =  0.0;  

  // gate on Z 
  id->zgate_low = 85;
  id->zgate_high = 90;

  //=======
  //  Si
  //======= 
   
  si->si_factor1=5.82775; //CH 03/06/2016
  si->si_offset1=-381.593; //CH 03/06/2016

  si->si_factor2=3.809; //CH 18.10.2014
  si->si_offset2=-529.01; //CH 18.10.2014

  si->si_factor3=3.2596; //CH 21.05.2016
  si->si_offset3=-550.59; //CH 21.05.2016|

  si->si_factor4=3.2596; //CH 21.05.2016
  si->si_offset4=-550.59; //CH 21.05.2016

  si->dssd_factor[0] = 1.;
  si->dssd_factor[1] = 1.;
  si->dssd_factor[2] = 1.;
  si->dssd_factor[3] = 1.;
  si->dssd_factor[4] = 1.;
  si->dssd_factor[5] = 1.;
  si->dssd_factor[6] = 1.;
  si->dssd_factor[7] = 1.;
  si->dssd_factor[8] = 1.;
  si->dssd_factor[9] = 1.;
  si->dssd_factor[10] = 1.;
  si->dssd_factor[11] = 1.;
  si->dssd_factor[12] = 1.;
  si->dssd_factor[13] = 1.;
  si->dssd_factor[14] = 1.;
  si->dssd_factor[15] = 1.;
  si->dssd_factor[16] = 1.;
  si->dssd_factor[17] = 1.;
  si->dssd_factor[18] = 1.;
  si->dssd_factor[19] = 1.;
  si->dssd_factor[20] = 1.;
  si->dssd_factor[21] = 1.;
  si->dssd_factor[22] = 1.;
  si->dssd_factor[23] = 1.;
  si->dssd_factor[24] = 1.;
  si->dssd_factor[25] = 1.;
  si->dssd_factor[26] = 1.;
  si->dssd_factor[27] = 1.;
  si->dssd_factor[28] = 1.;
  si->dssd_factor[29] = 1.;
  si->dssd_factor[30] = 1.;
  si->dssd_factor[31] = 1.;

  si->dssd_offset[0] = 0.;
  si->dssd_offset[1] = 0.;
  si->dssd_offset[2] = 0.;
  si->dssd_offset[3] = 0.;
  si->dssd_offset[4] = 0.;
  si->dssd_offset[5] = 0.;
  si->dssd_offset[6] = 0.;
  si->dssd_offset[7] = 0.;
  si->dssd_offset[8] = 0.;
  si->dssd_offset[9] = 0.;
  si->dssd_offset[10] = 0.;
  si->dssd_offset[11] = 0.;
  si->dssd_offset[12] = 0.;
  si->dssd_offset[13] = 0.;
  si->dssd_offset[14] = 0.;
  si->dssd_offset[15] = 0.;
  si->dssd_offset[16] = 0.;
  si->dssd_offset[17] = 0.;
  si->dssd_offset[18] = 0.;
  si->dssd_offset[19] = 0.;
  si->dssd_offset[20] = 0.;
  si->dssd_offset[21] = 0.;
  si->dssd_offset[22] = 0.;
  si->dssd_offset[23] = 0.;
  si->dssd_offset[24] = 0.;
  si->dssd_offset[25] = 0.;
  si->dssd_offset[26] = 0.;
  si->dssd_offset[27] = 0.;
  si->dssd_offset[28] = 0.;
  si->dssd_offset[29] = 0.;
  si->dssd_offset[30] = 0.;
  si->dssd_offset[31] = 0.;
  
  //=========
  //MR-TOF-MS
  //=========
  
  mrtof->MRTOFMS_a=0.069;
  mrtof->MRTOFMS_b=0.71;
  mrtof->MRTOFMS_t0=0;
  mrtof->MRTOFMS_TXRS=0;

  cout << "Focus distance S4: " << frs->dist_focS4 << endl;


  ElecMod->Nb_Modules = 64;
  ElecMod->Nb_QDC = 3;
  ElecMod->Nb_ADC = 1;
  ElecMod->Nb_TDC = 5;
  ElecMod->Nb_Scaler = 2;
  ElecMod->Nb_TimeStamp = 2;
  
  for(int i = 0;i<64;++i)
    ElecMod->Nb_Channels.insert(std::pair<int,int>(i,32));

  
  // Crate FRS procID 10
  int IdMod = 0;
  //std::unordered_map<int,int> CrateFRS;
  std::map<int,int> CrateFRS;
  CrateFRS.insert(std::pair<int,int>(1,IdMod++));
  CrateFRS.insert(std::pair<int,int>(9,IdMod++));
  CrateFRS.insert(std::pair<int,int>(8,IdMod++));
  CrateFRS.insert(std::pair<int,int>(11,IdMod++));
  CrateFRS.insert(std::pair<int,int>(12,IdMod++));
  
  //std::unordered_map<int,int> CrateTPC;
  std::map<int,int> CrateTPC;
  //CrateTPC.insert(std::pair<int,int>(5,IdMod++));
  CrateTPC.insert(std::pair<int,int>(8,IdMod++));
  CrateTPC.insert(std::pair<int,int>(9,IdMod++));
  CrateTPC.insert(std::pair<int,int>(15,IdMod++));
  CrateTPC.insert(std::pair<int,int>(3,IdMod++));
  
  //std::unordered_map<int,int> CrateUser;
  std::map<int,int> CrateUser;
  CrateUser.insert(std::pair<int,int>(0,IdMod++));

  std::map<int,int> CrateMT;
  CrateMT.insert(std::pair<int,int>(0,IdMod++));
  CrateMT.insert(std::pair<int,int>(2,IdMod++));

  std::map<int,int> CrateSOFIA; //added                                                                                                                                 
  CrateSOFIA.insert(std::pair<int,int>(0,IdMod++));
  CrateSOFIA.insert(std::pair<int,int>(1,IdMod++));
  CrateSOFIA.insert(std::pair<int,int>(2,IdMod++));

  
  Map1* temp1 = new Map1("temp1");
  Map1* temp2 = new Map1("temp2");
  Map1* temp3 = new Map1("temp3");
  Map1* temp4 = new Map1("temp4");
  Map1* temp5 = new Map1("temp5");
  temp1->map=CrateFRS;
  temp2->map=CrateTPC;
  temp3->map=CrateUser;
  temp4->map=CrateMT;
  temp5->map=CrateSOFIA;
  
  TObjString* key1 = new TObjString("10");
  TObjString* key2 = new TObjString("20");
  TObjString* key3 = new TObjString("30");
  TObjString* key4 = new TObjString("40");
  TObjString* key5 = new TObjString("50");
  
  ElecMod->Maptemp.Add(key1,temp1);
  ElecMod->Maptemp.Add(key2,temp2);
  ElecMod->Maptemp.Add(key3,temp3);
  ElecMod->Maptemp.Add(key4,temp4);
  ElecMod->Maptemp.Add(key5,temp5);
  
  // ElecMod->Maptemp.insert(std::pair<int,map<int,int> >(10,CrateFRS));
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(20,CrateTPC));
  // ElecMod->MapCrates.insert(std::pair<int,std::unordered_map<int,int> >(30,CrateUser));
  
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(10,CrateFRS));
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(20,CrateTPC));
  //ElecMod->MapCrates.insert(std::pair<int,std::map<int,int> >(30,CrateUser));

  
  ElecMod->ModType.push_back("FRS_SC_1");
  ElecMod->ModType.push_back("FRS_TDC_1");
  ElecMod->ModType.push_back("FRS_TDC_2");
  ElecMod->ModType.push_back("FRS_QDC_1");
  ElecMod->ModType.push_back("FRS_ADC_1");
  ElecMod->ModType.push_back("TPC_TDC_1");
  ElecMod->ModType.push_back("TPC_TDC_2");
  ElecMod->ModType.push_back("TPC_QDC_1");
  ElecMod->ModType.push_back("TPC_QDC_2");
  ElecMod->ModType.push_back("USER_MTDC_1");
  ElecMod->ModType.push_back("MT_SC_1");
  ElecMod->ModType.push_back("MT_MTDC_1");
  ElecMod->ModType.push_back("SOFIA_VTX_1");
  ElecMod->ModType.push_back("SOFIA_MDP_1");
  ElecMod->ModType.push_back("SOFIA_MADC_1");


  ElecMod->Scaler32bit=1;
  //ElecMod->EventFlags.push_back(0x00000000);
  ElecMod->EventFlags.push_back(0x00000100);
  ElecMod->EventFlags.push_back(0x00000200);
  ElecMod->EventFlags.push_back(0x00000300);
  
  // ElecMod->ModType.push_back("QDC_11");
  // ElecMod->ModType.push_back("QDC_12");
  // ElecMod->ModType.push_back("QDC_13");
  // ElecMod->ModType.push_back("QDC_14");
  // ElecMod->ModType.push_back("QDC_15");
  // ElecMod->ModType.push_back("QDC_16");
  // ElecMod->ModType.push_back("QDC_17");
  // ElecMod->ModType.push_back("QDC_18");
  
  ElecMod->setMap();

  ElecMod->Print();
  
  cout << "Setup done " << endl;




}



void BS_Detector_System::get_Event_data(Raw_Event* RAW){
	    
    RAW->set_DATA_MUSIC(de, de_cor);
    RAW->set_DATA_SCI(sci_l, sci_r, sci_e, sci_tx, sci_x);
    RAW->set_DATA_SCI_dT(dt_21l_21r, dt_41l_41r, dt_21l_41l, dt_21r_41r, dt_42l_42r, dt_43l_43r,
			    dt_42l_21l, dt_42r_21r, dt_81l_81r, dt_21l_81l, dt_21r_81r);
    RAW->set_DATA_SCI_ToF(sci_tofll2, sci_tofll3, sci_tof2, sci_tofrr2, sci_tofrr3, sci_tof3);
    RAW->set_DATA_ID_2_4(id_x2, id_y2, id_a2, id_b2, id_x4, id_y4, id_a4, id_b4);
    RAW->set_DATA_ID_Beta_Rho(id_brho, id_rho, id_beta, id_beta3, id_gamma);
    RAW->set_DATA_ID_Z_AoQ(id_AoQ, id_AoQ_corr, id_z, id_z2, id_z3);
    RAW->set_DATA_ID_Timestamp(timestamp, ts, ts2);
	
}

//---------------------------------------------------------------

void BS_Detector_System::Process_MBS(int* pdata){

}

int* BS_Detector_System::get_pdata(){return pdata;}

//---------------------------------------------------------------

Float_t BS_Detector_System::rand0_5(){
    
  return rand()*1./RAND_MAX;// - 0.5; 
    
}

//---------------------------------------------------------------

Int_t BS_Detector_System::getbits(Int_t value, int nword, int start, int length){
    
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*16 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
  
}

//---------------------------------------------------------------

Int_t BS_Detector_System::get2bits(Int_t value, int nword, int start, int length){
    
  UInt_t buf = (UInt_t) value;
  buf = buf >> ((nword-1)*32 + start - 1);
  buf = buf & ((1 << length) - 1);
  return buf;
  
}

//---------------------------------------------------------------
Bool_t BS_Detector_System::Check_WinCond(Float_t P, Float_t* V){
    
    if(P >= V[0] && P <= V[1]) return true;
    else return false;

}

//---------------------------------------------------------------
Bool_t BS_Detector_System::Check_WinCond_Multi(Float_t P, Float_t** V, int cond_num){
    
    if(P >= V[cond_num][0] && P <= V[cond_num][1]) return true;
    else return false;

}

//---------------------------------------------------------------
Bool_t BS_Detector_System::Check_WinCond_Multi_Multi(Float_t P, Float_t*** V, int cond_num, int cond_num_2){
    
    if(P >= V[cond_num][cond_num_2][0] && P <= V[cond_num][cond_num_2][1]) return true;
    else return false;

}

//---------------------------------------------------------------

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
Bool_t BS_Detector_System::Check_PolyCond(Float_t* P, Float_t** V, int n ){
    int    cn = 0;    // the  crossing number counter

    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {    // edge from V[i]  to V[i+1]
       if (((V[i][1] <= P[1]) && (V[i+1][1] > P[1]))     // an upward crossing
        || ((V[i][1] > P[1]) && (V[i+1][1] <=  P[1]))) { // a downward crossing
            // compute  the actual edge-ray intersect x-coordinate
            float vt = (float)(P[1]  - V[i][1]) / (V[i+1][1] - V[i][1]);
            if (P[0] <  V[i][0] + vt * (V[i+1][0] - V[i][0])) // P.x < intersect
                 ++cn;   // a valid crossing of y=P.y right of P.x
        }
    }
    
    if((cn&1) == 0) return false;
    if((cn&1) == 1) return true;
    //return (cn&1);    // 0 if even (out), and 1 if  odd (in)

}

//---------------------------------------------------------------

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
Bool_t BS_Detector_System::Check_PolyCond_Multi(Float_t* P, Float_t*** V, int n, int cond_num )
{
    int    cn = 0;    // the  crossing number counter

    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {    // edge from V[i]  to V[i+1]
       if (((V[cond_num][i][1] <= P[1]) && (V[cond_num][i+1][1] > P[1]))     // an upward crossing
        || ((V[cond_num][i][1] > P[1]) && (V[cond_num][i+1][1] <=  P[1]))) { // a downward crossing
            // compute  the actual edge-ray intersect x-coordinate
            float vt = (float)(P[1]  - V[cond_num][i][1]) / (V[cond_num][i+1][1] - V[cond_num][i][1]);
            if (P[0] <  V[cond_num][i][0] + vt * (V[cond_num][i+1][0] - V[cond_num][i][0])) // P.x < intersect
                 ++cn;   // a valid crossing of y=P.y right of P.x
        }
    }
    
    if((cn&1) == 0) return false;
    if((cn&1) == 1) return true;

    //return (cn&1);    // 0 if even (out), and 1 if  odd (in)

}
//---------------------------------------------------------------

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
Bool_t BS_Detector_System::Check_PolyCond_X_Y(Float_t X, Float_t Y, Float_t** V, int n ){
    int    cn = 0;    // the  crossing number counter

    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {    // edge from V[i]  to V[i+1]
       if (((V[i][1] <= Y) && (V[i+1][1] > Y))     // an upward crossing
        || ((V[i][1] > Y) && (V[i+1][1] <=  Y))) { // a downward crossing
            // compute  the actual edge-ray intersect x-coordinate
            float vt = (float)(Y  - V[i][1]) / (V[i+1][1] - V[i][1]);
            if (X <  V[i][0] + vt * (V[i+1][0] - V[i][0])) // P.x < intersect
                 ++cn;   // a valid crossing of y=P.y right of P.x
        }
    }
    
    if((cn&1) == 0) return false;
    if((cn&1) == 1) return true;
    //return (cn&1);    // 0 if even (out), and 1 if  odd (in)

}

//---------------------------------------------------------------

// cn_PnPoly(): crossing number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  0 = outside, 1 = inside
// This code is patterned after [Franklin, 2000]
Bool_t BS_Detector_System::Check_PolyCond_Multi_X_Y(Float_t X, Float_t Y, Float_t*** V, int n, int cond_num )
{
    int    cn = 0;    // the  crossing number counter

    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {    // edge from V[i]  to V[i+1]
       if (((V[cond_num][i][1] <= Y) && (V[cond_num][i+1][1] > Y))     // an upward crossing
        || ((V[cond_num][i][1] > Y) && (V[cond_num][i+1][1] <=  Y))) { // a downward crossing
            // compute  the actual edge-ray intersect x-coordinate
            float vt = (float)(Y  - V[cond_num][i][1]) / (V[cond_num][i+1][1] - V[cond_num][i][1]);
            if (X <  V[cond_num][i][0] + vt * (V[cond_num][i+1][0] - V[cond_num][i][0])) // P.x < intersect
                 ++cn;   // a valid crossing of y=P.y right of P.x
        }
    }
    
    if((cn&1) == 0) return false;
    if((cn&1) == 1) return true;

    //return (cn&1);    // 0 if even (out), and 1 if  odd (in)

}

//---------------------------------------------------------------

void BS_Detector_System::Setup_Conditions(){
        
    string line;
    int line_number = 0;
    
    const char* format = "%f %f %f %f %f %f %f %f %f %f %f %f %f %f";

    ifstream cond_a("Configuration_Files/FRS_Window_Conditions/lim_csum.txt");
    
    lim_csum = new Float_t**[4];
    
    for(int i = 0; i < 4; ++i){
	
	lim_csum[i] = new Float_t*[7];
	
	    for(int j = 0; j < 7; ++j){
		
		
		lim_csum[i][j] = new Float_t[2];
		
	    }
    }
    
    
    while(/*cond_a.good()*/getline(cond_a,line,'\n')){
	
	//getline(cond_a,line,'\n');
	if(line[0] == '#') continue;
	

	    sscanf(line.c_str(),format,&lim_csum[line_number][0][0],&lim_csum[line_number][0][1]
					,&lim_csum[line_number][1][0],&lim_csum[line_number][1][1]
					,&lim_csum[line_number][2][0],&lim_csum[line_number][2][1]
					,&lim_csum[line_number][3][0],&lim_csum[line_number][3][1]
					,&lim_csum[line_number][4][0],&lim_csum[line_number][4][1]
					,&lim_csum[line_number][5][0],&lim_csum[line_number][5][1]
					,&lim_csum[line_number][6][0],&lim_csum[line_number][6][1]);
					
	line_number++;
	
	
    }
    
   /*lim_csum = {{{300,1800},{350,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}},
				 {{300,1800},{300,1800},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}},
				 {{300,1800},{300,1840},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}},
				 {{300,1800},{300,1810},{300,1800},{300,1700},{300,2000},{300,2000},{300,2000}}};
	*/			 
				 

    
    lim_xsum = new Float_t*[13];
    lim_ysum = new Float_t*[13];
    
    for(int i = 0; i < 13; ++i){
	
	
	lim_xsum[i] = new Float_t[2];
	lim_ysum[i] = new Float_t[2];
	
	//lim_xsum[i][0] = 1;
	//lim_xsum[i][1] = 8000;
	//lim_ysum[i][0] = 2;
	//lim_ysum[i][1] = 8000;
	
	
    }
    
    line_number = 0;
    
    format = "%f %f";

    ifstream cond_b("Configuration_Files/FRS_Window_Conditions/lim_xsum.txt");
    
    while(/*cond_b.good()*/getline(cond_b,line,'\n')){
	
	//getline(cond_b,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&lim_xsum[line_number][0],&lim_xsum[line_number][1]);
					
	line_number++;
    }
    
    line_number = 0;
    
    format = "%f %f";

     ifstream cond_c("Configuration_Files/FRS_Window_Conditions/lim_ysum.txt");
    
    while(/*cond_c.good()*/getline(cond_c,line,'\n')){
	
	//getline(cond_c,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&lim_ysum[line_number][0],&lim_ysum[line_number][1]);
					
	line_number++;
    }
    
    /*lim_xsum = {	{1,8000},  // MW11
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

    lim_ysum = { {2,8000},  // MW11
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
				
    
    
    /*for(int i=0;i<7;i++){
	
	//    cTPC_CSUM[i][0]=MakeWindowCond("TPC/CSUM1",condname,lim_csum1[i][0],
	//				   lim_csum1[i][1],"CSUM1");
	char condname[40];
	
	sprintf(condname,"TPC/CSUM1%s",tpc_name_ext1[i]);
	cTPC_CSUM[i][0]=MakeWinCond(condname,lim_csum1[i][0],lim_csum1[i][1],"CSUM1");
	
	sprintf(condname,"CSUM2%s",tpc_name_ext1[i]);
	cTPC_CSUM[i][1]=MakeWindowCond("TPC/CSUM2",condname,lim_csum2[i][0],lim_csum2[i][1],"CSUM2");
	
	sprintf(condname,"CSUM3%s",tpc_name_ext1[i]);
	cTPC_CSUM[i][2]=MakeWindowCond("TPC/CSUM3",condname,lim_csum3[i][0],lim_csum3[i][1],"CSUM3");
	
	sprintf(condname,"CSUM4%s",tpc_name_ext1[i]);
	cTPC_CSUM[i][3]=MakeWindowCond("TPC/CSUM4",condname,lim_csum4[i][0],lim_csum4[i][1],"CSUM4");
    }
    
      for(int i=0;i<13;i++){  // up to MW31
       //up to MWB2 (09.07.2018)

      char condname[40];
      sprintf(condname,"XSUM%s",mw_name_ext[i]);
      cMW_XSUM[i] = MakeWindowCond("MW/XSUM", condname, lim_xsum[i][0], lim_xsum[i][1], MW_XSUM);

      sprintf(condname,"YSUM%s",mw_name_ext[i]);
      cMW_YSUM[i] = MakeWindowCond("MW/YSUM", condname, lim_ysum[i][0], lim_ysum[i][1], MW_YSUM);
    }*/
    
    /*** MUSIC Conditions ***/
    
    cMusic1_E = new Float_t*[8];
    cMusic1_T = new Float_t*[8];
    cMusic2_E = new Float_t*[8];
    cMusic2_T = new Float_t*[8];
    cMusic3_T = new Float_t*[4];
    cMusic3_E = new Float_t*[4];

    cMusic3_dec = new Float_t[2];
    
    for(int i = 0; i < 8; ++i){
	
	
	cMusic1_E[i] = new Float_t[2];
	cMusic1_T[i] = new Float_t[2];
	cMusic2_E[i] = new Float_t[2];
	cMusic2_T[i] = new Float_t[2];

	if(i < 4){
	    cMusic3_E[i] = new Float_t[2];
	    cMusic3_T[i] = new Float_t[2];
	}
	
    }
    
    
    line_number = 0;
    
    format = "%f %f %f %f";

     ifstream cond_d("Configuration_Files/FRS_Window_Conditions/MUSIC1.txt");
    
    while(/*cond_d.good()*/getline(cond_d,line,'\n')){
	
	//getline(cond_d,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cMusic1_E[line_number][0],&cMusic1_E[line_number][1],&cMusic1_T[line_number][0],&cMusic1_T[line_number][1]);
					
	line_number++;
    }

    line_number = 0;
    
    format = "%f %f %f %f";

     ifstream cond_e("Configuration_Files/FRS_Window_Conditions/MUSIC2.txt");
    
    while(/*cond_e.good()*/getline(cond_e,line,'\n')){
	
	//getline(cond_e,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cMusic2_E[line_number][0],&cMusic2_E[line_number][1],&cMusic2_T[line_number][0],&cMusic2_T[line_number][1]);
					
	line_number++;
    }

    line_number = 0;
    
    format = "%f %f %f %f";

     ifstream cond_f("Configuration_Files/FRS_Window_Conditions/MUSIC3.txt");
    
    while(/*cond_f.good()*/getline(cond_f,line,'\n')){
	
	//getline(cond_f,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cMusic3_E[line_number][0],&cMusic3_E[line_number][1],&cMusic3_T[line_number][0],&cMusic3_T[line_number][1]);
					
	line_number++;
    }

    
    line_number = 0;
    
    format = "%f %f";

     ifstream cond_g("Configuration_Files/FRS_Window_Conditions/MUSIC_dEc3.txt");
    
    while(/*cond_g.good()*/getline(cond_g,line,'\n')){
	
	//getline(cond_g,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cMusic3_dec[0],&cMusic3_dec[1]);
    }

    /***SCINTILATOR Condtions***/
    
    cSCI_L = new Float_t[2];
    cSCI_R = new Float_t[2];
    cSCI_E = new Float_t[2];
    cSCI_Tx = new Float_t[2];
    cSCI_X = new Float_t[2];
    
    
    line_number = 0;
    
    format = "%f %f";

     ifstream cond_h("Configuration_Files/FRS_Window_Conditions/SCI_Cons.txt");
    
    while(/*cond_h.good()*/getline(cond_h,line,'\n')){
	
	//getline(cond_h,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cSCI_L[0],&cSCI_L[1]);
	    
	getline(cond_h,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_R[0],&cSCI_R[1]);
	    
	getline(cond_h,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_E[0],&cSCI_E[1]);
	    
	getline(cond_h,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_Tx[0],&cSCI_Tx[1]);
	    
	getline(cond_h,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_X[0],&cSCI_X[1]);
	    
    }
    
    cSCI_LL2 = new Float_t[2];
    cSCI_RR2 = new Float_t[2];
    cSCI_LL3 = new Float_t[2];
    cSCI_RR3 = new Float_t[2];
    cSCI_LL4 = new Float_t[2];
    cSCI_RR4 = new Float_t[2];
    
    format = "%f %f";

    ifstream cond_i("Configuration_Files/FRS_Window_Conditions/SCI_LLRR.txt");

    while(/*cond_i.good()*/getline(cond_i,line,'\n')){
	
	
	//getline(cond_i,line,'\n');
	if(line[0] == '#') continue;	
	    sscanf(line.c_str(),format,&cSCI_LL2[0],&cSCI_LL2[1]);
	getline(cond_i,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_RR2[0],&cSCI_RR2[1]);
	    
	getline(cond_i,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_LL3[0],&cSCI_LL3[1]);
	    
	getline(cond_i,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_RR3[0],&cSCI_RR3[1]);
	    
	    
	getline(cond_i,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_LL4[0],&cSCI_LL4[1]);
	    
	getline(cond_i,line,'\n');
	    sscanf(line.c_str(),format,&cSCI_RR4[0],&cSCI_RR4[1]);
	    
    }
    
    cSCI_detof = new Float_t*[5];
    
    for(int i = 0; i < 5; ++i){
	
	cSCI_detof[i] = new Float_t[2];
	
    }
    
    line_number = 0;
    
    format = "%f %f";

     ifstream cond_j("Configuration_Files/FRS_Window_Conditions/SCI_dEToF.txt");
    
    while(/*cond_j.good()*/getline(cond_j,line,'\n')){
	
	//getline(cond_j,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cSCI_detof[line_number][0],&cSCI_detof[line_number][1]);
					
	line_number++;
    }

    /***ID Condtions***/
    
    cID_x2 = new Float_t[2];
    cID_x4 = new Float_t[2];
    cID_Z_Z = new Float_t[2];
    
    format = "%f %f";

     ifstream cond_k("Configuration_Files/FRS_Window_Conditions/ID_x2.txt");


    while(/*cond_k.good()*/getline(cond_k,line,'\n')){
	
	//getline(cond_k,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cID_x2[0],&cID_x2[1]);
	    
    }
    
     ifstream cond_l("Configuration_Files/FRS_Window_Conditions/ID_x4.txt");

    while(/*cond_l.good()*/getline(cond_l,line,'\n')){
	
	//getline(cond_l,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cID_x4[0],&cID_x4[1]);
	    
    }
    
     ifstream cond_m("Configuration_Files/FRS_Window_Conditions/ID_Z_Z.txt");

    while(/*cond_m.good()*/getline(cond_m,line,'\n')){
	
	//getline(cond_m,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cID_Z_Z[0],&cID_Z_Z[1]);
	    
    }
    
    
    cID_x4AoQ_Z = new Float_t*[5];
    
    for(int i = 0; i < 5; ++i){
	
	cID_x4AoQ_Z[i] = new Float_t[2];
	
    }
    
    
    line_number = 0;

     ifstream cond_n("Configuration_Files/FRS_Window_Conditions/ID_Z_Z.txt");

    while(/*cond_n.good()*/getline(cond_n,line,'\n')){
	
	//getline(cond_n,line,'\n');
	if(line[0] == '#') continue;
	    sscanf(line.c_str(),format,&cID_x4AoQ_Z[line_number][0],&cID_x4AoQ_Z[line_number][1]);
	    
	line_number++;

    }
    
    cID_x2AoQ = new Float_t**[6];
    cID_Z_AoQ = new Float_t**[5];
    
    for(int i = 0; i < 6; ++i){
	
	cID_x2AoQ[i] = new Float_t*[6];
	if (i < 5) cID_Z_AoQ[i] = new Float_t*[6];
	
	    for(int j = 0; j < 6; ++j){
		
		cID_x2AoQ[i][j] = new Float_t[2];
		if (i < 5) cID_Z_AoQ[i][j] = new Float_t[2];
		
	    }
	
    }
    
    line_number = 0;
    int selection_number = 0;

     ifstream cond_o("Configuration_Files/FRS_Window_Conditions/ID_x2AoQ.txt");

    while(/*cond_o.good()*/getline(cond_o,line,'\n')){
	
	//getline(cond_o,line,'\n');
	if(line[0] == '#') continue;
	if(line[0] == '&'){
	    selection_number++;
	    line_number = 0;
	    continue;
	}
	
	    sscanf(line.c_str(),format,&cID_x2AoQ[selection_number][line_number][0],&cID_x2AoQ[selection_number][line_number][1]);
	    
	line_number++;

    }
    
    
    line_number = 0;
    selection_number = 0;

     ifstream cond_p("Configuration_Files/FRS_Window_Conditions/ID_Z_AoQ.txt");

    while(/*cond_p.good()*/getline(cond_p,line,'\n')){
	
	//getline(cond_p,line,'\n');
	if(line[0] == '#') continue;
	if(line[0] == '&'){
	    selection_number++;
	    line_number = 0;
	    continue;
	}
	    sscanf(line.c_str(),format,&cID_Z_AoQ[selection_number][line_number][0],&cID_Z_AoQ[selection_number][line_number][1]);
	    
	line_number++;

    }
    
    cID_dEToF = new Float_t*[5];

    for(int i = 0; i < 5; ++i){
	
	cID_dEToF[i] = new Float_t[2];
	
    }
    line_number = 0;
    selection_number = 0;

     ifstream cond_q("Configuration_Files/FRS_Window_Conditions/ID_dEToF.txt");

    while(/*cond_q.good()*/getline(cond_q,line,'\n')){
	
	//getline(cond_p,line,'\n');
	if(line[0] == '#') continue;

	    sscanf(line.c_str(),format,&cID_dEToF[line_number][0],&cID_dEToF[line_number][1]);
	    
	line_number++;

    }
    
    
    /*for(int i=0;i<8;i++){
	
       sprintf(name,"Music1_E(%f)",i);
       cMusic1_E[i] = MakeWindowCond("MUSIC/MUSIC(1)/E",name, 10, 4086, "hMUSIC1_E");

        sprintf(name,"Music1_T(%d)",i);
       cMusic1_T[i] = MakeWindowCond("MUSIC/MUSIC(1)/T",name, 10, 4086,"hMUSIC1_T");

       sprintf(name,"Music2_E(%d)",i);
       cMusic2_E[i] = MakeWindowCond("MUSIC/MUSIC(2)/E",name, 10, 4086, "hMUSIC2_E");

       sprintf(name,"Music2_T(%d)",i);
       cMusic2_T[i] = MakeWindowCond("MUSIC/MUSIC(2)/T",name, 10, 4086, "hMUSIC2_T");
     }

   for(int i=0;i<4;i++){
       
       sprintf(name,"Music3_E(%d)",i);
       cMusic3_E[i] = MakeWindowCond("MUSIC/MUSIC(3)/E",name, 10, 4086, "hMUSIC3_E");

       sprintf(name,"Music3_T(%d)",i);
       cMusic3_T[i] = MakeWindowCond("MUSIC/MUSIC(3)/T",name, 10, 4086, "hMUSIC3_T");
    }
   
   
    cMusic3_dec = MakeWindowCond("MUSIC/MUSIC 3","Music3_dec", 10., 4000., "hMUSIC3_dECOR");*/
    
    
    
    
    
}


