#ifndef PLASTIC_DATA_STREAM_H
#define PLASTIC_DATA_STREAM_H

#include "Data_Stream.cxx"

typedef unsigned long ULong;
typedef unsigned int UInt;

class PLASTIC_Data_Stream : public Data_Stream{

private:

    int amount;

    bool mismatch;

    ULong PLASTIC_Coarse_T[100];
    ULong PLASTIC_Fine_T[100];
    ULong PLASTIC_ch_ID[100];

    ULong PLASTIC_Coarse_T_Trigger;
    ULong PLASTIC_Fine_T_Trigger;

public:
    PLASTIC_Data_Stream();
    ~PLASTIC_Data_Stream();

    void set_event_data(ULong*,ULong*,UInt*,ULong,ULong);

    void set_amount_of_Events(int);

    void set_mismatch();
    bool get_mismatch();

    void reset();
    int get_amount_Hits();
    double get_E(int);
    

};

#endif