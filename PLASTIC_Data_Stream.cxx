#include "PLASTIC_Data_Stream.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Data_Stream::PLASTIC_Data_Stream(){}

//---------------------------------------------------------------

PLASTIC_Data_Stream::~PLASTIC_Data_Stream(){}

//---------------------------------------------------------------

void PLASTIC_Data_Stream::set_data_stream(double** data_stream,ULong64_t WR_T){

    this->WR_T = WR_T;
    am_hits = (int) data_stream[0][3];
    
    for(int i = 0;i < am_hits;++i){
        E[i] = data_stream[i][0];
        T[i] = data_stream[i][1];
        hitID[i] = (int) data_stream[i][2];
    }
}