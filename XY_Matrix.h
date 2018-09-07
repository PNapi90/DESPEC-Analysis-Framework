#ifndef XY_MATRIX_H
#define XY_MATRIX_H

#include <mutex>
#include <thread>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "TX_Matrix.h"
#include "Raw_Event.h"

typedef unsigned long long ULong64_t;

class XY_Matrix{
private:

    int local_z;
    int data_points_per_thr_x,data_points_per_thr_last_x;
    int data_points_per_thr_y,data_points_per_thr_last_y;
    
    int len_X,len_Y;
    int am_threads;
    int amount_of_deleted_events;
    int amount_of_events;

    TX_Matrix* Cluster_X;
    TX_Matrix* Cluster_Y;

    std::mutex MUTEX;

    std::thread threading(int);

    inline int get_XY_Counter();
    
    void Thread_XY(int);
    inline void Form_XY(double*,int,int);

public:
    XY_Matrix(int);
    ~XY_Matrix();
    
    void Process(TX_Matrix*,TX_Matrix*);
    void get_DATA(Raw_Event*);
};


#endif