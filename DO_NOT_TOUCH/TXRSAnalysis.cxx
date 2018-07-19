#include "TXRSAnalysis.h"

#include "Riostream.h"

#include "TH1.h"
#include "TH2.h"

#include "TGo4MbsEvent.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"

#include "TGo4StepFactory.h"

#include "TXRSParameter.h"

#include "TXRSCalibrProc.h"
#include "TXRSAnlProc.h"

#include <sstream>
#include <fstream>

//***********************************************************
TXRSAnalysis::TXRSAnalysis() 
  : fMbsEvent(0), fSize(0), fEvents(0), fLastEvent(0) 
{
  std::cout << "Wrong constructor TXRSAnalysis()!" << std::endl;
}

//TCanvas* gPrintCanvas = 0;

//***********************************************************
// this constructor is used
TXRSAnalysis::TXRSAnalysis(const char* lmd,
			   const char* asf,
                           const char* out1, 
                           const char* out2, 
                           const char* out3, 
                           const char* out4, 
                           const char* out5) 
  : fMbsEvent(0), fSize(0), fEvents(0), fLastEvent(0) { 


  //   gPrintCanvas = new TCanvas("can1","can1 title",500,500);



  // lmd: input file name (*.lmd)
  // out1: output file name of first analysis step  (*.root)
  // out2: output file name of second analysis step (*.root)
  std::cout << "**** TXRSAnalysis: Create ****" << std::endl;

  // Enable autosave. Autosave is done when analysis loop is stopped
  // or intervall os over.
  SetAutoSave(kFALSE);   // no autosave
  SetAutoSaveFile(asf);   // optional
  SetAutoSaveInterval(100000); // events , 0 = termination of event loop

  // the step definitions can be changed in the GUI
  // first step definitions:
  // the name of the step can be used later to get event objects   
  //TXRSUnpackFact*         factory1 = new TXRSUnpackFact("Unpack-factory");
   
  TGo4StepFactory* factory1 = new TGo4StepFactory("Unpack-factory");
  factory1->DefEventProcessor("FRSUnpackProc","TXRSUnpackProc"); 
  factory1->DefOutputEvent("FRSUnpackEvent", "TXRSUnpackEvent");   
   
  TGo4MbsFileParameter*   source1  = new TGo4MbsFileParameter(lmd);
  TGo4FileStoreParameter* store1   = new TGo4FileStoreParameter(out1);
  TGo4AnalysisStep*       step1    = new TGo4AnalysisStep("Unpack",factory1,source1,store1,0);
  store1->SetOverwriteMode(kTRUE);
  step1->SetSourceEnabled(kTRUE);
  step1->SetStoreEnabled(kFALSE);  // dissable output
  step1->SetProcessEnabled(kTRUE);
  step1->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step1);


  // second step definitions:
  // the name of the step can be used later to get event objects   
  TGo4StepFactory* factory2  = new TGo4StepFactory("Sort-factory");
  factory2->DefInputEvent("FRSUnpackEvent", "TXRSUnpackEvent");   
  factory2->DefEventProcessor("FRSSortProc","TXRSSortProc"); 
  factory2->DefOutputEvent("FRSSortEvent", "TXRSSortEvent");   
   
  TGo4FileSourceParameter* source2  = new TGo4FileSourceParameter(out1);
  TGo4FileStoreParameter*  store2   = new TGo4FileStoreParameter(out2);
  TGo4AnalysisStep*       step2    = new TGo4AnalysisStep("Sort",factory2,source2,store2,0);
  store2->SetOverwriteMode(kTRUE);
  step2->SetSourceEnabled(kFALSE); // disable file input
  step2->SetStoreEnabled(kFALSE);  // dissable output
  step2->SetProcessEnabled(kTRUE);
  step2->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step2);


  // third step definitions:
  // the name of the step can be used later to get event objects   
  TGo4StepFactory* factory3  = new TGo4StepFactory("Calibr-factory");
  factory3->DefInputEvent("FRSSortEvent", "TXRSSortEvent");   
  factory3->DefEventProcessor("FRSCalibrProc","TXRSCalibrProc"); 
  factory3->DefOutputEvent("FRSCalibrEvent", "TXRSCalibrEvent");   
  TGo4FileSourceParameter* source3  = new TGo4FileSourceParameter(out2);
  TGo4FileStoreParameter*  store3   = new TGo4FileStoreParameter(out3);
  TGo4AnalysisStep*       step3    = new TGo4AnalysisStep("Calibr",factory3,source3,store3,0);
  store3->SetOverwriteMode(kTRUE);
  step3->SetSourceEnabled(kFALSE); // disable file input
  step3->SetStoreEnabled(kFALSE);  // dissable output
  step3->SetProcessEnabled(kTRUE);
  step3->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step3);

   
  // forth step definition
  // step definitions: FRS - Analysis
  TGo4StepFactory* factory4 = new TGo4StepFactory("Anlalysis-factory");
  factory4->DefInputEvent("FRSCalibrEvent", "TXRSCalibrEvent");   
  factory4->DefEventProcessor("FRSAnlProc","TXRSAnlProc"); 
  factory4->DefOutputEvent("FRSAnlEvent", "TXRSAnlEvent");   
  TGo4FileSourceParameter* source4  = new TGo4FileSourceParameter(out3);
  TGo4FileStoreParameter*  store4   = new TGo4FileStoreParameter(out4);
  TGo4AnalysisStep*        step4    = new   TGo4AnalysisStep("Analysis",factory4,source4,store4,0);  
  store4->SetOverwriteMode(kTRUE);   
  step4->SetSourceEnabled(kFALSE); // disable file input
  step4->SetStoreEnabled(kFALSE);  // disable output   
  step4->SetProcessEnabled(kTRUE);
  step4->SetErrorStopEnabled(kTRUE);
  AddAnalysisStep(step4);
   

  // fifth step definition
  // step definitions: FRS - Analysis
  TGo4StepFactory* factory5 = new TGo4StepFactory("User-factory");
  factory5->DefInputEvent("FRSAnlEvent", "TXRSAnlEvent");   
  factory5->DefEventProcessor("FRSUserProc","TXRSUserProc"); 
  factory5->DefOutputEvent("FRSUserEvent", "TXRSUserEvent");   
  TGo4FileSourceParameter* source5  = new TGo4FileSourceParameter(out4);
  TGo4FileStoreParameter*  store5   = new TGo4FileStoreParameter(out5);
  TGo4AnalysisStep*        step5    = new TGo4AnalysisStep("User",factory5,source5,store5,0);  
  store5->SetOverwriteMode(kTRUE);   
  step5->SetSourceEnabled(kFALSE); // disable file input
  step5->SetStoreEnabled(kFALSE);  // disable output   
  step5->SetProcessEnabled(kFALSE);
  step5->SetErrorStopEnabled(kFALSE);
  AddAnalysisStep(step5);

   
  //////////////// Parameters //////////////////////////
  // At this point, autosave file has not yet been read!
  // Therefore parameter values set here will be overwritten
  // if an autosave file is there.
  fFRSPar = new TXRSParameter("FRSPar");
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
  AddParameter(MRtofPar);
}

