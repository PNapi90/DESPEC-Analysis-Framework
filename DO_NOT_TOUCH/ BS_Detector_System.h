
#ifndef BS_DETECTOR_SYSTEM_H
#define BS_DETECTOR_SYSTEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>

#include "Detector_System.cxx"


class GALILEO_Detector_System : public Detector_System{
typedef unsigned long long ULong64_t;

private:

    void BS_Unpack();
    void FRS_Timestamp_Module(UInt_t vme_num[21][32]){

    Int_t *pdata = psubevt->GetDataField();
    Int_t len = 0;	  
    Int_t vme_chn;
    Int_t lenMax = (psubevt->GetDlen()-2)/2;
    
    
    UInt_t vme0[21][32];         // FRS crate                                
    UInt_t vme1[21][32];         // TPC crate 
    UInt_t vme3[21][32];         // Mtof crate

    UInt_t vme5[21][32];          //Main Crate or User Crate

    Int_t vme2scaler[32];         // User Crate Messhure
    Int_t vme3scaler[32];         // User Crate Messhure

    Int_t vme2s[32][10];           // User TDC (V1290) 
    Int_t vme2s_trailing[32][10];  // User TDC (V1290) 
    Int_t nhit5[32][2];            // multiplicity (V1290)

    Int_t vme3s_MT[32][10];           // Mtof TDC (V1290) 
    Int_t vme3s_MT_trailing[32][10];  // Mtof TDC (V1290) 
    Int_t vme3_MT_nhit5[32][2];       // multiplicity (V1290)
  
    UInt_t vme4[21][32] ; // SOFIA crate temp need multiplicity for both MDPP and vftx
    


public:
	BS_Detector_System();
	~BS_Detector_System();
	
	//void Process_FRS(TModParameter* , TGo4MbsSubEvent* , TGo4MbsEvent*){};
	void Process_FRS(TGo4MbsSubEvent* psubevt);

	void Process_MBS(int*);
	void get_Event_data(Raw_Event*);
	int* get_pdata();

	unsigned long** tmp_get_coarse_T(){return NULL;};
    int tmp_get_am_hits(){return 0;};

    unsigned int** tmp_get_chID(){return NULL;};

    int* tmp_get_iterator(){return NULL;};

    bool calibration_done(){return false;}
    void write(){return;};
    void set_Gain_Match_Filename(std::string){return;};


};



#endif
