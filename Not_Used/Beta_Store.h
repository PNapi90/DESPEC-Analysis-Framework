#ifndef BETA_STORE_H
#define BETA_STORE_H

#include "Beta_Clusters.h"
#include "Raw_Event.h"

typedef unsigned long long ULong64_t;

class Beta_Store{
private:

    const int MAX_AMOUNT = 1000000;

    int stored_counter;

    ULong64_t* TIMES;
    double* ENERGIES;
    int* LENGTHS;

public:
    Beta_Store();
    ~Beta_Store();

    void SAVE(Beta_Clusters*);
    void GET(Raw_Event*);
};

#endif