#include "Raw_Event.h"

#include <iostream>

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event(bool PADI_OR_PADIWA) : PLASTIC_Data(),PLASTIC_VME_Data(),FATIMA_Data(){
    Event_Type = -1;
    
    PLASTIC_Data.PADI_OR_PADIWA = PADI_OR_PADIWA;

}

//---------------------------------------------------------------

Raw_Event::~Raw_Event(){}

//---------------------------------------------------------------

// ################################################################# 


void Raw_Event::set_DATA_MUSIC(Float_t* FRS_dE,Float_t* FRS_dE_cor){
    
    for(int i; i<3; ++i){
	
	dE[i] = FRS_dE[i];
	dE_cor[i] = FRS_dE_cor[i];
	
    }	    

}
void Raw_Event::set_DATA_SCI(Float_t* FRS_sci_l,Float_t* FRS_sci_r,Float_t* FRS_sci_e,Float_t* FRS_sci_tx,Float_t* FRS_sci_x){
    
    for(int i; i<12; ++i){
	
	sci_l[i] = FRS_sci_l[i];
	sci_r[i] = FRS_sci_r[i];
	sci_e[i] = FRS_sci_e[i];
	sci_tx[i] = FRS_sci_tx[i];
	sci_x[i] = FRS_sci_x[i];
	
    }

}

void Raw_Event::set_DATA_SCI_dT(Int_t FRS_dt_21l_21r, Int_t FRS_dt_41l_41r,
				Int_t FRS_dt_21l_41l, Int_t FRS_dt_21r_41r,
				Int_t FRS_dt_42l_42r, Int_t FRS_dt_43l_43r,
				Int_t FRS_dt_21l_42l, Int_t FRS_dt_21r_42r,
				Int_t FRS_dt_81l_81r, Int_t FRS_dt_21l_81l, Int_t FRS_dt_21r_81r){


	dt_21l_21r = FRS_dt_21l_21r; 
	dt_41l_41r = FRS_dt_41l_41r;
	dt_21l_41l = FRS_dt_21l_41l;
	dt_21r_41r = FRS_dt_21r_41r;
	dt_42l_42r = FRS_dt_42l_42r;
	dt_43l_43r = FRS_dt_43l_43r;
	dt_21l_42l = FRS_dt_21l_42l;
	dt_21r_42r = FRS_dt_21r_42r;
	dt_81l_81r = FRS_dt_81l_81r;
	dt_21l_81l = FRS_dt_21l_81l;
	dt_21r_81r = FRS_dt_21r_81r;




}
void Raw_Event::set_DATA_SCI_ToF(Float_t FRS_sci_tofll2,Float_t FRS_sci_tofll3,Float_t FRS_sci_tof2,Float_t FRS_sci_tofrr2,Float_t FRS_sci_tofrr3,Float_t FRS_sci_tof3){
    
    sci_tofll2 = FRS_sci_tofll2;
    sci_tofll3 = FRS_sci_tofll3;
    sci_tof2   = FRS_sci_tof2;  
    sci_tofrr2 = FRS_sci_tofrr2; 
    sci_tofrr3 = FRS_sci_tofrr3; 
    sci_tof3   = FRS_sci_tof3;
    
}
void Raw_Event::set_DATA_ID_2_4(Float_t FRS_ID_x2,Float_t FRS_ID_y2,Float_t FRS_ID_a2,Float_t FRS_ID_b2,Float_t FRS_ID_x4,Float_t FRS_ID_y4,Float_t FRS_ID_a4,Float_t FRS_ID_b4){
    
    ID_x2 = FRS_ID_x2;
    ID_y2 = FRS_ID_y2;
    ID_a2 = FRS_ID_a2;
    ID_b2 = FRS_ID_b2;
    ID_x4 = FRS_ID_x4;
    ID_y4 = FRS_ID_y4;
    ID_a4 = FRS_ID_a4;
    ID_b4 = FRS_ID_b4;
    
}
void Raw_Event::set_DATA_ID_Beta_Rho(Float_t* FRS_ID_brho,Float_t* FRS_ID_rho,Float_t FRS_beta,Float_t FRS_beta3,Float_t FRS_gamma){
    
    for(int i; i<2; ++i){
	
	ID_brho[i] = FRS_ID_brho[i];
	ID_rho[i] = FRS_ID_rho[i];
	
    }
    
    beta = FRS_beta;
    beta3 = FRS_beta3;
    gamma = FRS_gamma;

}
void Raw_Event::set_DATA_ID_Z_AoQ(Float_t FRS_AoQ,Float_t FRS_AoQ_corr,Float_t FRS_z,Float_t FRS_z2,Float_t FRS_z3){
    
    AoQ = FRS_AoQ;
    AoQ_corr = FRS_AoQ_corr;
    z = FRS_z;
    z2 = FRS_z2;
    z3 = FRS_z3;    
    
}
void Raw_Event::set_DATA_ID_Timestamp(Float_t FRS_timestamp,Float_t FRS_ts,Float_t FRS_ts2){
    
    timestamp = FRS_timestamp;
    ts = FRS_ts;
    ts2 = FRS_ts2;
    
}

