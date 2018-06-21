// $Id: TSCNCalEvent.h 755 2011-05-20 08:04:11Z linev $
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

#ifndef TSCNCALEVENT_H
#define TSCNCALEVENT_H

#include "TGo4EventElement.h"
#include "TSCNUnpackEvent.h"


class TSCNCalEvent : public TGo4EventElement {
   public:
      TSCNCalEvent() : TGo4EventElement() {}
      TSCNCalEvent(const char* name) : TGo4EventElement(name) {}
      virtual ~TSCNCalEvent() {}

      virtual void  Clear(Option_t *t="");

      Float_t frDataC1[SCN_NUM_CHAN]; //For QDC 1
      Float_t frDataC2[SCN_NUM_CHAN]; //For QDC 2
      Double_t frDataC3[SCN_NUM_CHAN]; //For TDC


  

   ClassDef(TSCNCalEvent,1)
};
#endif //TSCNCALEVENT_H



