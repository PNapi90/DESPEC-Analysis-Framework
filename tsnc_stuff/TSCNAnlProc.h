// $Id: TSCNAnlProc.h 755 2011-05-20 08:04:11Z linev $
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

#ifndef TSCNANLPROCESSOR_H
#define TSCNANLPROCESSOR_H

#include "TGo4EventProcessor.h"


#include "TSCNUnpackEvent.h"
#include "TSCNCalEvent.h"

#include "TTree.h"
#include "TFile.h"

class TSCNAnlEvent;
class TSCNParameter;

class TSCNAnlProc : public TGo4EventProcessor {
   public:
      TSCNAnlProc();
      TSCNAnlProc(const char * name);
      virtual ~TSCNAnlProc();

      //double brent(TH1 *h1, TH1D *href1, Float_t ax, Float_t bx, Float_t cx);
 
      virtual Bool_t BuildEvent(TGo4EventElement* dest);


      TH1 *fTimeDiff; /// Time difference Histogram from TDC channels
      TH1 *fGatedHist; /// Time difference Histogram from TDC channels
      TGo4WinCond  *fconHis1;
      TGo4WinCond  *fWinCon1;

      TH1 *XAnodes; //! Charge for the x anodes
      TH1 *YAnodes; //! Charge for the y andes
      TSCNParameter *fParam1;
      TGo4Picture *QSpectrum;
     
      //  TH1 *GermaniumCal;//Calibrated ADC(Germanuim spectra)
      TH2 *fCoincQ1A1; //Coincidences with germaniun 
      TH2 *fCoincQ1T1; //Coincidences with germaniun 
//    TH1 *SumEner; //Sum of energies of Ge and Lyso
      TH2 *MatrixCond; //!Position matrix + conditions 
      TH2 *MatrixCluster; //!Position matrix + conditions 
//      TH2 *MatrixRec; //With the position reconstruction
//      TH2 *MatrixClustRec; //With the position reconstruction

      TH1D* Refx; //Reference histogram for X anodes
      TH1D* Refy; //Reference histogram for Y anodes
      
      ClassDef(TSCNAnlProc, 1)
	};
#endif //TSCNANLPROCESSOR_H
