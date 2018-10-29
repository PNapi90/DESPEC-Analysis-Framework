#include "FATIMA_Branch_Creator.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Branch_Creator::FATIMA_Branch_Creator(TTree* Tree){

    //create branches relevant for detector system and add it to Tree

    CLEAR();

    Tree->Branch("AmountHits",&AmountHits,"AmountHits/F");
    Tree->Branch("Energies",&Energies,"AmountHits[50]/F");
}

//---------------------------------------------------------------

FATIMA_Branch_Creator::~FATIMA_Branch_Creator(){

}

//---------------------------------------------------------------

void FATIMA_Branch_Creator::WriteData(FATIMA_Data* DATA){
    
    CLEAR();

    AmountHits = DATA->amountHits;
    for(int i = 0;i < AmountHits;++i) Energies[i] = DATA->Energy[i];
}

//---------------------------------------------------------------

void FATIMA_Branch_Creator::CLEAR(){
    AmountHits = 0;
    for(int i = 0;i < 50;++i) Energies[i] = 0;
}

//---------------------------------------------------------------