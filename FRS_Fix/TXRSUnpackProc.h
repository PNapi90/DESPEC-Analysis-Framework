#ifndef TXRSUNPACKPROCESSOR_H
#define TXRSUNPACKPROCESSOR_H

#include "TXRSBasicProc.h"
#include "TXRSUnpackEvent.h"
#include "TXRSParameter.h"

class TXRSUnpackEvent;
class TGo4MbsSubEvent;

class TXRSUnpackProc : public TXRSBasicProc {
public:
  TXRSUnpackProc() ;
  TXRSUnpackProc(const char* name);
  virtual ~TXRSUnpackProc();
   
  //void FRSUnpack(TXRSUnpackEvent* tgt);
  // event processing function, default name
  Bool_t BuildEvent(TGo4EventElement* output);
  Bool_t FillHistograms(TXRSUnpackEvent* event);

private:

  Bool_t Event_Extract(TXRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
#ifdef USELESS
  void UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TXRSUnpackEvent* tgt);
#endif
  TXRSParameter* frs ;
  TModParameter* ModSetup;
    
  TH1I* hTrigger;         //!
        	
  TH1I* hVME0_8[32];      //! FRS crate
  TH1I* hVME0_9[32];      //!
  TH1I* hVME0_11[32];     //!
  TH1I* hVME0_12[32];     //!
  TH1I* hVME0_13[32];     //!
  TH1I* hVME0_14[32];     //!
        	
  TH1I* hVME1_8[32];      //! TPC User crate
  TH1I* hVME1_9[32];      //!
  TH1I* hVME1_15[32];     //!
  TH1I* hVME1_3[32];     //!

  TH1I* hVME5_11[32];   //Main Crate or User Crate
        	
  TH1I* hVME2_TDC[32]; //!
  TH1I* hVME3_TDC[32]; //!

  TH2I* h_VME0_8All ;  //!
  TH2I* h_VME0_9All ;  //!
  TH2I* h_VME0_11All;  //!
  TH2I* h_VME0_12All;  //!
  TH2I* h_VME1_8All ;  //!
  TH2I* h_VME1_9All ;  //!
  TH2I* h_VME1_3All ;  //!
  TH2I* h_VME1_15All;  //!
  TH2I* hVME2_TDCAll;  //!
  TH2I* hVME3_TDCAll;  //!
  TH2I* h_UnpackStatus; //!
  TH2I* h_TSFlagStatus; //!
  TH2I* h_Multiplicity; //!
  
  bool firstTS[3] ;
  Long64_t previousTimeStamp[3];
  Long64_t currentTimeStamp;
  
  ClassDef(TXRSUnpackProc,1)
};

#endif //TXRSUNPACKPROCESSOR_H

