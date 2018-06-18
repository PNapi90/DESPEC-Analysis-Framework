#include "GALILEO_Data_Stream.h"

using namespace std;

//---------------------------------------------------------------

GALILEO_Data_Stream::GALILEO_Data_Stream(){}

//---------------------------------------------------------------

GALILEO_Data_Stream::~GALILEO_Data_Stream(){}

//---------------------------------------------------------------

void GALILEO_Data_Stream::set_data_stream(double** data_stream,int am_hits,ULong64_t WR_T){

    this->WR_T = WR_T;
    this->am_hits = am_hits;
    
    for(int i = 0;i < am_hits;++i){
        E[i] = data_stream[i][0];
        T[i] = data_stream[i][1];
        hitID[i] = (int) data_stream[i][2];
    }
}