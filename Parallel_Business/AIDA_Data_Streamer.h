#ifndef AIDA_DATA_STREAMER_H
#define AIDA_DATA_STREAMER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

typedef unsigned long long ULong64_t;

class AIDA_Data_Streamer{

private:

    const int max_len = 1000000;
    const int z_strip_amount = 3;

    int conversion_array[2][100];

    int* row_counter;

    bool** x_or_y;
    int** x_coord;
    ULong64_t** Time;
    double** Energy;

    void load_conversion_array();
    inline void reset_iterators();
    inline void check_array_position();


public:
    AIDA_Data_Streamer();
    ~AIDA_Data_Streamer();

    double* get_Energy(bool,int);
    ULong64_t* get_Time(bool,int);
    int* get_Coordinate(bool,int);

    int get_amount_of_hits(bool,int);
    
};

#endif