Bool_t TXRSAnalysis::InitEventClasses() 
{
  Bool_t res = TGo4Analysis::InitEventClasses();
  std::string nameExperiment;
  std::ifstream ifs ( "config.txt" );
  if(ifs.is_open())
    {
      const std::string CommentSymbol("#");
      const std::string ExpNameSymbol("NameExperiment");

      std::string temp_line;
      while(std::getline(ifs,temp_line))
	{
	  std::stringstream stream(temp_line);
	  std::string testComment(stream.str());
	  std::size_t it_comment = testComment.find(CommentSymbol);
	  if(it_comment!=std::string::npos)
	    {
	      //std::cout<<"!> Skip comment"<<temp_line<<std::endl;
	      continue;
	    }
	  std::string key, value;
	  stream >> key >> value ;
	  if(key==ExpNameSymbol)
	    nameExperiment=value;
	}
    }
  std::string nameSetupFile ("setup/setup_July_2018");
  nameSetupFile+=nameExperiment;
  nameSetupFile+=".C";

  std::cout << "TXRSAnalysis::Call "<< nameSetupFile<<" script" << std::endl;
  
  std::ifstream testingFileExist(nameSetupFile.c_str());
  if(testingFileExist.good()==false)
    {
      std::cout<<" E> !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<std::endl;
      std::cout<<" E> Setup file "<<nameSetupFile<<" not found ! Please look to directory ./setup/ if it exits or set proper name of experiment in config.txt"<<std::endl;
      std::cout<<" E> !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! "<<std::endl;

      std::exit(-1);
    }
  
  std::string processL(".x ");
  processL += nameSetupFile;
  gROOT->ProcessLine(processL.c_str());
  ModPar->setMap();
    
  SetupPolyCond("cID_Z_AoQ0", 5, fIDPar->ID_Z_AoverQ[0]);
  SetupPolyCond("cID_Z_AoQ1", 5, fIDPar->ID_Z_AoverQ[1]);
  SetupPolyCond("cID_Z_AoQ2", 5, fIDPar->ID_Z_AoverQ[2]);
  SetupPolyCond("cID_Z_AoQ3", 5, fIDPar->ID_Z_AoverQ[3]);
  SetupPolyCond("cID_Z_AoQ4", 5, fIDPar->ID_Z_AoverQ[4]);

  SetupPolyCond("cID_x2AoQ0", 5, fIDPar->ID_x2AoverQ[0]);
  SetupPolyCond("cID_x2AoQ1", 5, fIDPar->ID_x2AoverQ[1]);
  SetupPolyCond("cID_x2AoQ2", 5, fIDPar->ID_x2AoverQ[2]);
  SetupPolyCond("cID_x2AoQ3", 5, fIDPar->ID_x2AoverQ[3]);
  SetupPolyCond("cID_x2AoQ4", 5, fIDPar->ID_x2AoverQ[4]);
  SetupPolyCond("cID_x2AoQ5", 5, fIDPar->ID_x2AoverQ[5]);

  



  printf("Analysis Name: %s\n",GetName()); 
  return res;
}


