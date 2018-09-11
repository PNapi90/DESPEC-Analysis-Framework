#include "T_Matrix_Row.h"

using namespace std;

//---------------------------------------------------------------

T_Matrix_Row::T_Matrix_Row(){
    len_of_row = 0;
    len_of_relevant = 0;
    len_old = 0;
    
    try{
        Relevant_for_i = new int[max_len];
    }
    catch(const std::bad_alloc&){
        cerr << "std::bad_alloc occured!" << endl;
        cerr << "Problem occured in Relevant_for_i" << endl;
        exit(0);
    }
    for(int i = 0;i < max_len;++i) Relevant_for_i[i] = -1;
}

//---------------------------------------------------------------

T_Matrix_Row::~T_Matrix_Row(){
    delete[] Relevant_for_i;
}

//---------------------------------------------------------------

void T_Matrix_Row::set_Row(ULong64_t* row,ULong64_t Time_i,int pos,int len_of_row){
    this->len_of_row = len_of_row;
    
    check_limits();

    len_of_relevant = 0;
    double delta = 0;
    bool ROW = false;

    for(int i = pos+1;i < len_of_row;++i){
        delta = (row[i] >= Time_i) ? (double) (row[i] - Time_i) : (double) (Time_i - row[i]);
        ROW = (abs(delta) < t_max);
        if(ROW){
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

void T_Matrix_Row::Reset(){
    len_of_relevant = 0;
}

//---------------------------------------------------------------

inline void T_Matrix_Row::check_limits(){
    if(len_of_row > max_len){
        cerr << "ARRAY LENGTH LIMIT OF T_Matrix_Row REACHED!" << endl;
        exit(0);
    }
}

//---------------------------------------------------------------
