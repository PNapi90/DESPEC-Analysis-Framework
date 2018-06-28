#include "Raw_Event.h"

using namespace std;

//---------------------------------------------------------------

Raw_Event::Raw_Event(){}

//---------------------------------------------------------------

Raw_Event::~Raw_Event(){}

//---------------------------------------------------------------

void Raw_Event::set_DATA_FATIMA(int FAT_FIRED,double* Ql,double* Qs,ULong* TDC,ULong* QDC_c,ULong* QDC_f){
	this->FAT_FIRED = FAT_FIRED;
	for(int i = 0;i < FAT_FIRED;++i){
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