#ifndef T_MATRIX_ROW_H
#define T_MATRIX_ROW_H

#include <cmath>

typedef unsigned long long ULong64_t;

class T_Matrix_Row{

    const double t_max = 10000;
    
    const int max_len = 50000;
    const int bad_val = -11223344;

    int* Relevant_for_i;
    double* ROW;
    
    int len_of_row,len_of_relevant;

public:
    T_Matrix_Row();
    ~T_Matrix_Row();
    
    int* get_Relevant_Evts();
    int get_Relevant_amount();
    
    void set_Row(ULong64_t*,ULong64_t,int,int);
    
};

#endif
