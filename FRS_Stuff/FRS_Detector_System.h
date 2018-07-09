#ifndef FRS_DETECTOR_SYSTEM_H
#define FRS_DETECOR_SYSTEM_H

#include "TFRSBasicProc.h"
#include "TFRSAnlEvent.h"
#include "TFRSParameter.h"
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"

#include "Detector_System.cxx"

class RawEvent;
class TFRSSortEvent;
class TFRSCalibrEvent;
class TFRSAnlEvent;

class FRS_Detector_System : public Detector_System {

private:

  int* pdata;

    
   // MUSIC PARAMETERS //
  
  Float_t* dE;
  Float_t* dE_cor;

  // SCINTILLATOR PARAMETERS //
  
  Float_t* sci_l;
  Float_t* sci_r;
  Float_t* sci_e;
  Float_t* sci_tx;
  Float_t* sci_x;
  
  
  Float_t sci_tofll2;
  Float_t sci_tofll3;
  Float_t sci_tof2;
  Float_t sci_tofrr2;
  Float_t sci_tofrr3;
  Float_t sci_tof3;
  
  // ID PARAMETERS //
  
  Float_t ID_x2;
  Float_t ID_y2;
  Float_t ID_a2;
  Float_t ID_b2;
  
  Float_t ID_x4;
  Float_t ID_y4;
  Float_t ID_a4;
  Float_t ID_b4;
  
  Float_t* ID_brho;
  Float_t* ID_rho;
  
  Float_t beta;
  Float_t beta3;
  Float_t gamma;
  Float_t AoQ;
  Float_t AoQ_corr;
  
  Float_t z;
  Float_t z2;
  Float_t z3;
  
  Float_t timestamp;
  Float_t ts;
  Float_t ts2;   
    
    
public:
  FRS_Detector_System() ;
  TFRSUserProc(const char* name);
  virtual ~FRS_Detector_System() ;


  void Process_MBS(int*);
  void get_Event_data(Raw_Event*);
  int* get_pdata();


  //TFRSAnlEvent previous;

  TModParameter* Setup ;

};

#endif //FRS_DETECTOR_SYSTEM_H
