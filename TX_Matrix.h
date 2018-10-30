#ifndef TX_MATRIX_H
#define TX_MATRIX_H

#define GPP_FLAG ((__GNUC__ == 4 && __GNUC_MINOR__ >= 9) || __GNUC__ > 4)

#ifdef GPP_FLAG
    #include <thread>
    #include <mutex>
#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iomanip>
#include <vector>


#include "T_Matrix_Row.h"
#include "AIDA_Data_Streamer.h"


typedef unsigned long long ULong64_t;

class TX_Matrix{

private:
    int max_len;
    
    const double Time_tolerance = 1000;

    int amount_of_data_points,am_threads;
    int z_strip_number,save_iter;
    int data_points_per_thr,data_points_per_thr_last;
    
    
    int iterator_mutex;
    int strip_iterator;

    bool x_or_y;

    double am_threads_d,amount_of_data_points_d;

    ULong64_t* Time_Arr;
    double* Energy_Arr;
    int* X_Arr;

    ULong64_t* Time_sent;
    double* Energy_sent;
    double** Energies_sent;
    
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
    
    void set_Time_and_Energy();
    void reset_Saved();
    void Thread_X(int);
    void Thread_T(int);
    void Thread_T(int,int);
    void print_COINC_MAT();
    void Set_Next_Cluster(int**,int,int);
    void set_data_points_per_thread();

    void process_mem_usage(int,int);

    bool sortFunc(const std::vector<int>&,const std::vector<int>&);
    inline bool keep_Event(int);

    inline bool check_relevant(int);
    inline void set_relevant(int,int,int*);
    inline void Save_Matrix_Row(int);
    inline void set_skip_array_element(int,int,int);

    #ifdef GPP_FLAG
        std::mutex MUTEX;
        std::thread threading(bool,int,int);
    #endif

public:
    TX_Matrix(int,int,int*);
    ~TX_Matrix();
    
    void Process(int*,ULong64_t*,double*,int,int);
    void set_Saved_To_Streamer(AIDA_Data_Streamer*);
    
    int get_len();
    ULong64_t* get_Time();
    int** get_len_array();
    double* get_Energy();
    double** get_Energies();
};

#endif
