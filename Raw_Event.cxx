#include "Raw_Event.h"

#include <iostream>

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event(){}

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

void Raw_Event::set_DATA_FATIMA(int QDC_FIRED,int TDC_FIRED,
								double* Ql_Raw,double* Qs_Raw,
								double* Ql,
								ULong64_t* TDC, double* TDC_ns,
								ULong64_t* QDC_c, double* QDC_f,
								int* det_ids_QDC,int* det_ids_TDC){
	
	
	this->FAT_QDCs_FIRED = QDC_FIRED;
	this->FAT_TDCs_FIRED = TDC_FIRED;
	int dets_fired = 0;
	for (int i=0; i<QDC_FIRED; i++) {
		this->FAT_QDC_id[i] = det_ids_QDC[i];
		this->FAT_QLong[i]  = Ql[i];
		this->FAT_QLong_Raw[i]  = Ql_Raw[i];
		this->FAT_QShort_Raw[i] = Qs_Raw[i];
		this->FAT_QDC_t_coarse[i] = QDC_c[i];
		this->FAT_QDC_t_fine[i] = QDC_f[i];
		for (int j=0; j<TDC_FIRED; j++) {
			if (det_ids_QDC[i] == det_ids_TDC[j]) {
				this->FAT_id[dets_fired] = det_ids_QDC[i];
				this->FAT_E[dets_fired] = Ql[i];
				this->FAT_ratio[dets_fired] = (double) Qs_Raw[i]/Ql_Raw[i];
				this->FAT_t[dets_fired] = TDC_ns[j];
				this->FAT_t_qdc[dets_fired] = QDC_c[i];
				dets_fired++;
			}
		}
	}
	this->FAT_DET_FIRED = dets_fired;
	
	for (int i=0; i<TDC_FIRED; i++) {
		this->FAT_TDC_id[i]        = det_ids_TDC[i];
		this->FAT_TDC_timestamp[i] = TDC[i];
	}

	
	
	
	
	/*this->FAT_FIRED = FAT_FIRED;
	this->TDC_FIRED = TDC_FIRED;

	int position = -5;
	int active_det = 0;
	BOTH_FIRED = 0;

	for(int i = 0;i < 50;++i) used_for_QDC[i] = false;
	//all correlated tdcs and qdcs
	for(int i = 0;i < FAT_FIRED;++i){

		active_det = det_ids_QDC[i];
		//cout<<"From RAW EVENT:"<<endl;;

		//cout<<active_det<<" ";

		//used_for_QDC[active_det] = false;
		for(int j = 0;j < TDC_FIRED;++j){
			if(det_ids_QDC[i] == det_ids_TDC[j]){
				position = det_ids_TDC[j];
				used_for_QDC[position] = true;
				//Det_Nums[i] = det_ids_QDC[i];
				FAT_Both[BOTH_FIRED] = i;
				++BOTH_FIRED;
				//cout << "USED POS " << j << endl;
				break;
			}
		}
		Det_Nums[i] = det_ids_QDC[i];
		E_Raw[i] = Ql_Raw[active_det];
		QShort_Raw[i] = Qs_Raw[active_det];
		E[i] = Ql[active_det];
		QShort[i] = Qs[active_det];
		QDC_t_coarse[i] = QDC_c[active_det];
		QDC_t_fine[i] = QDC_f[active_det];
		TDC_timestamp[i] = TDC[position];
	}
	
	//cout<<endl;

	ch51 = false;
	if(FAT_FIRED == 0) return;
	
	//remaining tdcs
	for(int i = FAT_FIRED;i < TDC_FIRED;++i){
		active_det = det_ids_TDC[i];
		//if(!ch51 && active_det == 51 && position != -5) ch51 = true;
		if(!used_for_QDC[active_det]){
			Det_Nums[i] = det_ids_TDC[active_det];
			TDC_timestamp[i] = TDC[active_det];
;
			if(active_det == 51 && position != -5 && !ch51 && TDC_FIRED == 3){
				ch51 = true;
				time_difference = ((double) TDC_timestamp[i]) - ((double) TDC_timestamp[0]);
			}
		}
	}
	if(TDC_FIRED != FAT_FIRED && false){
		cout << "FAT " << FAT_FIRED << " " << TDC_FIRED << endl;
		for(int i = 0;i < TDC_FIRED;++i) cout << Det_Nums[i] << " " << used_for_QDC[i] << " | ";
		cout << endl;

	}
	*/
}

