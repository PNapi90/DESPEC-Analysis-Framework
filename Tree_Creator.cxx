#include "Tree_Creator.h"

using namespace std;

//---------------------------------------------------------------

Tree_Creator::Tree_Creator(int* interestArray,int len){
    this->interestArray = std::vector<int>(len,0);
    this->len = len;

    typePos = std::vector<int>(len,0);

    string treename = "";
    filename = "Root_Trees/";

    for(int i = 0;i < len;++i){
        this->interestArray[i] = interestArray[i];
        treename += names[interestArray[i]];
    }

    filename += treename + ".root";
        
    
    File = new TFile(filename.c_str(),"RECREATE");
    Tree = new TTree(treename.c_str(),treename.c_str());
    
    CreateBranches();
}

//---------------------------------------------------------------

Tree_Creator::~Tree_Creator(){
    File->Write();
    File->Close();

    for(int i = 0;i < len;++i) if(Branches_Created[i]) delete Branches_Created[i];
    delete[] Branches_Created;
}

//---------------------------------------------------------------

void Tree_Creator::CreateBranches(){
    Branches_Created = new Branch_Creator*[len];
    for(int i = 0;i < len;++i) CreateBranch(interestArray[i],i);
}

//---------------------------------------------------------------

inline void Tree_Creator::CreateBranch(int type,int i){

    typePos[type] = i;

    switch(type){
        case 0:
            Branches_Created[i] = new FRS_Branch_Creator(Tree);
            break;
        case 1:
            Branches_Created[i] = new AIDA_Branch_Creator(Tree);
            break;
        case 2:
            Branches_Created[i] = new PLASTIC_Branch_Creator(Tree);
            break;
        case 3:
            Branches_Created[i] = new FATIMA_Branch_Creator(Tree);
            break;
        case 4:
            Branches_Created[i] = new GALILEO_Branch_Creator(Tree);
            break;
        case 5:
            Branches_Created[i] = new FINGER_Branch_Creator(Tree);
            break;
        default:
            cerr << "Unknown Branch_Creator type " << type << " in Tree_Creator" << endl;
            exit(1);
    }
}

//---------------------------------------------------------------

void Tree_Creator::SetFATIMA(FATIMA_Data* DATA){
    
    //get Branch position from type
    int tmpPos = typePos[3];

    //fill branch variables with data
    Branches_Created[tmpPos]->WriteData(DATA);

    //fill tree with current data
    Tree->Fill();
}

//---------------------------------------------------------------
