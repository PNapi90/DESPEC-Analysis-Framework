#ifndef TXRSPARAMETER_H
#define TXRSPARAMETER_H

#include "TGo4Parameter.h"
#include "TMap.h"
#include <map>

class TXRSParameter : public TGo4Parameter {
public:
  TXRSParameter();  
  TXRSParameter(const char* name); 
  virtual ~TXRSParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter *);
      
  Bool_t        fill_raw_histos; // fill raw histograms in unpack step
  Bool_t        after_run176;    // adapt cable-swapping at run176
      
  // MON Part
  Int_t         max_scaler;
      
  // common FRS part
      
  Float_t       dist_focS2;      /* All distances from    */
  Float_t       dist_MW21;       /*       TS3QT33         */
  Float_t       dist_MW22;       /*                       */
  Float_t       dist_SC21;	     /*        in mm          */
  Float_t       dist_SC22;       /*        in mm          */
  Float_t       dist_TPC3;
  Float_t       dist_TPC4;
  
  /*-----------------------*/
  /*    distances  S4      */
  /*-----------------------*/
  Float_t       dist_focS4;      /* All distances from    */
  Float_t       dist_MW41;       /*      HFSQT13          */
  Float_t       dist_MW42;       /*                       */
  Float_t       dist_MW43;       /*                       */
  Float_t       dist_SC41;       /*        in mm          */
  Float_t       dist_SC42;       /*        in mm          */
  Float_t       dist_SC43;       /*        in mm          */
  Float_t       dist_itag_42;    /*        in mm          */
  Float_t       dist_itag_43;    /*        in mm          */
  Float_t       dist_itag_stopper;/*        in mm          */
  Float_t       dist_TPC1;
  Float_t       dist_TPC2;      
  Float_t       dist_TPC5;
  Float_t       dist_TPC6;
 
  Float_t       dist_MUSICa1;    /*  MUSIC(window-anode1) */
  Float_t       dist_MUSICa2;    /*  MUSIC(window-anode2) */
  Float_t       dist_MUSICa3;    /*  MUSIC(window-anode3) */
  Float_t       dist_MUSICa4;    /*  MUSIC(window-anode4) */
  Float_t       dist_MUSIC1;     /*  MUSIC1               */
  Float_t       dist_MUSIC2;     /*  MUSIC2               */
  Float_t       dist_MUSIC3;     /*  MUSIC3               */


  /*-----------------------*/
  /*    distances not used */
  /*-----------------------*/
  Float_t       dist_focS8;      /* All distances from    */
  Float_t       dist_MW81;       /*       TS????          */
  Float_t       dist_MW82;       /*                       */
  Float_t       dist_SC81;       /*        in mm          */
      

  Float_t       dispersion[7];   /* d* Disp. S0-S2,S2-S4  */
  Float_t       magnification[7]; 
  Float_t       bfield[7];       /* D1/D2 D3/D4 Feld      */
  Float_t       rho0[7];         /* S2/4 Sollbahnradius s */
      
  Float_t       primary_z;       /* Z of primary beam     */
  Float_t       offset_z;
  Float_t       offset_z2;
  Float_t       offset_z3;
      
  Float_t		  a2AoQCorr;  /* Correction of AoQ based on S2 angle in x */
  Float_t		  a4AoQCorr;  /* Correction of AoQ based on S4 angle in x */

  


  
  ClassDef(TXRSParameter,1)
};

class Map1 : public TObject
{
public :
  std::map<int,int> map;
  Map1():TObject() { }
  Map1(const char* name):TObject() { }
  ClassDef(Map1,1);
};

class TModParameter : public TGo4Parameter{
public:
  TModParameter();  
  TModParameter(const char* name); 
  virtual ~TModParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);
  void setMap();
  int Nb_Modules;
  int Nb_QDC;
  int Nb_ADC;
  int Nb_TDC;
  int Nb_Scaler;
  int Nb_TimeStamp;

  int Scaler32bit;
  std::vector<int> EventFlags;

  // C++11 only
  //std::unordered_map<int, int> Nb_Channels; 
  //std::unordered_map<int, std::unordered_map<int,int> > MapCrates; // [ procID -> { VMEGeo, idMod}] 

  std::map<int, int> Nb_Channels; 
  std::map<int, std::map<int,int> > MapCrates; // [ procID -> { VMEGeo, idMod}] 
  
  TMap Maptemp;
  
  std::vector<std::string> ModType;
  ClassDef(TModParameter,1)
};


// *************************************************************************

class TMWParameter : public TGo4Parameter {
public:
  TMWParameter();  
  TMWParameter(const char* name); 
  virtual ~TMWParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);
      
  Float_t       x_factor[13];    /*  MWPC calibration        */
  Float_t       x_offset[13];    /*  mm                        */
  Float_t       y_factor[13];    /*                          */
  Float_t       y_offset[13];    /*  mm                        */
   
  Float_t       gain_tdc[5][13]; /* 14.09.05 CN+AM  TDC calibration ns/ch */
  ClassDef(TMWParameter,1)
};

// *************************************************************************

class TTPCParameter : public TGo4Parameter {
public:
  TTPCParameter();  
  TTPCParameter(const Text_t* name); 
  virtual ~TTPCParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);
      
  Float_t       x_factor[7][2];     /* [mm/ch]                  */
  Float_t       x_offset[7][2];     /*                          */
  Float_t       y_factor[7][4];     /* [mm/ch]                  */
  Float_t       y_offset[7][4];     /*                          */
      
  Float_t 	    tdc1_factor[32];
  Float_t 	    tdc2_factor[32];
      
  ClassDef(TTPCParameter,1)
};
// *************************************************************************





