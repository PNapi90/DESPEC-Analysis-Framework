#ifndef GALILEO_BRANCH_CREATOR_H
#define GALILEO_BRANCH_CREATOR_H

#include <TTree.h>


class GALILEO_Branch_Creator : public Branch_Creator{


public:
    GALILEO_Branch_Creator(TTree* Tree);
    ~GALILEO_Branch_Creator();
};

#endif