// #################################################################

int Raw_Event::get_Event_type(){
    return Event_Type;
}


void Raw_Event::set_DATA_FATIMA(int QDC_FIRED,int TDC_FIRED,std::vector<double> &Ql_Raw,std::vector<double> &Qs_Raw,
                                std::vector<double> &Ql,std::vector<ULong64_t> &TDC,std::vector<double> &TDC_ns,
                                std::vector<ULong64_t> &QDC_c,std::vector<double> &QDC_f,std::vector<int> &det_ids_QDC,
                                std::vector<int> &det_ids_TDC){
	
    FATIMA_Data.SetDATA(QDC_FIRED,TDC_FIRED,Ql_Raw,Qs_Raw,Ql,TDC,TDC_ns,QDC_c,QDC_f,det_ids_QDC,det_ids_TDC);
	
    Event_Type = 3;

}

//---------------------------------------------------------------

void Raw_Event::set_DATA_PLASTIC(std::vector<int> &it,std::vector<std::vector<double> > &Edge_Coarse,
                                 std::vector<std::vector<double> > &Edge_fine, std::vector<std::vector<UInt> > &ch_ed,
                                 std::vector<double> &Coarse_Trigger,std::vector<double> &Fine_Trigger,int amount_hit_tamex)
{
	
    PLASTIC_Data.SetDATA(it,Edge_Coarse,Edge_fine,ch_ed,Coarse_Trigger,Fine_Trigger,amount_hit_tamex);
    VME_Event = false;
    Event_Type = 2;
}

//---------------------------------------------------------------

void set_DATA_PLASTIC_VME(std::vector<double> &VME_QDC_Data,std::vector<double> &VME_QDC_Channels,
                          std::vector<double> &VME_TDC_Data,std::vector<double> &VME_TDC_Channels,int TDC_iterator){

    PLASTIC_VME_Data.SetDATA(VME_QDC_Data,VME_QDC_Channels,VME_TDC_Data,VME_TDC_Channels,TDC_iterator);
    VME_Event = true;
    Event_Type = 2;

}

//---------------------------------------------------------------

bool Raw_Event::PLASTIC_CheckVME(){
    return VME_Event;
}

//---------------------------------------------------------------

void Raw_Event::set_DATA_GALILEO(int GAL_FIRED,ULong64_t* sum_time,int* pileup,int* hit_pattern,ULong64_t* chan_time,double* chan_en, int* FEBEX_det_ids){
	this->GAL_FIRED = GAL_FIRED;
	    
	for(int i = 0;i < GAL_FIRED;++i){
	    
	    GALILEO_Det_Nums[i] = FEBEX_det_ids[i];
	    GALILEO_sum_time[i] = sum_time[i];
	    GALILEO_pileup[i] = pileup[i];
	    GALILEO_hit_pattern[i] = hit_pattern[i];
	    GALILEO_chan_time[i] = chan_time[i];
	    GALILEO_chan_energy[i] = chan_en[i];

	}

    Event_Type = 4;
}
//TEMPORARY GETTERS FOR FRS, FATIMA, PLASTIC, and GALILEO

// #############################################################

// FRS

Float_t Raw_Event::get_FRS_dE(int i){return dE[i];}
Float_t Raw_Event::get_FRS_dE_corr(int i){return dE_cor[i];}

Float_t Raw_Event::get_FRS_sci_l(int i){return sci_l[i];}
Float_t Raw_Event::get_FRS_sci_r(int i){return sci_r[i];}
Float_t Raw_Event::get_FRS_sci_e(int i){return sci_e[i];}
Float_t Raw_Event::get_FRS_sci_tx(int i){return sci_tx[i];}
Float_t Raw_Event::get_FRS_sci_x(int i){return sci_x[i];}


