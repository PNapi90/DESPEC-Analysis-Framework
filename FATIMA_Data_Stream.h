#ifndef FATIMA_DATA_STREAM_H
#define FATIMA_DATA_STREAM_H

#include "Data_Stream.cxx"

#include <iostream>
#include <string>

typedef unsigned long ULong;
typedef unsigned int UInt;

class FATIMA_Data_Stream : public Data_Stream{

private:

    int FATIMA_Det_ID[100];
    double FATIMA_E[100];

    bool mismatch;

    ULong FATIMA_T[100];
    ULong FATIMA_QDC_T[100];
    ULong FATIMA_QDC_FT[100];
    int FATIMA_hits_det[100];

    int fat_iter;
    int FATIMA_Event_Length;
    int ID_int;
    int amount_ID[100];


public:
    FATIMA_Data_Stream();
    ~FATIMA_Data_Stream();

    void set_event_data(double,ULong,ULong,int,int,int);
    
    double get_E(int);
    
    //inherited
    void reset();
    bool get_mismatch();
    void set_mismatch();
    int get_amount_Hits();    
};


#endif
