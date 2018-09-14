#ifndef FINGER_DATA_STREAM_H
#define FINGER_DATA_STREAM_H

#include "Data_Stream.cxx"

typedef unsigned long ULong;
typedef unsigned int UInt;

class FINGER_Data_Stream : public Data_Stream{

private:

    int amount;

    bool mismatch;

    ULong FINGER_Coarse_T[100];
    ULong FINGER_Fine_T[100];
    ULong FINGER_ch_ID[100];

    ULong FINGER_Coarse_T_Trigger;
    ULong FINGER_Fine_T_Trigger;

public:
    FINGER_Data_Stream();
    ~FINGER_Data_Stream();

    void set_event_data(ULong*,ULong*,UInt*,ULong,ULong);

    void set_amount_of_Events(int);

    void set_mismatch();
    bool get_mismatch();

    void reset();
    int get_amount_Hits();
    double get_E(int);
    

};

#endif
