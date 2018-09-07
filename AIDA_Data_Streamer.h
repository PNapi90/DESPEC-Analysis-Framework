#ifndef AIDA_DATA_STREAMER_H
#define AIDA_DATA_STREAMER_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "AIDA_Decay_Event_Store.h"

typedef unsigned long long ULong64_t;

class AIDA_Data_Streamer{

private:

    const int max_len = 10000;
    const int z_strip_amount = 3;

    int conversion_array[2][100];
    
    int* array_iterator;

    int* row_counter;

    bool** x_or_y;
    int** x_coord;
    ULong64_t** Time;
    double** Energy;

    void load_conversion_array();
    inline int get_Coord(int,bool);
    inline void reset_iterators();
    inline void check_array_position(int,int);


public:
    AIDA_Data_Streamer();
    ~AIDA_Data_Streamer();

    double* get_Energy(bool,int);
    ULong64_t* get_Time(bool,int);
    int* get_Coordinate(bool,int);

    void set_DATA(AIDA_Decay_Event_Store*);

    int get_amount_of_hits(bool,int);
    
    void Store_and_Purge(bool,int*,ULong64_t*,double*,int,int);
};

#endif
