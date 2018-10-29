#ifndef FINGER_BRANCH_CREATOR_H
#define FINGER_BRANCH_CREATOR_H

#include <TTree.h>

#include "Branch_Creator.cxx"


class FINGER_Branch_Creator : public Branch_Creator{


public:
    FINGER_Branch_Creator(TTree* Tree);
    ~FINGER_Branch_Creator();
    
    void WriteData(FATIMA_Data*){};
};

#endif
