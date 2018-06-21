// $Id: TSCNCalProc.h 755 2011-05-20 08:04:11Z linev $
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

#ifndef TSCNCALPROCESSOR_H
#define TSCNCALPROCESSOR_H

#include "TGo4EventProcessor.h"

#include "TSCNUnpackEvent.h"



class TSCNCalEvent;
class TSCNParameter;

class TSCNCalProc : public TGo4EventProcessor {
   public:
      TSCNCalProc();
      TSCNCalProc(const char * name);
      virtual ~TSCNCalProc();

      virtual Bool_t BuildEvent(TGo4EventElement* dest);

      TH1 *fQDC1Ch[SCN_NUM_CHAN]; //The calibrated histograms
     
      // TH1 *GermaniumCal;
      TH1 *Sum; 
      TSCNParameter *fParam1;
      TGo4Picture *picPMT;
      
      ClassDef(TSCNCalProc, 1)
	};
#endif //TSCNANLPROCESSOR_H
