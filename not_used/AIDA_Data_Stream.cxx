#include "AIDA_Data_Stream.h"

using namespace std;

//---------------------------------------------------------------

AIDA_Data_Stream::AIDA_Data_Stream(){}

//---------------------------------------------------------------

AIDA_Data_Stream::~AIDA_Data_Stream(){}

//---------------------------------------------------------------

void AIDA_Data_Stream::set_data_stream(double** data_stream,ULong64_t WR_T){

    am_hits = (int) data_stream[0][5];
    
    for(int i = 0;i < am_hits;++i){
        
        E[i] = data_stream[i][0];
        T[i] = data_stream[i][1];

        hit_grid_ID[i] = (int) data_stream[i][2];
        strip_x_ID[i] = (int) data_stream[i][3];
        strip_y_ID[i] = (int) data_stream[i][4];
    }
}