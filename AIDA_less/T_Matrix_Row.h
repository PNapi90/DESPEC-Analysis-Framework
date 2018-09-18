#ifndef T_MATRIX_ROW_H
#define T_MATRIX_ROW_H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <fstream>

typedef unsigned long long ULong64_t;

class T_Matrix_Row{

    const double t_max = 10000;
    
    int max_len;
    const int bad_val = -11223344;

    int* Relevant_for_i;

    int len_old,max_len_TX;
    
    int len_of_row,len_of_relevant;

    void process_mem_usage(int);
    inline void check_limits();

public:
    T_Matrix_Row(int,int);
    ~T_Matrix_Row();
    
    int* get_Relevant_Evts();
    int get_Relevant_amount();
    
    void set_Row(ULong64_t*,ULong64_t,int,int);
    void Reset();
};

#endif
