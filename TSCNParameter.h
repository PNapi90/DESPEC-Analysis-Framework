// $Id: TSCNParameter.h 755 2011-05-20 08:04:11Z linev $
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

#ifndef TSCNParameter_H
#define TSCNParameter_H

#include "TGo4Parameter.h"

#include "TMap.h"
#include <map>

class TSCNParameter : public TGo4Parameter {
   public:
      TSCNParameter(const char* name = 0);
      virtual ~TSCNParameter() {}
      void SetPedestal(int chan,int val);
      void SetFactor(int chan,double val);

      Int_t Pedestals[32];//Pedestals 
      Double_t Factors[32]; //Gain  factors
      Int_t peak_alg;  //0 for centroid, 1 for Lerche
      Int_t Anodes_Low; //Lower treshold for anodes sum
      Int_t Anodes_High;//Upper treshold for anodes sum
      Int_t QDC2Ch0_Low; //Lower treshhold for qdc2 chan. 1
      Int_t QDC2Ch0_High;//Upper treshhold for qdc2 chan. 1
      Int_t QDC2Ch1_Low; //Lower treshhold for qdc2 chan. 2
      Int_t QDC2Ch1_High;//Upper treshhold for qdc2 chan. 2
      Int_t ClusterCh0_Low; //Lower treshhold for cluster chan. 1
      Int_t ClusterCh0_High;//Upper treshhold for cluster chan. 1

      Bool_t NaI_F; //If I have NaI detectors
      Bool_t Cluster_F; //If I have the cluster
    
   ClassDef(TSCNParameter,1)
};

#endif //TSCNPARAMETER_H