TXRSAnalysis::~TXRSAnalysis() 
{
  std::cout << "**** TXRSAnalysis: Delete ****" << std::endl;
}


Int_t TXRSAnalysis::UserPreLoop() 
{ 
  std::cout << "**** TXRSAnalysis: PreLoop ****" << std::endl;
  // we update the pointers to the current event structures here:
  fMbsEvent    = dynamic_cast<TGo4MbsEvent*>   (GetInputEvent("Unpack"));   // of step "Unpack"
  
  fEvents=0;
  fLastEvent=0;

  // create histogram for UserEventFunc
  // At this point, the histogram has been restored from autosave file if any.
  fSize=(TH1I*)GetHistogram("Eventsize");
  if(fSize==0)
    { // no autosave read, create new and register
      fSize = new TH1I ("Eventsize", "Event size[b]",500,0,500);
      fSize->SetLineColor(2);
      fSize->SetFillColor(2);
      AddHistogram(fSize);
    }
  
  // ======= initialization of some event processors
   if (IsClientMode()) printf("go4 found in client mode\n");
  if (IsServerMode()) printf("go4 found in server mode\n");

  TGo4AnalysisStep* step3 = GetAnalysisStep("Calibr");
  if (step3!=0) {
    TXRSCalibrProc* proc3 = dynamic_cast<TXRSCalibrProc*> (step3->GetEventProcessor());
    if (proc3) proc3->InitProcessor();
    if (IsBatchMode() && !IsAutoSaveOn()) {
      printf("Disabled Calibr histograms drawing!\n");
      proc3->bDrawHist=kFALSE;
    }

  }
  TGo4AnalysisStep* step4 = GetAnalysisStep("Analysis");
  if (step4!=0) {
    TXRSAnlProc* proc4 = dynamic_cast<TXRSAnlProc*> (step4->GetEventProcessor());
    if (IsBatchMode() && !IsAutoSaveOn()) {
      printf("Disabled Analysis histograms drawing!\n");
      proc4->bDrawHist=kFALSE;
    }

  }
  
  // ============= run initialization script =========

  return 0;
}

Int_t TXRSAnalysis::UserPostLoop()
{
  std::cout << "**** TXRSAnalysis: PostLoop ****" << std::endl;
  std::cout << "Last event: " << fLastEvent << " Total events: " << fEvents << std::endl;
  fMbsEvent = 0; // reset to avoid invalid pointer if analysis is changed in between
  fEvents=0;

  TGo4AnalysisStep* step3 = GetAnalysisStep("Calibr");
  if (step3!=nullptr)
    {
      TXRSCalibrProc* proc3 = dynamic_cast<TXRSCalibrProc*> (step3->GetEventProcessor());
      if(proc3!=nullptr)
	{//printf("Total SC41 = %d,  Total SC21 = %d, Total SEETRAM = %d \n",check_total_sc41,check_total_sc21,check_total_seetram);
	  std::cout<<"Total SC41 ="<< proc3->check_total_sc41 <<",  Total SC21 ="<< proc3->check_total_sc21  <<" Total SEETRAM ="<< proc3->check_total_seetram
		   <<" Total MRtof Start ="<< proc3->check_total_mrtof_start<<" Total MRtof Stop ="<<proc3->check_total_mrtof_stop
		   <<"\n"; 
	}
    }
  return 0;
}

