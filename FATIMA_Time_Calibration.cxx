#include "FATIMA_Time_Calibration.h"

#include <cstdlib>

using namespace std;

//----------------------------------------------------------

FATIMA_Time_Calibration::FATIMA_Time_Calibration(){
    load_Calibration_File();
}

//----------------------------------------------------------

FATIMA_Time_Calibration::~FATIMA_Time_Calibration(){}

//----------------------------------------------------------

void FATIMA_Time_Calibration::load_Calibration_File(){
    

    const char* format = "%d %lf";

    ifstream calib_file("Configuration_Files/FATIMA_Time_Calibration.txt");

    if(calib_file.fail()){
        cerr << "Could not find Fatima Time Calibration file" << endl;
        exit(0);
    }
    string line;

    double tmp_coeffs;

    int det_id;

    while(calib_file.good()){
        getline(calib_file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&det_id,&tmp_coeffs);

        calib_coeffs[det_id] = tmp_coeffs;
    }

    calib_file.close();
    calib_file.clear();
    calib_file.open("Configuration_Files/FATIMA_QDC_Time_Calibration.txt");

    if(calib_file.fail()){
        cerr << "Could not find Fatima QDC Time Calibration file" << endl;
        exit(0);
    }

    while(calib_file.good()){
        getline(calib_file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&det_id,&tmp_coeffs);

        calib_coeffs_QDC[det_id] = tmp_coeffs;
    }
}

//----------------------------------------------------------

double FATIMA_Time_Calibration::Calibrate(double T,int det_id){
    return T + calib_coeffs[det_id];
}

//----------------------------------------------------------

double FATIMA_Time_Calibration::Calibrate_QDC(double T,int det_id){
    return T + calib_coeffs_QDC[det_id];
}

//----------------------------------------------------------