class TMUSICParameter : public TGo4Parameter {
public:
  TMUSICParameter();  
  TMUSICParameter(const char* name); 
  virtual ~TMUSICParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);
      
  Int_t         e1_off[8];       /* ADC offsets              */
  Float_t       e1_gain[8];      /* ADC gains                */ 
  Int_t         e2_off[8];       /* ADC offsets              */
  Float_t       e2_gain[8];      /* ADC gains                */ 
  Int_t         e3_off[4];       /* ADC offsets              */
  Float_t       e3_gain[4];      /* ADC gains                */ 
  Bool_t        b_selfcorr1;     /* 1 => Music1 x correction */
  /* 0 => MW41,MW42 x corr.   */
  Float_t       pos_a1[7];       /* pos. corr. de(i) ->  (1) */
  /*            de_cor(i)     */
      
  ClassDef(TMUSICParameter,1)
};

// -------------------------------------------------------------------------

class TSCIParameter : public TGo4Parameter {
public:
  TSCIParameter();  
  TSCIParameter(const char* name); 
  virtual ~TSCIParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);
      
  Float_t       le_a[2][12];     /* dE-left   shift,gain     */
  Float_t       re_a[2][12];     /* dE-right  shift,gain     */
  Float_t       le_veto_a[2][3]; /* veto dE-left  shift,gain */
  Float_t       re_veto_a[2][3]; /* veto dE-right shift,gain */

  Float_t       tac_factor[12];   /* TAC factor               */
  Float_t       tac_off[12];      /* TAC offset               */
  Float_t       x_a[7][12];      /* pos. corr. de(i) ->  (1) */
  Float_t       y5_a[7];         /* pos. corr. de(i) ->  (1) */

  Float_t       tof_bll2;        /* [ps/channel]             */
  Float_t       tof_brr2;        /* [ps/channel]             */
  Float_t       tof_a2;          /* shift [channels]         */
  Float_t       tof_bll3;        /* [ps/channel]             */
  Float_t       tof_brr3;        /* [ps/channel]             */
  Float_t       tof_a3;          /* shift [channels]         */
  Float_t       tof_bll4;        /* [ps/channel]             */
  Float_t       tof_brr4;        /* [ps/channel]             */
  Float_t       tof_a4;          /* shift [channels]         */
      
  ClassDef(TSCIParameter,1)
};

// -------------------------------------------------------------------------

class TIDParameter : public TGo4Parameter {
public:
  TIDParameter();  
  TIDParameter(const char* name); 
  virtual ~TIDParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);
      
  Int_t         x2_select;       /* 1=> x2 position from MWs */
  /* 0=> x2 position from SCI */
  /* 2=> x2 position from TPC */
  Int_t         x4_select;       /* 1=> x4 pos.from MWs      */
  /* 0=> x4 pos.from ...      */
  /* 2=> x4 pos.from MUSIC    */
  Int_t         x8_select;       /* 1=> x8 pos.from MWs      */
  /* 0=> x8 pos.from ...      */
  /* 2=> x8 pos.from MUSIC    */
  Float_t       id_tofoff2;      /* Tof offset S2-S4 [ps]    */
  Float_t       id_tofoff3;      /* Tof offset S2-S4 [ps]    */ //2nd tof from S2 - S4
  Float_t       id_tofoff4;      /* Tof offset S2-S8 [ps]    */ 
  Float_t       id_path2;        /* Flight path/c S2-S4 [ps] */
  Float_t       id_path3;        /* Flight path/c S2-S4 [ps] */ //2nd tof from S2 - S4
  Float_t       id_path4;        /* Flight path/c S2-S8 [ps] */
  Float_t       id_tofcorr2;     /* Tof correction for x4    */
  Float_t       id_tofcorr3;     /* Tof correction for x4    */ //2nd tof from S2 - S4
  Float_t       id_tofcorr4;     /* Tof correction for x8    */ 

  Float_t       vel_a[4];
  Float_t       vel_a2[4];
  Float_t       vel_a3[4];

  Int_t	zgate_low;
  Int_t	zgate_high;

  Int_t   ID_Z_AoverQ_num[5];
  Float_t ID_Z_AoverQ [5][5][2];

  Int_t   ID_x2AoverQ_num[6];
  Float_t ID_x2AoverQ [6][5][2];

  
  ClassDef(TIDParameter,1)
};


// SI parameters
class TSIParameter : public TGo4Parameter {
public:
  TSIParameter();  
  TSIParameter(const char* name); 
  virtual ~TSIParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);

  Float_t si_factor1;
  Float_t si_factor2;
  Float_t si_factor3;
  Float_t si_factor4;
  Float_t si_factor5;

  Float_t si_offset1;
  Float_t si_offset2;
  Float_t si_offset3;
  Float_t si_offset4;
  Float_t si_offset5;

  Float_t dssd_offset[32];
  Float_t dssd_factor[32];

  ClassDef(TSIParameter,1)
};   

// MRTOF parameters
class TMRTOFMSParameter : public TGo4Parameter {
public:
  TMRTOFMSParameter();  
  TMRTOFMSParameter(const char* name); 
  virtual ~TMRTOFMSParameter();
  virtual void Print(Option_t* t="") const;
  virtual Bool_t UpdateFrom(TGo4Parameter*);

  Float_t MRTOFMS_a;
  Float_t MRTOFMS_b;
  Float_t MRTOFMS_t0;
  Float_t MRTOFMS_TXRS;

  ClassDef(TMRTOFMSParameter,1)
};  

#endif //TXRSPARAMETER_H


