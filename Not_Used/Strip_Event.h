#ifndef STRIP_EVENT_H
#define STRIP_EVENT_H

typedef unsigned long long ULong64_t;

class Strip_Event{

private:

    const double width_t = 1;
    const double width_E = 1;
    const double offset_t = 0;
    const double offset_E = 0;

    ULong64_t Time;
    double Energy;

    int z_strip,strip_id;
    bool type;


public:
    Strip_Event(bool,int,double,ULong64_t);
    ~Strip_Event();

    int get_z_strip();
    int get_strip();
    bool get_Type();
    double get_Energy();
    ULong64_t get_Time();
    
    bool get_Energy_Time_Difference(ULong64_t,double);
};

#endif