#include "FATIMA_Time_Calibration.h"

using namespace std;

//----------------------------------------------------------

FATIMA_Time_Calibration::FATIMA_Time_Calibration(string filename){
    this->filename = filename;
    load_Calibration_File();
}

//----------------------------------------------------------

FATIMA_Time_Calibration::~FATIMA_Time_Calibration(){}

//----------------------------------------------------------

void FATIMA_Time_Calibration::load_Calibration_File(){
    
    ifstream calib_file(filename);

    if(calib_file.fail()){
        cerr << "Could not find Fatima Time Calibration file" << endl;
        //exit(0);
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
}

//----------------------------------------------------------

double FATIMA_Time_Calibration::Calibrate(double T,int det_id){
    return T + calib_coeffs[det_id];
}

//----------------------------------------------------------