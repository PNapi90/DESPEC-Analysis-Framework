#include "Raw_Event.h"

#include <iostream>

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event(){}

//---------------------------------------------------------------

Raw_Event::~Raw_Event(){}

//---------------------------------------------------------------

void Raw_Event::set_DATA_FATIMA(int FAT_FIRED,int TDC_FIRED,double* Ql,double* Qs,ULong* TDC,ULong* QDC_c,ULong* QDC_f,int* det_ids_QDC,int* det_ids_TDC){
	this->FAT_FIRED = FAT_FIRED;
	this->TDC_FIRED = TDC_FIRED;

	int position = -5;
	int active_det = 0;

	for(int i = 0;i < 50;++i) used_for_QDC[i] = false;
	//all correlated tdcs and qdcs
	for(int i = 0;i < FAT_FIRED;++i){

		active_det = det_ids_QDC[i];

		used_for_QDC[active_det] = false;
		for(int j = 0;j < TDC_FIRED;++j){
			if(det_ids_QDC[i] == det_ids_TDC[j]){
				position = det_ids_TDC[j];
				used_for_QDC[position] = true;
				//cout << "USED POS " << j << endl;
				break;
			}
		}
		Det_Nums[i] = det_ids_QDC[i];
		E[i] = Ql[active_det];
		QShort[i] = Qs[active_det];
		QDC_t_coarse[i] = QDC_c[active_det];
		QDC_t_fine[i] = QDC_f[active_det];
		TDC_timestamp[i] = TDC[position];
	}

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

bool Raw_Event::CH_51_FIRED(){return ch51;};

//---------------------------------------------------------------

double Raw_Event::get_FATIMA_Time_Diff(){return time_difference;}

//---------------------------------------------------------------

int Raw_Event::get_FATIMA_am_Fired_TDC(){return TDC_FIRED;}

//---------------------------------------------------------------

bool Raw_Event::get_FATIMA_QDC_TDC_LINKED(int i){return used_for_QDC[i];}

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
	return (coarse_T_edge_trail[i][j]*10 - fine_T_edge_trail[i][j]);
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









=======
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
>>>>>>> 6b998c4624cf4f21d81814c494c4864e0a622cd5