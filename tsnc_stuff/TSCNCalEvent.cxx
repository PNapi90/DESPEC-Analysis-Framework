// $Id: TSCNCalEvent.cxx 755 2011-05-20 08:04:11Z linev $
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

#include "TSCNCalEvent.h"
void TSCNCalEvent::Clear(Option_t *t)
{
  for (int i=0;i<SCN_NUM_CHAN;i++)
    {
      frDataC1[i] = 0.;
      frDataC2[i] = 0.;
      frDataC3[i] = 0.;
    
//      Det[i]= 0. ;
       
    }
 
 
 
  
}
