#ifndef T_MATRIX_ROW_H
#define T_MATRIX_ROW_H

typedef unsigned long long ULong64_t;

class T_Matrix_Row{

    const int max_len = 50000;
    const int bad_val = -11223344;

    int* Relevant_for_i;
    double ROW[max_len];
    
    int len_of_row,len_of_relevant;

public:
    T_Matrix_Row();
    ~T_Matrix_Row();
    
};

#endif