#include "AIDA_Data_Streamer.h"

using namespace std;

//---------------------------------------------------------------

AIDA_Data_Streamer::AIDA_Data_Streamer(int max_len){
    this->max_len = max_len;
    Time = new ULong64_t*[z_strip_amount*2];
    x_or_y = new bool*[z_strip_amount*2];
    Energy = new double*[z_strip_amount*2];
    x_coord = new int*[z_strip_amount*2];

    for(int i = 0;i < z_strip_amount*2;++i){
        Time[i] = new ULong64_t[max_len];
        x_or_y[i] = new bool[max_len];
        Energy[i] = new double[max_len];
        x_coord[i] = new int[max_len];
        for(int j = 0;j < max_len;++j){
            Time[i][j] = 0;
            x_or_y[i][j] = false;
            Energy[i][j] = 0;
            x_coord[i][j] = 0;
        }
    }
    array_iterator = new int[z_strip_amount*2];
    row_counter = new int[z_strip_amount];
    for(int i = 0;i < z_strip_amount;++i) row_counter[i] = 0;
    for(int i = 0;i < z_strip_amount*2;++i) array_iterator[i] = 0;
    //set conversion array values to -1 for not used AIDA z strips
    for(int i = 0;i < 2;++i) for(int j = 0;j < 100;++j) conversion_array[i][j] = -1;

    load_conversion_array();
    reset_iterators();
}

//---------------------------------------------------------------

AIDA_Data_Streamer::~AIDA_Data_Streamer(){

    for(int i = 0;i < z_strip_amount*2;++i){

        if(Time[i]) delete[] Time[i];
        if(x_or_y[i]) delete[] x_or_y[i];
        if(Energy[i]) delete[] Energy[i];
        if(x_coord[i]) delete[] x_coord[i];
    }
    delete[] Time;
    delete[] x_or_y;
    delete[] Energy;
    delete[] x_coord;
    delete[] array_iterator;
    delete[] row_counter;

}

//---------------------------------------------------------------

inline int AIDA_Data_Streamer::get_Coord(int z_strip,bool x_or_y){
    return (x_or_y) ? z_strip + 1 : z_strip;
}

//---------------------------------------------------------------


void AIDA_Data_Streamer::set_DATA(AIDA_Decay_Event_Store* Store){
    
    //get Data from AIDA_Decay_Event_Store
    bool* x_or_y_tmp = Store->get_x_or_y();
    int* x_coord_tmp = Store->get_x_coord();
    ULong64_t* Time_tmp = Store->get_Time();
    double* Energy_tmp = Store->get_Energy();
    int* z_strip_tmp = Store->get_z_strip();
    int amount_tmp = Store->get_amount_of_events();

    int array_position = 0;
    for(int i = 0;i < amount_tmp;++i){
        //get xyz position from conversion array
        array_position = conversion_array[x_or_y_tmp[i]][z_strip_tmp[i]];
        //check if array position unknown
        check_array_position(array_position,z_strip_tmp[i]);
        
        //set values
        x_coord[array_position][array_iterator[array_position]] = x_coord_tmp[i];
        Time[array_position][array_iterator[array_position]] = Time_tmp[i];
        Energy[array_position][array_iterator[array_position]] = Energy_tmp[i];

        //increment amount of events for specific x/y/z combination
        array_iterator[array_position]++;
    }

    //reset Data in store
    Store->Reset();
}

//---------------------------------------------------------------

void AIDA_Data_Streamer::load_conversion_array(){
    ifstream data("Configuration_Files/AIDA_Conversion_Array.txt");
    if(data.fail()){
        cerr << "Could not find AIDA_Conversion_Array.txt file" << endl;
        exit(0);
    }
    int xy,z,val;
    string line;
    while(data.good()){
        getline(data,line,'\n');
        if(line[0] == '#') continue;
        sscanf(line.c_str(),"%d %d %d",&xy,&z,&val);
        conversion_array[xy][z] = val;
    }
}

//---------------------------------------------------------------

inline void AIDA_Data_Streamer::reset_iterators(){
    for(int i = 0;i < z_strip_amount*2;++i) array_iterator[i] = 0;
}

//---------------------------------------------------------------

inline void AIDA_Data_Streamer::check_array_position(int array_position,int z_strip){
    if(array_position == -1){
        cerr << "Unknown z strip number in AIDA " << z_strip << endl;
        cerr << "Exiting" << endl;
        exit(0);
    }
}

//---------------------------------------------------------------

double* AIDA_Data_Streamer::get_Energy(bool xy,int z_strip){
    return Energy[conversion_array[xy][z_strip]];
}

//---------------------------------------------------------------

ULong64_t* AIDA_Data_Streamer::get_Time(bool xy,int z_strip){
    return Time[conversion_array[xy][z_strip]];
}

//---------------------------------------------------------------

int* AIDA_Data_Streamer::get_Coordinate(bool xy,int z_strip){
    return x_coord[conversion_array[xy][z_strip]];
}

//---------------------------------------------------------------

int AIDA_Data_Streamer::get_amount_of_hits(bool xy,int z_strip){
    return array_iterator[conversion_array[xy][z_strip]];
}

//---------------------------------------------------------------

void AIDA_Data_Streamer::Store_and_Purge(bool x_or_y,int* x_coord,ULong64_t* Time,double* Energy,int z_strip,int amount){
    int array_position = 0;
    reset_iterators();
    for(int i = 0;i < amount;++i){
        //get xyz position from conversion array
        array_position = conversion_array[x_or_y][z_strip];
        //check if array position unknown
        check_array_position(array_position,z_strip);
        
        //set values
        this->x_coord[array_position][array_iterator[array_position]] = x_coord[i];
        this->Time[array_position][array_iterator[array_position]] = Time[i];
        this->Energy[array_position][array_iterator[array_position]] = Energy[i];

        //increment amount of events for specific x/y/z combination
        array_iterator[array_position]++;
    }
}

//---------------------------------------------------------------
