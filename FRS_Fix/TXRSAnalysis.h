#ifndef TXRSANALYSIS_H
#define TXRSANALYSIS_H

#include "TGo4Analysis.h"

#include "TXRSParameter.h"

class TH1;
class TH2;

class TGo4MbsEvent;
class TGo4WinCond;
class TGo4PolyCond;

// class TXRSParameter;
// class TMWParameter;
// class TTPCParameter;
// class TMUSICParameter;
// class TSCIParameter;
// class TIDParameter;
// class TSIParameter;

class TXRSAnalysis : public TGo4Analysis {
public:   
  TXRSAnalysis();
  TXRSAnalysis(const char* lmd, 
	       const char* asf=0,
	       const char* raw=0, 
	       const char* sort=0, 
	       const char* cal=0, 
	       const char* anal=0, 
	       const char* user=0);
  virtual ~TXRSAnalysis() ;
      
  virtual Bool_t InitEventClasses();
      
  virtual Int_t UserPreLoop();
  virtual Int_t UserEventFunc();
  virtual Int_t UserPostLoop();
      
  TH1* SetupH1(const char* histoname,
	       Int_t nbinsx, Float_t xmin, Float_t xmax, 
	       const char* xtitle = 0);

  TH2* SetupH2(const char* histoname,
	       Int_t nbinsx, Float_t xmin, Float_t xmax, 
	       Int_t nbinsy, Float_t ymin, Float_t ymax, 
	       const char* xtitle = 0, const char* ytitle = 0);
                  
  TGo4WinCond* SetupWinCond(const char* condname,
			    float left, float right,
			    Bool_t enable = kTRUE, 
			    Bool_t alwaysreturn = kTRUE);
                                
  TGo4PolyCond* SetupPolyCond(const char* condname,
			      Int_t size,
			      void* _points,   //  Float_t (*points)[2],
			      Bool_t enable = kTRUE, 
			      Bool_t alwaysreturn = kTRUE);

  void AssignCond(const char* condname, const char* HistoName);
         
private:      
  TGo4MbsEvent          *fMbsEvent;  //!
         
  TXRSParameter         *fFRSPar;    //!
  TMWParameter          *fMWPar;     //!
  TTPCParameter         *fTPCPar;     //!
  TMUSICParameter       *fMUSICPar;  //!
  TSCIParameter         *fSCIPar;    //!
  TIDParameter          *fIDPar;     //!
  TSIParameter          *fSIPar;     //!
  TModParameter * ModPar; //!
  TMRTOFMSParameter* MRtofPar; //!

  
  TH1I                  *fSize;      //!
  Int_t                 fEvents;     //!
  Int_t                 fLastEvent;  //!

  ClassDef(TXRSAnalysis,1)
};
#endif //TXRSANALYSIS_H

