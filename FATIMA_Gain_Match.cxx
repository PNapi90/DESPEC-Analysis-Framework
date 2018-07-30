#include "FATIMA_Gain_Match.h"

#include <cstdlib>
#include <string>


using namespace std;

//----------------------------------------------------------

FATIMA_Gain_Match::FATIMA_Gain_Match(string GM_filename){
    load_Gain_Match_File(GM_filename);

}


//----------------------------------------------------------

FATIMA_Gain_Match::~FATIMA_Gain_Match(){}

//----------------------------------------------------------

void FATIMA_Gain_Match::load_Gain_Match_File(string GM_filename){
    
    const char* format = "%d %lf %lf";

    ifstream gain_match_file(GM_filename.c_str());

    if(gain_match_file.fail()){
        cerr << "Could not find FATIMA Gain Match file" << endl;
        exit(0);
    }
    if (gain_match_file.is_open()) cout<<"FATIMA Gain Match file:  "<<GM_filename<<"  was opened sucessfully"<<endl;
    
    string line;

    double tmp_coeffs[2] = {0,0};

    int det_id = 0;

    while(gain_match_file.good()){
        getline(gain_match_file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&det_id,&tmp_coeffs[0],&tmp_coeffs[1]);
	

        for(int i = 0;i < 2;++i) gain_match_coeffs[det_id][i] = tmp_coeffs[i];
	
    }
}

//----------------------------------------------------------

double FATIMA_Gain_Match::Gain_Match(double E,int det_id){
    double Energy_gm = gain_match_coeffs[det_id][0]*E + gain_match_coeffs[det_id][1];
    return Energy_gm;
}

//----------------------------------------------------------


