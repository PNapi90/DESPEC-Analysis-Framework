#ifndef BETA_EVENT_H
#define BETA_EVENT_H

typedef unsigned long long ULong64_t;

class Beta_Event{
private:

    int* xy;
    
    ULong64_t Time;
    double Energy;

    int Cluster_ID;

public:
    Beta_Event();
    ~Beta_Event();

    ULong64_t get_Time();
    int* get_xy();

    void set_Cluster_ID(int);
    
};

#endif