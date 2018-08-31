// $Id: TSCNCalProc.cxx 754 2011-05-18 11:04:52Z adamczew $
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

#include "TSCNCalProc.h"

#include <cstdlib>
#include <math.h>

#include "TH1.h"
#include "TH2.h"
#include "Riostream.h"
#include "TROOT.h"
#include "TFile.h"

#include "TGo4WinCond.h"
#include "TGo4Analysis.h"

#include "TGo4Picture.h"

#include "TSCNCalEvent.h"
#include "TSCNUnpackEvent.h"
#include "TSCNParameter.h"


//-----------------------------------------------------------
TSCNCalProc::TSCNCalProc() :
  TGo4EventProcessor(),
  fParam1(0)
{
}
//-----------------------------------------------------------
TSCNCalProc::TSCNCalProc(const char* name) :
   TGo4EventProcessor(name)
{
  cout << "**** TSCNCalProc: Create" << endl;
  
  TFile* f;
  f = new TFile("Parameters.root");
  if(f->IsOpen()) //The File Parameter.root exist
    {
      cout<<"Reading initial parameters from file Parameters.root"<<endl;
      fParam1=(TSCNParameter*)f->Get("SCNParameter");
      f->Close();
    }
  else //Data file with parameters
    {
      ifstream myfile;
      myfile.open ("Parameters.dat", ios::in); 
      if(myfile.is_open()) //I have the file
	{
	  fParam1 = (TSCNParameter*)  GetParameter("SCNParameter");
	  string intro;
	  myfile>>intro;
	  cout<<intro<<endl;
	  cout<<"Readind data file Parameters.dat"<<endl;
	  int a;
	  double b;
	  for(int i=0;i<SCN_NUM_CHAN;i++)
	    {
	      myfile>>a;
	      fParam1->SetPedestal(i,a);
	    }
	  myfile>>intro;
	  for(int i=0;i<SCN_NUM_CHAN;i++)
	    {
	      myfile>>b;
	      fParam1->SetFactor(i,b);
	    }
	  myfile.close();
	}
      else  //No Parameters at all. Go to default constructor
	{
	  cout<<"No parameters file. Using default constructor"<<endl;
	  fParam1 = (TSCNParameter*)  GetParameter("SCNParameter");
	}
    }


  for(int i=0;i<SCN_NUM_CHAN;i++) 
    {
      fQDC1Ch[i] = MakeTH1('D', Form("QDC1Cal/QDC1Cal_channel%2d",i), Form("QDC 1 channel %2d",i), 4001, 1., 8001.);
    }
  Sum=MakeTH1('D',"QDC1Cal/QDC1Sum","QDC1Sum",1600,0.,20000.);
  picPMT = GetPicture("picPMT");
  picPMT = new TGo4Picture("pmtCal","photomultiplier Calibrated",1,2);
  picPMT -> SetDrawHeader();
  picPMT -> Pic(0,0) -> SetDivision(SCN_NUM_CHAN/2,1);// X anodes
  picPMT -> Pic(0,1) -> SetDivision(SCN_NUM_CHAN/2,1);// Y anodes
  
  for(int i=0;i<SCN_NUM_CHAN;i++)
    {
      if(i<16)
	{
	  picPMT -> Pic(0,0) -> Pic(i,0) -> AddH1(fQDC1Ch[i]);// HistRaw X
	  picPMT -> Pic(0,0) -> Pic(i,0) -> SetRangeX(0,8000);
	}
      else 
	{
	  picPMT -> Pic(0,1) -> Pic(i-SCN_NUM_CHAN/2,0) -> AddH1(fQDC1Ch[i]);// HistRaw X
	  picPMT -> Pic(0,1) -> Pic(i-SCN_NUM_CHAN/2,0) -> SetRangeX(0,8000);
	}
    }
  AddPicture(picPMT);
     
}
//-----------------------------------------------------------
TSCNCalProc::~TSCNCalProc()
{
   cout << "**** TSCNCalProc: Delete" << endl;
}
//-----------------------------------------------------------
Bool_t TSCNCalProc::BuildEvent(TGo4EventElement* dest)
{

  Bool_t isValid=kFALSE; // validity of output event
  
  TSCNUnpackEvent* inp_evt  = (TSCNUnpackEvent*) GetInputEvent();
  TSCNCalEvent* out_evt = (TSCNCalEvent*) dest;
  
  // see comments in UnpackProc
  if((inp_evt==0) || !inp_evt->IsValid()){ // input invalid
    out_evt->SetValid(isValid); // invalid
    return isValid; // must be same is for SetValid
   }
  isValid=kTRUE;

  //Calibrated histograms

  // unused // double sum=0.;


  // unused // double dat;
  for(Int_t j=0;j<SCN_NUM_CHAN;j++)// Maping data to DetID(E,t)
    {

	out_evt->frDataC1[j]=inp_evt->fiCrate1[j];
	out_evt->frDataC2[j]=inp_evt->fiCrate2[j];
	out_evt->frDataC3[j]=inp_evt->fiCrate3[j];
    }
  out_evt->SetValid(isValid);
  return isValid;

}
