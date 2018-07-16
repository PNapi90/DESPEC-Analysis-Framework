#ifndef TFRSSORTPROCESSOR_H
#define TFRSSORTPROCESSOR_H

#include "TFRSBasicProc.h"

class TFRSSortEvent;

class TFRSSortProc : public TFRSBasicProc {
public:
  TFRSSortProc() ;
  TFRSSortProc(const char* name);
  virtual ~TFRSSortProc() ;

  Bool_t BuildEvent(TGo4EventElement* output);
  //Bool_t FillHistograms(TFRSSortEvent* event);
   
  //void FRSSort(TFRSSortEvent* tgt);
      
protected:
  Long64_t StartOfSpilTime;
  Long64_t StartOfSpilTime2; //does not reset at end of extraction
  Long64_t PreviousTS;
  
  Int_t counter;


  ClassDef(TFRSSortProc,1)
};

#endif //TFRSSORTPROCESSOR_H

