#ifndef STRIP_EVENT_H
#define STRIP_EVENT_H

typedef unsigned long long ULong64_t;

class Strip_Event{

private:

    ULong64_t Time;
    double Energy;

    int z_strip;
    bool type;


public:
    Strip_Event(bool,int,double,ULong64_t);
    ~Strip_Event();

    int get_z_strip();
    bool get_Type();
    double get_Energy();
    ULong64_t get_Time();
    
};

#endif