Int_t Raw_Event::get_FRS_dt_21l_21r(){return dt_21l_21r;} 
Int_t Raw_Event::get_FRS_dt_41l_41r(){return dt_41l_41r;}
Int_t Raw_Event::get_FRS_dt_21l_41l(){return dt_21l_41l;}
Int_t Raw_Event::get_FRS_dt_21r_41r(){return dt_21r_41r;}
Int_t Raw_Event::get_FRS_dt_42l_42r(){return dt_42l_42r;}
Int_t Raw_Event::get_FRS_dt_43l_43r(){return dt_43l_43r;}
Int_t Raw_Event::get_FRS_dt_21l_42l(){return dt_21l_42l;}
Int_t Raw_Event::get_FRS_dt_21r_42r(){return dt_21r_42r;}
Int_t Raw_Event::get_FRS_dt_81l_81r(){return dt_81l_81r;}
Int_t Raw_Event::get_FRS_dt_21l_81l(){return dt_21l_81l;}
Int_t Raw_Event::get_FRS_dt_21r_81r(){return dt_21r_81r;}

Float_t Raw_Event::get_FRS_tofll2(){return sci_tofll2;}
Float_t Raw_Event::get_FRS_tofll3(){return sci_tofll3;}
Float_t Raw_Event::get_FRS_tof2(){return sci_tof2;}
Float_t Raw_Event::get_FRS_tofrr2(){return sci_tofrr2;}
Float_t Raw_Event::get_FRS_tofrr3(){return sci_tofrr3;}
Float_t Raw_Event::get_FRS_tof3(){return sci_tof3;}

Float_t Raw_Event::get_FRS_x2(){return ID_x2;}
Float_t Raw_Event::get_FRS_y2(){return ID_y2;}
Float_t Raw_Event::get_FRS_a2(){return ID_a2;}
Float_t Raw_Event::get_FRS_b2(){return ID_b2;}

Float_t Raw_Event::get_FRS_x4(){return ID_x4;}
Float_t Raw_Event::get_FRS_y4(){return ID_y4;}
Float_t Raw_Event::get_FRS_a4(){return ID_a4;}
Float_t Raw_Event::get_FRS_b4(){return ID_b4;}

Float_t Raw_Event::get_FRS_brho(int i){return ID_brho[i];}
Float_t Raw_Event::get_FRS_rho(int i){return ID_rho[i];}

Float_t Raw_Event::get_FRS_beta(){return beta;}
Float_t Raw_Event::get_FRS_beta3(){return beta3;}
Float_t Raw_Event::get_FRS_gamma(){return gamma;}

Float_t Raw_Event::get_FRS_AoQ(){return AoQ;}
Float_t Raw_Event::get_FRS_AoQ_corr(){return AoQ_corr;}
Float_t Raw_Event::get_FRS_z(){return z;}
Float_t Raw_Event::get_FRS_z2(){return z2;}
Float_t Raw_Event::get_FRS_z3(){return z3;}

Float_t Raw_Event::get_FRS_timestamp(){return timestamp;}
Float_t Raw_Event::get_FRS_ts(){return ts;}
Float_t Raw_Event::get_FRS_ts2(){return ts2;}

// ####################################################### 

//White Rabbit

//---------------------------------------------------------------

void Raw_Event::set_WR(ULong64_t WR){this->WR = WR;}

//---------------------------------------------------------------

ULong64_t Raw_Event::get_WR(){return WR;}

//---------------------------------------------------------------


//FATIMA

//---------------------------------------------------------------

      int Raw_Event::get_FAT_det_fired(){return FAT_DET_FIRED;}
	  int Raw_Event::get_FAT_id(int i){return FAT_id[i];}
   double Raw_Event::get_FAT_E(int i){return FAT_E[i];}
   double Raw_Event::get_FAT_ratio(int i){return FAT_ratio[i];}
   double Raw_Event::get_FAT_t(int i){return FAT_t[i];}
   double Raw_Event::get_FAT_t_qdc(int i){return FAT_t_qdc[i];}

      int Raw_Event::get_FAT_QDCs_fired(){return FAT_QDCs_FIRED;}
      int Raw_Event::get_FAT_QDC_id(int i){return FAT_QDC_id[i];}
   double Raw_Event::get_FAT_QLong(int i){return FAT_QLong[i];}
   double Raw_Event::get_FAT_QShort_Raw(int i){return FAT_QShort_Raw[i];}
   double Raw_Event::get_FAT_QLong_Raw(int i){return FAT_QLong_Raw[i];}
