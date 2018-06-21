// $Id: TSCNAnlProc.cxx 754 2011-05-18 11:04:52Z adamczew $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum für Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TSCNAnlProc.h"

#include <cstdlib>
#include <math.h>

#include "TH1.h"
#include "TH2.h"
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "TGo4WinCond.h"
#include "TGo4CondArray.h"
#include "TGo4Analysis.h"

#include "TGo4Picture.h"

#include "TSCNAnlEvent.h"
#include "TSCNUnpackEvent.h"
#include "TSCNCalEvent.h"
#include "TSCNParameter.h"

//#include "BrentMethod.c"
//#include "Linearizator.h" //pos reconstruction
//#include "GoldenMethod.c"

#define range 6


//-----------------------------------------------------------
TSCNAnlProc::TSCNAnlProc() :
  TGo4EventProcessor(),
  fParam1(0),
  fTimeDiff(0),
  fGatedHist(0),
  fCoincQ1A1(0),
  fCoincQ1T1(0),
  fconHis1(0)
{
}
//-----------------------------------------------------------
TSCNAnlProc::TSCNAnlProc(const char* name) :
   TGo4EventProcessor(name)
{
  cout << "**** TSCNAnlProc: Create" << endl;
  fParam1 = (TSCNParameter*)  GetParameter("SCNParameter");
 

  
  fTimeDiff = MakeTH1('D',"TimeDiff","TimeDiff",20000,-10000,10000);
  fGatedHist = MakeTH1('D',"GatedHist","GatedHist",4000,1,4000);

  fCoincQ1A1 = MakeTH2('D',"CoincE","CoincQDC1ADC1", 200, 0.0, 2000.0, 200, 0.0, 2000.0);     
  fCoincQ1T1 = MakeTH2('D',"CoincET","CoincQDC1TDC1", 2000, -2000.0, 2000.0, 3000, 0.0, 3000.0);     
  fconHis1   = MakeWinCond("cHis1", -200, 200, "TimeDiff"); 
  fWinCon1   = MakeWinCond("wincon1", -100, 100, 0, 3000);

  fconHis1->Enable();
  fWinCon1->Enable();


 }
//-----------------------------------------------------------
TSCNAnlProc::~TSCNAnlProc()
{
  cout << "**** TSCNAnlProc: Delete" << endl;
}
//-----------------------------------------------------------
Bool_t TSCNAnlProc::BuildEvent(TGo4EventElement* dest)
{
  Bool_t isValid=kFALSE; // validity of output event
  
  TSCNCalEvent* inp_evt  = (TSCNCalEvent*) GetInputEvent();
  TSCNAnlEvent* out_evt = (TSCNAnlEvent*) dest;

 
  
  // see comments in UnpackProc
  if((inp_evt==0) || !inp_evt->IsValid()){ // input invalid
    out_evt->SetValid(isValid); // invalid
    return isValid; // must be same is for SetValid
  }
  isValid=kTRUE;
  

  Double_t tdc_event[SCN_NUM_CHAN];
  Double_t tdc_event_diff[SCN_NUM_CHAN];
  Double_t timediffdata;



  for(Int_t j=0;j<SCN_NUM_CHAN;j++)
    {

      out_evt->Data1[j] = inp_evt->frDataC1[j];
      out_evt->Data2[j] = inp_evt->frDataC2[j];
      out_evt->Data3[j] = inp_evt->frDataC3[j];
   }


  for(Int_t j=0;j<SCN_NUM_CHAN;j++)
    {
      tdc_event[j]=inp_evt->frDataC3[j];
	  if(tdc_event[0]>1000 && tdc_event[j]> 1000)
	  {
      tdc_event_diff[j]= (tdc_event[0]-tdc_event[j])*25.0;
	  }
    }






  //-----the fitting algorithms--------
  
  double pos_x=0, pos_y=0;

  //--------filling the time difference histogram  -------

      if(tdc_event[0]>3000 && tdc_event[1]> 3000)
      {
		fTimeDiff  ->Fill(Double_t(tdc_event_diff[1]));
		fCoincQ1T1->Fill(tdc_event_diff[1],inp_evt->frDataC1[1]);
      }


//     if(fWinCon1->Test(tdc_event_diff[1],inp_evt->frDataC1[1]))
//	 {fGatedHist ->Fill(inp_evt->frDataC1[0]);}
  

//      if(tdc_event_diff[1]<100 && tdc_event_diff[1]>-400)
//      if(fconHis1->Test(tdc_event_diff[1]))
//      {
//        fGatedHist ->Fill(inp_evt->frDataC1[0]);
//		fCoincQ1A1->Fill(inp_evt->frDataC1[0],inp_evt->frDataC1[1]);
//      }

//  out_evt->sum=inp_evt->ChargeSum;  
//  double tmpx[2] = {pos_x,pos_y};
 
  
  //Coincidences with Cluster
  

 
 out_evt->SetValid(isValid);
  return isValid;
}

