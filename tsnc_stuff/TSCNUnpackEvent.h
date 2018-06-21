// $Id: TSCNUnpackEvent.h 755 2011-05-20 08:04:11Z linev $
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

#ifndef TSCNEVENT_H
#define TSCNEVENT_H

#define SCN_NUM_CHAN 36
#define RAW_DATA_LENGTH 1024
#define E_DATA_LENGTH   512
#define FADC_CHAN 8
#define ADC_NUM_CHAN 32


#include "TGo4EventElement.h"

class TSCNUnpackEvent : public TGo4EventElement 
{
 public:
 TSCNUnpackEvent() : TGo4EventElement() {}
 TSCNUnpackEvent(const char* name) : TGo4EventElement(name) {}
  virtual ~TSCNUnpackEvent() {}
  
  /**
   * Method called by the event owner (analysis step) to clear the
   * event element.
   */
  virtual void Clear(Option_t *t="");
  
  Int_t fiCrate1[SCN_NUM_CHAN]; //This two are for the QDC's 
  Int_t fiCrate2[SCN_NUM_CHAN];
  Double_t fiCrate3[SCN_NUM_CHAN];  // For TDC
 
  //For the FADC
//  Int_t trace[FADC_CHAN][RAW_DATA_LENGTH];
//  Int_t trace_e[FADC_CHAN][RAW_DATA_LENGTH];
//  Int_t pileup[FADC_CHAN];
//  Int_t ph[FADC_CHAN];


  
  ClassDef(TSCNUnpackEvent,1)
    };
#endif //TSCNEVENT_H



