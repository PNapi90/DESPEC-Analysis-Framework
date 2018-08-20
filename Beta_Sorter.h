#ifndef BETA_SORTER_H
#define BETA_SORTER_H

#include "Strip_Event.h"
#include "Beta_Event.h"
#include "Beta_Cluster.h"


typedef unsigned long long ULong64_t;

class Beta_Sorter{

private:

public:
    Beta_Sorter();
    ~Beta_Sorter();

    void set_Event(int*,double,ULong64_t);
    
};

#endif