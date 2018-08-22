#ifndef T_MATRIX_H
#define T_MATRIX_H

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "T_Matrix_Row.h"
#include "Data_Class_Obj.h"

typedef unsigned long long ULong64_t;

class T_Matrix{

private:

    int amount_of_data_points,am_threads;

    double am_threads_d,amount_of_data_points_d;

    ULong64_t* Time_Arr;
    ULong64_t** Thr_Time_Array;

    T_Matrix_Row** T_Rows;

    bool* skip_arr;

    void load_thread_file();

public:
    T_Matrix();
    ~T_Matrix();
    
    void set_data(Data_Class_Obj*);
};

#endif