//---------------------------------------------------------------

void Raw_Event::set_DATA_PLASTIC(int* it,double** Edge_Coarse,double** Edge_fine,UInt** ch_ed,double* Coarse_Trigger,double* Fine_Trigger){

	//reset lead and trail hits
	for(int i = 0;i < 4;++i){
		for(int j = 0;j < 17;++j){
			leading_hits_ch[i][j] = 0;
			trailing_hits_ch[i][j] = 0;
		}
	}

	//loop over all 4 tamex modules
	for(int i = 0;i < 4;++i){
		iterator[i] = it[i];
		trigger_coarse[i] = Coarse_Trigger[i];
		trigger_fine[i] = Fine_Trigger[i];
		fired_tamex[i] = (iterator[i] > 0);
		leading_hits[i] = 0;
		trailing_hits[i] = 0;
		for(int j = 0;j < iterator[i];++j){
			ch_ID[i][j] = ch_ed[i][j];
			if(ch_ID[i][j] % 2 == 1){
				coarse_T_edge_lead[i][j] = (double) Edge_Coarse[i][j];
				fine_T_edge_lead[i][j] = (double) Edge_fine[i][j];
				
				phys_channel[i][j] = (ch_ID[i][j]+1)/2;
				leading_hits[i]++;
				leading_hits_ch[i][phys_channel[i][j]]++;
			}
			else{
				coarse_T_edge_trail[i][j] = (double)  Edge_Coarse[i][j];
				fine_T_edge_trail[i][j] =(double)  Edge_fine[i][j];
				
				trailing_hits[i]++;
				phys_channel[i][j] = (ch_ID[i][j])/2;
				trailing_hits_ch[i][phys_channel[i][j]]++;

			}
		}	
	}
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


//---------------------------------------------------------------


//---------------------------------------------------------------


//---------------------------------------------------------------


	
//---------------------------------------------------------------

//PLASTIC

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_am_Fired(int i){return iterator[i];}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_trigger_T(int i){return (trigger_coarse[i] - trigger_fine[i]);}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_CH_ID(int i,int j){return ch_ID[i][j];}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_lead_T(int i,int j){
	//cout << "SEND l" << coarse_T_edge_lead[i][j] << " " << fine_T_edge_lead[i][j]  << " " <<  coarse_T_edge_lead[i][j]*5 - fine_T_edge_lead[i][j] << endl;
	return (coarse_T_edge_lead[i][j]*5 - fine_T_edge_lead[i][j]);
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_coarse_lead(int i,int j){
	//cout << "SEND l" << coarse_T_edge_lead[i][j] << " " << fine_T_edge_lead[i][j]  << " " <<  coarse_T_edge_lead[i][j]*5 - fine_T_edge_lead[i][j] << endl;
	return coarse_T_edge_lead[i][j];
}


//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_trail_T(int i,int j){
	//cout << "SEND t" << coarse_T_edge_trail[i][j] << " " << fine_T_edge_trail[i][j] << endl;
	return (coarse_T_edge_trail[i][j]*5 - fine_T_edge_trail[i][j]);
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_TOT(int i,int j){
    // i is board ID, j is physical channel
    double T_lead = (coarse_T_edge_lead[i][j]*5 - fine_T_edge_lead[i][j]);
    double T_trail = (coarse_T_edge_trail[i][j+1]*5 - fine_T_edge_trail[i][j+1]);

    return T_trail - T_lead;
}

//---------------------------------------------------------------


int Raw_Event::get_PLASTIC_trail_hits(int i){return trailing_hits[i];}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_lead_hits(int i){return leading_hits[i];}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_physical_channel(int i,int j){return phys_channel[i][j];}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_physical_lead_hits(int i,int j){return leading_hits_ch[i][j];}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_physical_trail_hits(int i,int j){return trailing_hits_ch[i][j];}

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

