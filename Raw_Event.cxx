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

	int position = 0;
	int active_det = 0;

	for(int i = 0;i < 50;++i) used_for_QDC[i] = false;
	//all correlated tdcs and qdcs
	for(int i = 0;i < FAT_FIRED;++i){

		active_det = det_ids_QDC[i];

		used_for_QDC[active_det] = false;
		for(int j = 0;j < TDC_FIRED;++j){
			if(det_ids_QDC[i] == det_ids_TDC[j]){
				position = j;
				used_for_QDC[j] = true;
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
	//remaining tdcs
	for(int i = 0;i < TDC_FIRED;++i){
		active_det = det_ids_TDC[i];
		if(!used_for_QDC[active_det]){
			Det_Nums[i+FAT_FIRED] = det_ids_TDC[active_det];
			TDC_timestamp[i+FAT_FIRED] = TDC[active_det];
		}
	}
	if(TDC_FIRED != FAT_FIRED && false){
		cout << "FAT " << FAT_FIRED << " " << TDC_FIRED << endl;
		for(int i = 0;i < TDC_FIRED;++i) cout << Det_Nums[i] << " " << used_for_QDC[i] << " | ";
		cout << endl;

	}
}

//---------------------------------------------------------------

void Raw_Event::set_DATA_PLASTIC(int* it,ULong** Edge_Coarse,ULong** Edge_fine,UInt** ch_ed,ULong* Coarse_Trigger,ULong* Fine_Trigger){

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
				coarse_T_edge_lead[i][j] = Edge_Coarse[i][j];
				fine_T_edge_lead[i][j] = Edge_fine[i][j];
				leading_hits[i]++;
			}
			else{
				coarse_T_edge_trail[i][j] = Edge_Coarse[i][j];
				trailing_hits[i]++;
				fine_T_edge_trail[i][j] = Edge_fine[i][j];	
			}
		}	
	}
}

//TEMPORARY GETTERS FOR FATIMA AND PLASTIC

//FATIMA

//---------------------------------------------------------------

int Raw_Event::get_FATIMA_am_Fired(){return FAT_FIRED;}

//---------------------------------------------------------------

int Raw_Event::get_FATIMA_am_Fired_TDC(){return TDC_FIRED;}

//---------------------------------------------------------------

bool Raw_Event::get_FATIMA_QDC_TDC_LINKED(int i ){return used_for_QDC[i];}

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

int Raw_Event::get_PLASTIC_trail_hits(int i){return trailing_hits[i];}

//---------------------------------------------------------------

int Raw_Event::get_PLASTIC_lead_hits(int i){return leading_hits[i];}

//---------------------------------------------------------------