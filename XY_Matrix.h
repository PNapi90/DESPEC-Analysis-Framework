#ifndef XY_MATRIX_H
#define XY_MATRIX_H

#define GPP_FLAG ((__GNUC__ == 4 && __GNUC_MINOR__ >= 9) || __GNUC__ > 4)

#ifdef(GPP_FLAG)
    #include <mutex>
    #include <thread>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

#include "TX_Matrix.h"
#include "Raw_Event.h"

typedef unsigned long long ULong64_t;

class XY_Matrix{
private:

    const double max_T = 1000.;
    const double max_E = 1000.;
    
    int max_len;

    int local_z;
    int data_points_per_thr_x,data_points_per_thr_last_x;
    int data_points_per_thr_y,data_points_per_thr_last_y;
    
    int len_X,len_Y;
    int am_threads;
    int amount_of_deleted_events;
    int amount_of_events;

    std::vector<int> deleted_events;

    double am_threads_d;

    TX_Matrix* Cluster_X;
    TX_Matrix* Cluster_Y;

    ULong64_t* Cluster_X_Time;
    ULong64_t* Cluster_Y_Time;

    int** Cluster_X_len;
    int** Cluster_Y_len;

    double* Cluster_X_Energy;
    double* Cluster_Y_Energy;

    double** Cluster_X_Energies;
    double** Cluster_Y_Energies;

    #ifdef(GPP_FLAG)
        std::mutex MUTEX;
        std::thread threading(int);
    #endif

    inline int get_XY_Counter();
    
    void Thread_XY(int);
    void get_Arrays();
    void NULL_Arrays();
    void get_amount_points_per_thr();
    inline void Form_XY(double*,int,int);

public:
    XY_Matrix(int,int);
    ~XY_Matrix();
    
    void Process(TX_Matrix*,TX_Matrix*);
    void get_DATA(Raw_Event*);
};


#endif
