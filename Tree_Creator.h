#ifndef TREE_CREATOR_H
#define TREE_CREATOR_H

#include <vector>
#include <iostream>
#include <string>

//#include "FRS_Data_Struct.h"
//#include "AIDA_Data_Struct.h"
//#include "PLASTIC_Data_Struct.h"
#include "FATIMA_Data_Struct.h"
//#include "GALILEO_Data_Struct.h"
//#include "FINGER_Data_Struct.h"


#include <TTree.h>
#include <TFile.h>

class Tree_Creator{

private:

    const std::string names[6] = {"Frs","Aida","Plastic","Fatima","Galileo","Finger"};

    int len;
    std::vector<int> interestArray;

    TTree* Tree;
    Branch_Creator** Branches_Created;

    void CreateBranches();
    inline void CreateBranch(int type,int i);




public:
    Tree_Creator(int* interestArray,int len);
    ~Tree_Creator();
    
    void setFATIMA(FATIMA_Data* DATA);
};

#endif