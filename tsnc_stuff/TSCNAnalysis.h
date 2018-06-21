// $Id: TSCNAnalysis.h 524 2009-11-11 09:53:34Z adamczew $
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

#ifndef TSCNANALYSIS_H
#define TSCNANALYSIS_H

#include "TGo4Analysis.h"

class TH1D;
class TGo4MbsEvent;
class TSCNUnpackEvent;
class TSCNParameter;
class TSCNCalEvent;
class TSCNAnlEvent;


class TSCNAnalysis : public TGo4Analysis  {
   public:
      TSCNAnalysis();
      TSCNAnalysis(int argc, char** argv);
      virtual ~TSCNAnalysis() ;
      virtual Int_t UserPreLoop();
      virtual Int_t UserEventFunc();
      virtual Int_t UserPostLoop();
      
   private:
      TGo4MbsEvent       *fMbsEvent;
      TSCNUnpackEvent    *fRawEvent;
      TSCNCalEvent       *fCalEvent;
      TSCNAnlEvent       *fAnlEvent;
      TSCNParameter      *fPar;


      Int_t               fEvents;
      Int_t               fLastEvent;

   ClassDef(TSCNAnalysis,1)
};

#endif //TSCNANALYSIS_H



