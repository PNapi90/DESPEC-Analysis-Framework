#ifndef MATCH_H
#define MATCH_H

#include <iostream>
#include <cstdlib>


typedef unsigned long long ULong64_t;

class Match{

private:

    ULong64_t WR;

    int Match_ID,primary_type;
    int* Match_ID_Address;

    int match_filled;

    int len_interest;
    int* hit_types;
    int* filled_types;

    int** DATA;

public:
    Match(int,int,int*,int*,int,ULong64_t);
    ~Match();
    
    void set_Address(int);
    void set_Data(int,int*);
    void Write();

    bool Full();
    bool Check_Time(ULong64_t,int);
    int get_amount_Hits();

    int* get_hit_types();
    int* get_filled_types();
    int* get_Address();
    int** get_Address_Array();

    ULong64_t get_WR();

};


#endif