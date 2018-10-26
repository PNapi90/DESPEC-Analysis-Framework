#ifndef TREE_CREATOR_H
#define TREE_CREATOR_H 

#include <TTree.h>

class Tree_Creator{


public:
    
    virtual ~Tree_Creator(){};

    TTree* Get_Tree() = 0;
    
};

