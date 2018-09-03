#ifndef TX_MATRIX_H
#define TX_MATRIX_H

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "T_Matrix_Row.h"
//#include "Data_Class_Obj.h"

typedef unsigned long long ULong64_t;

class TX_Matrix{

private:

    int amount_of_data_points,am_threads;
    int z_strip_number;

    bool x_or_y;


    double am_threads_d,amount_of_data_points_d;

    ULong64_t* Time_Arr;
    double* Energy_Arr;
    int* X_Arr;

    ULong64_t* Time_Arr_Save;
    double* Energy_Arr_Save;
    int* X_Arr_Save;

    int** relevant_for_x;
    int* len_line_X;

    ULong64_t Time_Last;
    ULong64_t** Thr_Time_Array;

    T_Matrix_Row** T_Rows;

    bool* skip_arr;

    void Thread_X(int);
    void Thread_T(int);
    
    std::thread threading(bool,int);

public:
    TX_Matrix(int);
    ~TX_Matrix();
    
    void Process(int*,ULong64_t*,double*,int,int);
};

#endif
