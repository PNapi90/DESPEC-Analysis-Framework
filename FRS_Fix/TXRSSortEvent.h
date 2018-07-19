#ifndef TXRSSORTEVENT_H
#define TXRSSORTEVENT_H

#include "TGo4EventElement.h"


class TXRSSortProc;
class TGo4FileSource;

class TXRSSortEvent : public TGo4EventElement {
public:
  TXRSSortEvent();
  TXRSSortEvent(const char* name);
  virtual ~TXRSSortEvent();
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
  virtual void Clear(Option_t *t="");  
      
  virtual Int_t Init();  
      
      
  // MON part
  Int_t EventFlag;
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
  
                  

private:
  // TXRSSortProc   *fxProc;     //! This is processor
  // TGo4FileSource *fxFileSrc;  //! This is file source
      
  ClassDef(TXRSSortEvent,1)         
};

#endif //TXRSSORTEVENT_H

