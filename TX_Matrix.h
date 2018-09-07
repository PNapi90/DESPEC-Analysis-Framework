#ifndef TX_MATRIX_H
#define TX_MATRIX_H

#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <mutex>
#include <cstdlib>
#include <unistd.h>



#include "T_Matrix_Row.h"
#include "AIDA_Data_Streamer.h"
//#include "Data_Class_Obj.h"

typedef unsigned long long ULong64_t;

class TX_Matrix{

private:
    const int max_len = 50000;
    const double Time_tolerance = 1000;

    int amount_of_data_points,am_threads;
    int z_strip_number,save_iter;
    int data_points_per_thr,data_points_per_thr_last;
    
    std::mutex MUTEX;
    int iterator_mutex;

    bool x_or_y;



    double am_threads_d,amount_of_data_points_d;

    ULong64_t* Time_Arr;
    ULong64_t* Time_sent;
    double* Energy_Arr;
    int* X_Arr;
    
    int** Cluster_IDs;

    ULong64_t* Time_Arr_Save;
    double* Energy_Arr_Save;
    int* X_Arr_Save;
    int* cluster_counter;

    int** relevant_for_x;
    int* len_line_X;

    ULong64_t Time_Last;
    ULong64_t** Thr_Time_Array;

    T_Matrix_Row** T_Rows;

    bool* skip_arr;
    
    void set_Time();
    void reset_Saved();
    void Thread_X(int);
    void Thread_T(int);
    void Save_Matrix_Row(int);
    
    void process_mem_usage(int);

    inline bool keep_Event(int);
    
    std::thread threading(bool,int);

public:
    TX_Matrix(int,int);
    ~TX_Matrix();
    
    void Process(int*,ULong64_t*,double*,int,int);
    void set_Saved_To_Streamer(AIDA_Data_Streamer*);
    
    int get_len();
    ULong64_t* get_Time();
};

#endif
