#ifndef TXRSUSERPROCESSOR_H
#define TXRSUSERPROCESSOR_H

#include "TXRSBasicProc.h"
#include "TXRSAnlEvent.h"
#include "TXRSParameter.h"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"

class TXRSUserEvent;
class TXRSSortEvent;
class TXRSCalibrEvent;
class TXRSAnlEvent;

class TXRSUserProc : public TXRSBasicProc {
public:
  TXRSUserProc() ;
  TXRSUserProc(const char* name);
  virtual ~TXRSUserProc() ;

  void FRSUserProc(TXRSUserEvent* tgt);

  
protected:   
  //void Create_ROOT_Tree_Data();
  void CopyPrevious(TXRSAnlEvent &anl) ;
  void Process_Analysis(TXRSSortEvent &srt, TXRSCalibrEvent &cal, TXRSAnlEvent &anl, TXRSUserEvent &tgt);

  TXRSAnlEvent previous;

  TModParameter* Setup ;

  // TFile *evtFile;
	
  // TTree* allEvtTree;
  // TTree* impTree;
  // TTree* decTree;


  ClassDef(TXRSUserProc,1)
};

#endif //TXRSUSERPROCESSOR_H
