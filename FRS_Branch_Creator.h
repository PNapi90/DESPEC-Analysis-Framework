#ifndef FRS_BRANCH_CREATOR_H
#define FRS_BRANCH_CREATOR_H

#include <TTree.h>

#include "Branch_Creator.cxx"

class FRS_Branch_Creator : public Branch_Creator{


public:
    FRS_Branch_Creator(TTree* Tree);
    ~FRS_Branch_Creator();
    
    void WriteData(FATIMA_Data*){};
};

#endif
