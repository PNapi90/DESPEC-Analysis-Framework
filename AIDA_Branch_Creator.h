#ifndef AIDA_BRANCH_CREATOR_H
#define AIDA_BRANCH_CREATOR_H

#include <TTree.h>

#include "Branch_Creator.cxx"

class AIDA_Branch_Creator : public Branch_Creator{


public:
    AIDA_Branch_Creator(TTree* Tree);
    ~AIDA_Branch_Creator();
    
    void WriteData(FATIMA_Data*){};
};

#endif
