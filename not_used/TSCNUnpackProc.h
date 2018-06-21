// $Id: TSCNUnpackProc.h 755 2011-05-20 08:04:11Z linev $
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

#ifndef TSCNUNPACKPROCESSOR_H
#define TSCNUNPACKPROCESSOR_H



#include "TGo4EventProcessor.h"

class TSCNUnpackEvent;

#include "TSCNUnpackEvent.h"
//#include "TH1D.h"
#define MAX_CHA_INPUT 35

class TSCNUnpackProc : public TGo4EventProcessor {
   public:
      TSCNUnpackProc();
      TSCNUnpackProc(const char* name);
      virtual ~TSCNUnpackProc();

      virtual Bool_t BuildEvent(TGo4EventElement* dest);



struct TAMEXHEADER
{

       double PreTriggerWindow;
       double PostTriggerWindow;
       int    TamexID;
       int    SFPID;
       int    UnusedChannelId;
       int    SizeOfDataInWords;
       int    TDCHeader;
       int    TDCTrailer;
}tamexheader;

     struct PADI
     {
       unsigned int  ChannelNo;
       bool LeadingEdge;
       bool TrailingEdge;
       double  CoarseTime;
       double  FineTime;
       int  NHits[33];
     }tamexdata;

     void PrintCheckTamexHeaderData();
     void PrintCheckTamexTDCData();
//     void FillTamexHist();


//   protected:
      TH1           *fCr1Ch[35]; //PADI_Coarse
      TH1           *fCr2Ch[35]; //PADI_Fine
      TH1           *fCr3Ch[35]; //TimeDiffLeading
      TH1           *fCr4Ch[35]; //TimeDiffTrailing   
      TH1           *fCr5Ch[35]; //TOT
      TH1           *fCr6Ch[35]; //TOTAND
      TH2           *fCr7Ch[35];     // PADI_2D
      TH1           *fCr8Ch[35]; // TOTANDdiff
      TH1           *fCr9Ch[35]; // Time wrt Trigger
	  TH1           *fCr10Ch[35]; // 
	  TH1           *fCr11Ch[35];
 TH1           *fCr12Ch[35];

void    SetFineTimeFactor(int , int , double );
double  GetFineTimeFactor(unsigned int , int );

     double  fCoarse[33];
     double  fFine[33];     
	 double  fTrailing[33];

     double  B[33]; //  B is sum
     int     count_event;
     
 //     TGo4Picture   *picPMTraw; //Picture of the 32 chanels of QDC1

      //fir the SIS modules

 //     TH1 *hph[FADC_CHAN]; //energy
 //     TH1 *htrace[FADC_CHAN];//trace

      //TGo4Picture   *picSIStraces; //Picture of the 32 chanels of QDC1
      //TGo4Picture *picSISenergy;

 private:
      Bool_t ffill;
      Int_t fshift;
 double  A[33][2000]; // A is factor 

   ClassDef(TSCNUnpackProc,1)
};

#endif //TSCNUNPACKPROCESSOR_H
