#include "T_Matrix_Row.h"

using namespace std;

//---------------------------------------------------------------

T_Matrix_Row::T_Matrix_Row(){
    len_of_row = 0;
    len_of_relevant = 0;
    Relevant_for_i = new int[max_len];
    for(int i = 0;i < max_len;++i){
        Relevant_for_i[i] = -1;
        ROW[i] = bad_val;
    }
    
}

//---------------------------------------------------------------

T_Matrix_Row::~T_Matrix_Row(){
    delete[] Relevant_for_i;
}

//---------------------------------------------------------------

void T_Matrix_Row::set_Row(ULong64_t* row,ULong64_t Time_i,int pos,int len_of_row){
    this->len_of_row = len_of_row;

    len_of_relevant = 0;

    for(int i = pos+1;i < len_of_row;++i){
        delta = (double) (row[i] - Time_i);
        ROW[i] = (abs(delta) < t_max) ? delta : bad_val;
        if(ROW[i] != bad_val){
            Relevant_for_i[len_of_relevant] = i;
            len_of_relevant++;
        }
    }
}

//---------------------------------------------------------------

int* T_Matrix_Row::get_Relevant_Evts(){
    return Relevant_for_i;
}

//---------------------------------------------------------------

int T_Matrix_Row::get_Relevant_amount(){
    return len_of_relevant;
}

//---------------------------------------------------------------