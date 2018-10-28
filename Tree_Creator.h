#ifndef TREE_CREATOR_H
#define TREE_CREATOR_H

#include <vector>
#include <iostream>

class Tree_Creator{

private:

    int len;
    std::vector<int> interestArray;

    Branch_Creator** Branches_Created;

    void CreateBranches();
    inline void CreateBranc(int type,int i);




public:
    Tree_Creator(int* interestArray,int len);
    ~Tree_Creator();
    
};

#endif