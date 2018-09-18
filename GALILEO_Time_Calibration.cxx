#include "GALILEO_Time_Calibration.h"

#include <cstdlib>

using namespace std;

//----------------------------------------------------------

GALILEO_Time_Calibration::GALILEO_Time_Calibration(){
    load_Calibration_File();
}

//----------------------------------------------------------

GALILEO_Time_Calibration::~GALILEO_Time_Calibration(){}

//----------------------------------------------------------

void GALILEO_Time_Calibration::load_Calibration_File(){
    

    const char* format = "%d %lf %lf";

    ifstream calib_file("Configuration_Files/GALILEO_Time_Calibration.txt");

    if(calib_file.fail()){
        cerr << "Could not find GALILEO Time Calibration file" << endl;
        exit(0);
    }
    string line;

    double tmp_sum_coeffs;
    double tmp_chan_coeffs;

    int det_id;

    while(getline(calib_file,line,'\n')){
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&det_id,&tmp_sum_coeffs,&tmp_chan_coeffs);

        calib_coeffs_sum[det_id] = tmp_sum_coeffs;
        calib_coeffs_channels[det_id] = tmp_chan_coeffs;
        
    }
}

//----------------------------------------------------------

ULong GALILEO_Time_Calibration::Calibrate_FEBEX_Sum_T(ULong Sum_T,int det_id){
    return Sum_T + calib_coeffs_sum[det_id];
}

//----------------------------------------------------------

ULong GALILEO_Time_Calibration::Calibrate_FEBEX_Chan_T(ULong Chan_T,int det_id){
    return Chan_T + calib_coeffs_channels[det_id];
}

//----------------------------------------------------------
