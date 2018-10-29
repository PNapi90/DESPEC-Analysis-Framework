#ifndef FATIMA_BRANCH_CREATOR_H
#define FATIMA_BRANCH_CREATOR_H

#include <TTree.h>

#include "Branch_Creator.cxx"


class FATIMA_Branch_Creator : public Branch_Creator{
private:

    int AmountHits;
    double Energies[50];

    void CLEAR();

public:
    FATIMA_Branch_Creator(TTree* Tree);
    ~FATIMA_Branch_Creator();

    void WriteData(FATIMA_Data* DATA);
};

#endif