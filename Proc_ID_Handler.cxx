#include "Proc_ID_Handler.h"

using namespace std;

Proc_ID_Handler::Proc_ID_Handler(){
    load_Proc_ID_File();
}

//---------------------------------------------------------------

Proc_ID_Handler::~Proc_ID_Handler(){}

//---------------------------------------------------------------

void Proc_ID_Handler::load_Proc_ID_File(){
    
    ifstream proc_file("Configuration_Files/Proc_ID_Map.dat");
    if(proc_file.fail()){
        cerr << "Could not find Proc_ID map file!" << endl;
        exit(0);
    }

    //load file (and skip header)
    string line;
    while(proc_file.good()){
        getline(proc_file,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),format,&Proc_ID_Arr[0],&Proc_ID_Arr[1]
                           ,&Proc_ID_Arr[2],&Proc_ID_Arr[3],&Proc_ID_Arr[4]);
    }
}

//---------------------------------------------------------------

int Proc_ID_Handler::get_Detector_type(int proc_id){
    //look for detector id in map
    for(int i = 0;i < 5;++i){
        if(proc_id == Proc_ID_Arr[i]) return i;
    }
    cerr << "Unknown proc_id " << proc_id << " -> exiting program !" << endl;
    exit(0);
}

//---------------------------------------------------------------