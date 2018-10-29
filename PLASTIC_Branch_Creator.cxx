#include "PLASTIC_Branch_Creator.h"

using namespace std;

//---------------------------------------------------------------

PLASTIC_Branch_Creator::PLASTIC_Branch_Creator(TTree* Tree){

    //create branches relevant for detector system and add it to Tree
    //Remark: Branches of Trees need address of variable
    // -> addresses remain the same until object is deleted
    Tree->Branch("dummyArr",&dummyArr,"dummyArr[50]/F");
    Tree->Branch("dummyVal",&dummyVal,"dummyVal/F");

}

//---------------------------------------------------------------

PLASTIC_Branch_Creator::~PLASTIC_Branch_Creator(){

}

//---------------------------------------------------------------
