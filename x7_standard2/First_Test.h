//---------------------------------------------------------------
//        Go4 Release Package v3.02-0 (build 30200) 
//                      21-July-2006
//---------------------------------------------------------------
//	 The GSI Online Offline Object Oriented (Go4) Project
//	 Experiment Data Processing at EE department, GSI
//---------------------------------------------------------------
//
//Copyright (C) 2000- Gesellschaft f. Schwerionenforschung, GSI
//                    Planckstr. 1, 64291 Darmstadt, Germany
//Contact:            http://go4.gsi.de
//----------------------------------------------------------------
//This software can be used under the license agreements as stated
//in Go4License.txt file which is part of the distribution.
//----------------------------------------------------------------
#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "TGo4EventProcessor.h"
#include "TTree.h"
#include  "TGo4Analysis.h"


class Tx7_Param;

class First_Test : public TGo4EventProcessor {
   public:
      First_Test() ;
      First_Test(const char* name);
      virtual ~First_Test() ;

      Bool_t BuildEvent(TGo4EventElement* target); // event processing function

 private:
      TH1I          *fCr1Ch[8];
      TH1I          *fCr2Ch[8];
      TH2I          *fCr1Ch1x2;
      TH1I          *fHis1;
      TH1I          *fHis1gate;
      TH1I          *fHis2;
      TH1I          *fHis2gate;

      TH1I          *fE_QDC[32];
      TH1I          *fE_ADC[32];
      TH1I          *fTime[32];
      TH1I          *fTime2[32];
      TH1I          *fTime3[32];
      TH1I          *fdTime;


      TGo4WinCond   *fconHis1;
      TGo4WinCond   *fconHis2;
      TGo4PolyCond  *fPolyCon;
      TGo4CondArray *fConArr;
      TGo4MbsEvent  *fInput;
      Tx7_Param      *fParam;
      TGo4Picture   *fPicture;
      TGo4Picture   *fcondSet;
      Float_t        fCrate1[8];
      Float_t        fCrate2[8];

      Float_t         E1,E2,T1,T2;
      TTree          *gg_tree;

ClassDef(First_Test,1)
};
#endif //TUNPACKPROCESSOR_H


//----------------------------END OF GO4 SOURCE FILE ---------------------
