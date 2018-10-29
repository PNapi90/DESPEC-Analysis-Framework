#ifndef PLASTIC_BRANCH_CREATOR_H
#define PLASTIC_BRANCH_CREATOR_H

#include <TTree.h>

#include "Branch_Creator.cxx"

class PLASTIC_Branch_Creator : public Branch_Creator{

private:

    double dummyArr[50];
    double dummyVal;


public:
    PLASTIC_Branch_Creator(TTree* Tree);
    ~PLASTIC_Branch_Creator();
    
    void WriteData(FATIMA_Data*){};
};

#endif
