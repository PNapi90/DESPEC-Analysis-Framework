#include "PLASTIC_Calibrator.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Calibrator::PLASTIC_Calibrator(){
	load_Calibration_File();
}

//---------------------------------------------------------------

PLASTIC_Calibrator::~PLASTIC_Calibrator(){

}

//---------------------------------------------------------------

void PLASTIC_Calibrator::load_Calibration_File(){
	
	ifstream file("Configuration_Files/PLASTIC_Calibration.txt");
	string line;
	
	if(file.fail()){
		cerr << "Could not find PLASTIC Calibration file" << endl;
		exit(0);
	}
	
	const char* format = "%d %d %lf %lf %lf %lf";

	int tamex_id,ch_id;
	double val_lead_coarse,val_trail_coarse,val_lead_fine,val_trail_fine;

	while(file.good()){
		getline(file,line,'\n');
		if(line[0] == '#') continue;
		sscanf(line.c_str(),format,&tamex_id,&ch_id,&val_lead_coarse,&val_trail_coarse,&val_lead_fine,&val_trail_fine);
		cal_arr_c[tamex_id][ch_id][0] = val_lead_coarse;
		cal_arr_c[tamex_id][ch_id][1] = val_trail_coarse;
		cal_arr_f[tamex_id][ch_id][0] = val_lead_fine;
		cal_arr_f[tamex_id][ch_id][1] = val_trail_fine;
	}
}

//---------------------------------------------------------------

void PLASTIC_Calibrator::calibrate(ULong** coarse_T,ULong** fine_T,UInt** ch_id,int tamex_id,int am_fired){
	this->am_fired = am_fired;
	for(int i = 0;i < am_fired;++i){
		for(int j = 0;j < 2;++j){
			coarse_T[i][j] += cal_arr_c[tamex_id][ch_id[i][0]][j];
			fine_T[i][j] += cal_arr_f[tamex_id][ch_id[i][0]][j];
		}
	}
}

//---------------------------------------------------------------

void PLASTIC_Calibrator::calibrate(ULong coarse_T,ULong fine_T,ULong* Container,int tamex_id){
	Container[0] = coarse_T + cal_arr_c[tamex_id][am_fired][0];
	Container[1] = fine_T + cal_arr_f[tamex_id][am_fired][0];
}

//---------------------------------------------------------------
