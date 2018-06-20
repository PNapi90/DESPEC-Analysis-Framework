// $Id: TSCNUnpackEvent.cxx 755 2011-05-20 08:04:11Z linev $
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

#include "TSCNUnpackEvent.h"

void TSCNUnpackEvent::Clear(Option_t *t)
{
   for (int i=0;i<SCN_NUM_CHAN;i++) {
      fiCrate1[i] = 0;
      fiCrate2[i] = 0;
      fiCrate3[i] = 0;
   }
/*
for(int i=0;i<FADC_CHAN;i++)
     {
       for(int j=0;j<RAW_DATA_LENGTH;j++)
	 {
	   trace[i][j]=0;
	   trace_e[i][j]=0;
	 }
       pileup[i]=0;
       ph[i]=0;
     }
*/
}
