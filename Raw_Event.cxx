#include "Raw_Event.h"

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event(){}

//---------------------------------------------------------------

Raw_Event::~Raw_Event(){}

//---------------------------------------------------------------

void Raw_Event::set_DATA_FATIMA(int FAT_FIRED,double* Ql,ULong* TDC,ULong* QDC_c,ULong* QDC_f,int* det_ids){
	this->FAT_FIRED = FAT_FIRED;
	for(int i = 0;i < FAT_FIRED;++i){
		Det_Nums[i] = det_ids[i];
		E[i] = Ql[i];
		//QShort[i] = Qs[i]; still missing!
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

//TEMPORARY GETTERS FOR FATIMA AND PLASTIC

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

