// $Id: TSCNParameter.cxx 755 2011-05-20 08:04:11Z linev $
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

#include "TSCNParameter.h"
#include "Riostream.h"
#include "TObjString.h"

TSCNParameter::TSCNParameter(const char* name) :
  TGo4Parameter(name)
{
  
  for(int i=0;i<32;i++)
    {
      Pedestals[i]=0;
      Factors[i]=1.;
    }
  peak_alg=0;
  Anodes_Low=0;
  Anodes_High=8000;
  QDC2Ch0_Low=0;
  QDC2Ch0_High=5000;
  QDC2Ch1_Low=0;
  QDC2Ch1_High=5000;
  ClusterCh0_Low=0;
  ClusterCh0_High=5000;

  NaI_F=false;
  Cluster_F=false;
}

void TSCNParameter::SetPedestal(int chan,int val)
{
  Pedestals[chan]=val;
}
void TSCNParameter::SetFactor(int chan,double val)
{
  Factors[chan]=val;
}

