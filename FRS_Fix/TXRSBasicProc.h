#ifndef TXRSBASICPROCESSOR_H
#define TXRSBASICPROCESSOR_H

#include "TGo4EventProcessor.h"

#include "Gtypes.h"

class TH1I;
class TH2I;
class TGo4Picture;
class TGo4WinCond;
class TGo4CondArray;
class TGo4PolyCond;

/**
 * This class used as base class for all event processors in FRS analysis.
 * It's only aim - simplify and unify way to create histograms, pictures 
 * and conditions in event processors. Class introduce following functions:
 *     MakeH1I()        - create 1-dim histogram
 *     MakeH2I()        - create 2-dim histogram
 *     MakeWindowCond() - create simple 1-dim window condition
 *     MakePolyCond()   - create polygon condition
 *     MakePic()        - create picture
 *  All these functions works in following way:
 *     At the first time, when any object is created, these functions tests 
 *  existance of object of the same name in autosave file. If any, object
 *  from autosave file will be returned. Also following objects will be taken 
 *  from autosave file.
 *     If objects does not exist in autosave file, when new object will be created 
 *  with given parameters. In this case function ObjWasCreated() return kTRUE.
 *     As a return value of all these functions, pointer on appropriate 
 *  object will be returned
 *     Normally all these functions should be used in constructor of event processor only. 
 */

class TXRSBasicProc : public TGo4EventProcessor {
public:
  TXRSBasicProc() ;
  TXRSBasicProc(const char* name);
  virtual ~TXRSBasicProc();
protected:
  TH1I* MakeH1I(const char* foldername, 
		const char* histoname,
		Int_t nbins, 
		Float_t xmin, Float_t xmax, 
		const char* xtitle = "channels",
		Color_t linecolor = 2,
		Color_t fillcolor = 6,
		const char* ytitle = 0);
                    
  TH2I* MakeH2I(const char* foldername, 
		const char* histoname,
		Int_t nbinsx, Float_t xmin, Float_t xmax, 
		Int_t nbinsy, Float_t ymin, Float_t ymax, 
		const char* xtitle, const char* ytitle,
		Color_t marker);

  TH1F* MakeH1F(const char* foldername, 
		const char* histoname,
		Int_t nbins, 
		Float_t xmin, Float_t xmax, 
		const char* xtitle = "channels",
		Color_t linecolor = 2,
		Color_t fillcolor = 6,
		const char* ytitle = 0);

  TH1I* MakeH1I_MW(const char* foldername, const char* name, int nameindex,
		   Int_t nbins, Float_t xmin, Float_t xmax, 
		   const char* xtitle = "channels", Color_t linecolor = 2, Color_t fillcolor = 6);

  TH1I* MakeH1I_TPC(const char* foldername, const char* name, int nameindex,
		    Int_t nbins, Float_t xmin, Float_t xmax, 
		    const char* xtitle = "channels", Color_t linecolor = 2, Color_t fillcolor = 6);

  // for c++11
  // const char* mw_name_ext[13]={"(1:1)", "(2:1)", "(2:2)", "(3:1)",
  // 			       "(4:1)", "(4:2)", "(5:1)", "(6:1)",
  // 			       "(7:1)", "(8:1)", "(8:2)", "(9:1)", "(10:1)"};

  // const char* mw_folder_ext1[13]={"1", "2", "2", "3","4", "4", "5", "6","7", "8","8", "B", "B"};
  // const char* mw_folder_ext2[13]={"11", "21", "22", "31","41", "42", "51", "61","71", "81", "82", "B21", "B22"};
  
  // const char* tpc_name_ext1[6]={"TPC1_","TPC2_","TPC3_","TPC4_","TPC5_","TPC6_"};
  // const char* tpc_folder_ext1[6]={"TPC1","TPC2","TPC3","TPC4","TPC5","TPC6"};


  const static char* mw_name_ext[13];
  const static char* mw_folder_ext1[13];
  const static char* mw_folder_ext2[13];
  const static char* tpc_name_ext1[7];
  const static char* tpc_folder_ext1[7];

  // TH1I* MakeH1I_ITAG(const char* foldername, const char* name, int nameindex,
  // 		     Int_t nbins, Float_t xmin, Float_t xmax, 
  // 		     const char* xtitle = "channels", Color_t linecolor = 2, Color_t fillcolor = 6);

  
  TH1I* MakeH1ISeries(const char* foldername, Int_t seriesnumber, Int_t crate, Int_t number, Bool_t remove);			     
  TH1I* MakeH1ISeries3(const char* foldername, Int_t seriesnumber, Int_t crate, Int_t number, Bool_t remove);	 
  TGo4Picture* MakeSeriesPicture(const char* foldername, Int_t seriesnumber, Bool_t remove);              


  TGo4WinCond* MakeWindowCond(const char* foldername,
			      const char* condname,
			      float left = 0.,
			      float right = 4096.,
			      const char* HistoName = 0);
                                 
  TGo4PolyCond* MakePolyCond(const char* foldername,
			     const char* condname,
			     Int_t size,
			     Float_t (*points)[2],
			     const char* HistoName = 0);
                                 
  TGo4CondArray* MakeCondArray(const char* foldername,
			       const char* condname,
			       Int_t size,
			       const char* condclassname = "TGo4WinCond");
                                   
  void AssignCond(const char* condname, const char* HistoName);
                                  
  TGo4Picture* MakePic(const char* foldername,
		       const char* picname,
		       Int_t ndivy, Int_t ndivx, 
		       TObject* obj0 = 0,
		       TObject* obj1 = 0,
		       TObject* obj2 = 0,
		       TObject* obj3 = 0,
		       TObject* obj4 = 0,
		       TObject* obj5 = 0,
		       TObject* obj6 = 0,
		       TObject* obj7 = 0,
		       TObject* obj8 = 0,
		       TObject* obj9 = 0);
                          
  TGo4Picture* MakePicCond(const char* foldername,
			   const char* picname,
			   Int_t ndivy, Int_t ndivx, 
			   TObject* obj0 = 0, TGo4Condition* cond0 = 0,
			   TObject* obj1 = 0, TGo4Condition* cond1 = 0,
			   TObject* obj2 = 0, TGo4Condition* cond2 = 0,
			   TObject* obj3 = 0, TGo4Condition* cond3 = 0,
			   TObject* obj4 = 0, TGo4Condition* cond4 = 0,
			   TObject* obj5 = 0, TGo4Condition* cond5 = 0,
			   TObject* obj6 = 0, TGo4Condition* cond6 = 0,
			   TObject* obj7 = 0, TGo4Condition* cond7 = 0,
			   TObject* obj8 = 0, TGo4Condition* cond8 = 0);
                           
  Bool_t ObjWasCreated() const { return fbObjWasCreated; }

  
private:           
  typedef TNamed* (TXRSBasicProc::*getfunc)(const Text_t*);
      
  TNamed* TestObject(getfunc func, const char* fname, const char* name);
   
  Bool_t fbTestAutoSaveFile;  //!
  Bool_t fbUseAutoSaveFile;   //!
  Bool_t fbObjWasCreated;     //!

  
  ClassDef(TXRSBasicProc,1)
};

#endif //TXRSBASICPROCESSOR_H

