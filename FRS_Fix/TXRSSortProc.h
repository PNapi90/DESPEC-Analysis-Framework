#ifndef TXRSSORTPROCESSOR_H
#define TXRSSORTPROCESSOR_H

#include "TXRSBasicProc.h"

class TXRSSortEvent;

class TXRSSortProc : public TXRSBasicProc {
public:
  TXRSSortProc() ;
  TXRSSortProc(const char* name);
  virtual ~TXRSSortProc() ;

  Bool_t BuildEvent(TGo4EventElement* output);
  //Bool_t FillHistograms(TXRSSortEvent* event);
   
  //void FRSSort(TXRSSortEvent* tgt);
      
protected:
  Long64_t StartOfSpilTime;
  Long64_t StartOfSpilTime2; //does not reset at end of extraction
  Long64_t PreviousTS;
  
  Int_t counter;


  ClassDef(TXRSSortProc,1)
};

#endif //TXRSSORTPROCESSOR_H

