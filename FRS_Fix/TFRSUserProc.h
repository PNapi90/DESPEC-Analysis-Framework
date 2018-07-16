#ifndef TFRSUSERPROCESSOR_H
#define TFRSUSERPROCESSOR_H

#include "TFRSBasicProc.h"
#include "TFRSAnlEvent.h"
#include "TFRSParameter.h"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"

class TFRSUserEvent;
class TFRSSortEvent;
class TFRSCalibrEvent;
class TFRSAnlEvent;

class TFRSUserProc : public TFRSBasicProc {
public:
  TFRSUserProc() ;
  TFRSUserProc(const char* name);
  virtual ~TFRSUserProc() ;

  void FRSUserProc(TFRSUserEvent* tgt);

  
protected:   
  //void Create_ROOT_Tree_Data();
  void CopyPrevious(TFRSAnlEvent &anl) ;
  void Process_Analysis(TFRSSortEvent &srt, TFRSCalibrEvent &cal, TFRSAnlEvent &anl, TFRSUserEvent &tgt);

  TFRSAnlEvent previous;

  TModParameter* Setup ;

  // TFile *evtFile;
	
  // TTree* allEvtTree;
  // TTree* impTree;
  // TTree* decTree;


  ClassDef(TFRSUserProc,1)
};

#endif //TFRSUSERPROCESSOR_H