ULong64_t Raw_Event::get_FAT_QDC_t_Coarse(int i){return FAT_QDC_t_coarse[i];}
   double Raw_Event::get_FAT_QDC_t_Fine(int i){return FAT_QDC_t_fine[i];}

      int Raw_Event::get_FAT_TDCs_fired(){return FAT_TDCs_FIRED;}
      int Raw_Event::get_FAT_TDC_id(int i){return FAT_TDC_id[i];}
   double Raw_Event::get_FAT_TDC_timestamp(int i){return FAT_TDC_timestamp[i];}


FATIMA_DataStruct* Raw_Event::PassFATIMA(){return &FATIMA_Data;}


//---------------------------------------------------------------


//---------------------------------------------------------------


//---------------------------------------------------------------


	
//---------------------------------------------------------------

//PLASTIC

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_tamex_hits(){
    return PLASTIC_Data.amount_hit_tamex;
}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_am_Fired(int i){
    return PLASTIC_Data.iterator[i];
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_trigger_T(int i){
    return (PLASTIC_Data.trigger_coarse[i] - PLASTIC_Data.trigger_fine[i]);
}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_CH_ID(int i,int j){
    return PLASTIC_Data.ch_ID[i][j];
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_lead_T(int i,int j){
	//cout << "SEND l" << coarse_T_edge_lead[i][j] << " " << fine_T_edge_lead[i][j]  << " " <<  coarse_T_edge_lead[i][j]*5 - fine_T_edge_lead[i][j] << endl;
	return PLASTIC_Data.Time_Lead[i][j];
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_coarse_lead(int i,int j){
	//cout << "SEND l" << coarse_T_edge_lead[i][j] << " " << fine_T_edge_lead[i][j]  << " " <<  coarse_T_edge_lead[i][j]*5 - fine_T_edge_lead[i][j] << endl;
	return PLASTIC_Data.coarse_T_edge_lead[i][j];
}


//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_trail_T(int i,int j){
	//cout << "SEND t" << coarse_T_edge_trail[i][j] << " " << fine_T_edge_trail[i][j] << endl;
	return PLASTIC_Data.Time_Trail[i][j];
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_TOT(int i,int j){
    // i is board ID, j is physical channel
    
    double T_lead = PLASTIC_Data.Time_Lead[i][j];
    double T_trail = PLASTIC_Data.Time_Trail[i][j];

    return T_trail - T_lead;
}

//---------------------------------------------------------------


int Raw_Event::get_PLASTIC_trail_hits(int i){
    return PLASTIC_Data.trailing_hits[i];
}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_lead_hits(int i){
    return PLASTIC_Data.leading_hits[i];
}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_physical_channel(int i,int j){
    return PLASTIC_Data.phys_channel[i][j];
}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_physical_lead_hits(int i,int j){
    return PLASTIC_Data.leading_hits_ch[i][j];
}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_physical_trail_hits(int i,int j){
    return PLASTIC_Data.trailing_hits_ch[i][j];
}

//---------------------------------------------------------------

PLASTIC_VME_DataStruct* Raw_Event::PassPLASTIC_VME(){
    return &PLASTIC_VME_Data;
}

//---------------------------------------------------------------

PLASTIC_DataStruct* Raw_Event::PassPLASTIC(){
    return &PLASTIC_Data;
}

//---------------------------------------------------------------
//---------------------------------------------------------------

//GALILEO

//---------------------------------------------------------------

int Raw_Event::get_GALILEO_am_Fired(){return GAL_FIRED;}

//---------------------------------------------------------------

ULong64_t Raw_Event::get_GALILEO_Sum_T(int i){return GALILEO_sum_time[i];}

//---------------------------------------------------------------

int Raw_Event::get_GALILEO_Pileup(int i){return GALILEO_pileup[i];}

//---------------------------------------------------------------

int Raw_Event::get_GALILEO_Hit_Pattern(int i){return GALILEO_hit_pattern[i];}

//---------------------------------------------------------------

ULong64_t Raw_Event::get_GALILEO_Chan_T(int i){return GALILEO_chan_time[i];}

//---------------------------------------------------------------

double Raw_Event::get_GALILEO_Chan_E(int i){return GALILEO_chan_energy[i];}

//---------------------------------------------------------------

