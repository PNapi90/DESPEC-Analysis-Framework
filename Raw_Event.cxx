#include "Raw_Event.h"

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event(){}

//---------------------------------------------------------------

Raw_Event::~Raw_Event(){}

//---------------------------------------------------------------

void Raw_Event::set_DATA_FATIMA(int FAT_FIRED,double* Ql,double* Qs,ULong* TDC,ULong* QDC_c,ULong* QDC_f,int* det_ids){
	this->FAT_FIRED = FAT_FIRED;
	for(int i = 0;i < FAT_FIRED;++i){
		Det_Nums[i] = det_ids[i];
		E[i] = Ql[i];
		QShort[i] = Qs[i];
		TDC_timestamp[i] = TDC[i];
		QDC_t_coarse[i] = QDC_c[i];
		QDC_t_fine[i] = QDC_f[i];
	}
}

//---------------------------------------------------------------

void Raw_Event::set_DATA_PLASTIC(int* it,ULong*** Edge_Coarse,ULong*** Edge_fine,UInt*** ch_ed,ULong* Coarse_Trigger,ULong* Fine_Trigger){

	//loop over all 4 tamex modules
	for(int i = 0;i < 4;++i){
		iterator[i] = it[i];
		trigger_coarse[i] = Coarse_Trigger[i];
		trigger_fine[i] = Fine_Trigger[i];
		fired_tamex[i] = (iterator[i] > 0);
		for(int j = 0;j < iterator[i];++j){
			ch_ID[i][j] = ch_ed[i][j][0];
			coarse_T_edge_lead[i][j] = Edge_Coarse[i][j][0];
			coarse_T_edge_trail[i][j] = Edge_Coarse[i][j][1];

			fine_T_edge_lead[i][j] = Edge_fine[i][j][0];
			fine_T_edge_trail[i][j] = Edge_fine[i][j][1];	
		}
	}
}

//---------------------------------------------------------------

void Raw_Event::set_DATA_GALILEO(int GAL_FIRED,ULong* sum_time,int* pileup,int* hit_pattern,ULong* chan_time,double* chan_en, int* FEBEX_det_ids){
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
//TEMPORARY GETTERS FOR FATIMA, PLASTIC, and GALILEO

//FATIMA

//---------------------------------------------------------------

int Raw_Event::get_FATIMA_am_Fired(){return FAT_FIRED;}

//---------------------------------------------------------------

double Raw_Event::get_FATIMA_E(int i){return E[i];}

//---------------------------------------------------------------

ULong Raw_Event::get_FATIMA_TDC_T(int i){return TDC_timestamp[i];}

//---------------------------------------------------------------

ULong Raw_Event::get_FATIMA_QDC_T_Coarse(int i){return QDC_t_coarse[i];}

//---------------------------------------------------------------

ULong Raw_Event::get_FATIMA_QDC_T_Fine(int i){return QDC_t_fine[i];}

//---------------------------------------------------------------

int Raw_Event::get_FATIMA_det_id(int i){return Det_Nums[i];}

//---------------------------------------------------------------

//PLASTIC

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_am_Fired(int i){return iterator[i];}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_trigger_T(int i){return (double) (trigger_coarse[i] - trigger_fine[i]);}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_CH_ID(int i,int j){return ch_ID[i][j];}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_lead_T(int i,int j){
	return (double)(coarse_T_edge_lead[i][j] - fine_T_edge_lead[i][j]);
}

//---------------------------------------------------------------

double Raw_Event::get_PLASTIC_trail_T(int i,int j){
	return (double)(coarse_T_edge_trail[i][j] - fine_T_edge_trail[i][j]);
}

//---------------------------------------------------------------

//GALILEO

//---------------------------------------------------------------

int Raw_Event::get_GALILEO_am_Fired(){return GAL_FIRED;}

//---------------------------------------------------------------

ULong Raw_Event::get_GALILEO_Sum_T(int i){return GALILEO_sum_time[i];}

//---------------------------------------------------------------

int Raw_Event::get_GALILEO_Pileup(int i){return GALILEO_pileup[i];}

//---------------------------------------------------------------

int Raw_Event::get_GALILEO_Hit_Pattern(int i){return GALILEO_hit_pattern[i];}

//---------------------------------------------------------------

ULong Raw_Event::get_GALILEO_Chan_T(int i){return GALILEO_chan_time[i];}

//---------------------------------------------------------------

double Raw_Event::get_GALILEO_Chan_E(int i){return GALILEO_chan_energy[i];}

//---------------------------------------------------------------