Int_t TXRSAnalysis::UserEventFunc()
{
  //// This function is called once for each event.
  Int_t value=0;
  Int_t count=0;
  if(fMbsEvent) value = fMbsEvent->GetDlen()/2+2; // total longwords
  fSize->Fill(value); // fill histogram
  fEvents++;
  if(fMbsEvent) count=fMbsEvent->GetCount();
  if(fEvents == 1) std::cout << "First event: " << count  << std::endl;
  fLastEvent=count;

  if (IsBatchMode() && fEvents % 2000 == 0) {
    printf("Event %d\r",fEvents);
    fflush(stdout);
  }


  return 0;
}

TH1* TXRSAnalysis::SetupH1(const char* histoname,
                           Int_t nbinsx, Float_t xmin, Float_t xmax, 
                           const char* xtitle) {
  TH1* histo = dynamic_cast<TH1*> (GetHistogram(histoname));
  if (histo==0) {
    std::cout << "Histogram " << histoname << " did not found" << std::endl;
    return 0;   
  }

  histo->SetBins(nbinsx, xmin, xmax);
  if (xtitle) histo->SetXTitle(xtitle);
  histo->Reset();
   
  return histo;
                               
}

TH2* TXRSAnalysis::SetupH2(const char* histoname,
                           Int_t nbinsx, Float_t xmin, Float_t xmax, 
                           Int_t nbinsy, Float_t ymin, Float_t ymax, 
                           const char* xtitle, const char* ytitle) {
  TH2* histo = dynamic_cast<TH2*> (GetHistogram(histoname));
  if (histo==0) {
    std::cout << "Histogram " << histoname << " did not found" << std::endl;
    return 0;   
  }
   
  histo->SetBins(nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  if (xtitle) histo->SetXTitle(xtitle);
  if (ytitle) histo->SetYTitle(ytitle);
  histo->Reset();
   
  return histo;
}

TGo4WinCond* TXRSAnalysis::SetupWinCond(const char* condname,
                                        float left, float right,
                                        Bool_t enable, 
                                        Bool_t alwaysreturn) {
  TGo4WinCond* cond = dynamic_cast<TGo4WinCond*> (GetAnalysisCondition(condname));
  if (cond==0) {
    std::cout << "Window condition " << condname << " did not found" << std::endl;
    return 0;
  }
   
  cond->SetValues(left, right);
  cond->ResetCounts();
  if (enable) 
    cond->Enable();
  else 
    cond->Disable(alwaysreturn);   
   
  return cond;
}

void TXRSAnalysis::AssignCond(const char* condname, const char* HistoName) 
{
  TGo4Condition* cond = GetAnalysisCondition(condname);
  if (cond==0) 
    std::cout << "Condition " << condname << " did not found" << std::endl;
  else
    cond->SetHistogram(HistoName);
}

TGo4PolyCond* TXRSAnalysis::SetupPolyCond(const char* condname,
                                          Int_t size,
                                          void *_points, //  Float_t (*points)[2],
                                          Bool_t enable, 
                                          Bool_t alwaysreturn) {
  TGo4PolyCond* cond = dynamic_cast<TGo4PolyCond*> (GetAnalysisCondition(condname));
  if (cond==0) {
    std::cout << "Polygon condition " << condname << " did not found" << std::endl;
    return 0;
  }
   
  if ((size<3) || (_points==0)) {
    std::cout << "SetupPolyCond error: invalid conditions points for " << condname << std::endl;   
    return 0;
  }
   
  Float_t (*points)[2] = (Float_t (*)[2]) _points;
   
  Float_t fullx[size+1], fully[size+1];
  int numpoints = size;
  for (int i=0;i<numpoints;i++) {
    fullx[i] = points[i][0];
    fully[i] = points[i][1];
  }
   
  // connect first and last points
  if ((fullx[0]!=fullx[numpoints-1]) || (fully[0]!=fully[numpoints-1])) {
    fullx[numpoints] = fullx[0];
    fully[numpoints] = fully[0];
    numpoints++;
  }
 
  TCutG mycat("initialcut", numpoints, fullx, fully);
  cond->SetValues(&mycat);
  cond->ResetCounts();
  if (enable) 
    cond->Enable();
  else 
    cond->Disable(alwaysreturn);   
  return cond;                                           
}

ClassImp(TXRSAnalysis)

