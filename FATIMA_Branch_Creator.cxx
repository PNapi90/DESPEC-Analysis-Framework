#include "FATIMA_Branch_Creator.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Branch_Creator::FATIMA_Branch_Creator(TTree* Tree){

    //create branches relevant for detector system and add it to Tree
    Tree->Branch("TestEnergy",&x,"x/F");
}

//---------------------------------------------------------------

FATIMA_Branch_Creator::~FATIMA_Branch_Creator(){

}

//---------------------------------------------------------------
