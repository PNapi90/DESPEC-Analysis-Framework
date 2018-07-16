#ifndef TFRSUNPACKPROCESSOR_H
#define TFRSUNPACKPROCESSOR_H

#include "TFRSBasicProc.h"
#include "TFRSUnpackEvent.h"
#include "TFRSParameter.h"

class TFRSUnpackEvent;
class TGo4MbsSubEvent;

class TFRSUnpackProc : public TFRSBasicProc {
public:
  TFRSUnpackProc() ;
  TFRSUnpackProc(const char* name);
  virtual ~TFRSUnpackProc();
   
  //void FRSUnpack(TFRSUnpackEvent* tgt);
  // event processing function, default name
  Bool_t BuildEvent(TGo4EventElement* output);
  Bool_t FillHistograms(TFRSUnpackEvent* event);

private:

  Bool_t Event_Extract(TFRSUnpackEvent* event_out, TGo4MbsSubEvent* rawsub, int type_event=1);
#ifdef USELESS
  void UnpackUserSubevent(TGo4MbsSubEvent* psubevt, TFRSUnpackEvent* tgt);
#endif
  TFRSParameter* frs ;
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
  
  ClassDef(TFRSUnpackProc,1)
};

#endif //TFRSUNPACKPROCESSOR_H

