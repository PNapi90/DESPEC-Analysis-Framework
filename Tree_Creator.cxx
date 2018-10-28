#include "Tree_Creator.h"

using namespace std;

//---------------------------------------------------------------

Tree_Creator::Tree_Creator(int* interestArray,int len){
    this->interestArray = std::vector<int>(len,0);
    this->len = len;
    for(int i = 0;i < len;++i) this->interestArray[i] = interestArray[i];

    CreateBranches();
}

//---------------------------------------------------------------

Tree_Creator::~Tree_Creator(){

}

//---------------------------------------------------------------

void Tree_Creator::CreateBranches(){
    Branches_Created = new Branches_Created*[len];
    for(int i = 0;i < len;++i) CreateBranch(interestArray[i],i);
}

//---------------------------------------------------------------

inline void Time_EventBuilder::CreateBranch(int type,int i){
    switch(type){
        case 0:
            Branches_Created[i] = new FRS_Branch_Creator(Trees[i]);
            break;
        case 1:
            Branches_Created[i] = new AIDA_Branch_Creator(Trees[i]);
            break;
        case 2:
            Branches_Created[i] = new PLASTIC_Branch_Creator(Trees[i]);
            break;
        case 3:
            Branches_Created[i] = new FATIMA_Branch_Creator(Trees[i]);
            break;
        case 4:
            Branches_Created[i] = new GALILEO_Branch_Creator(Trees[i]);
            break;
        case 5:
            Branches_Created[i] = new FINGER_Branch_Creator(Trees[i]);
            break;
        default:
            cerr << "Unknown Branch_Creator type " << type << " in Time_EventBuilder" << endl;
            exit(1);
    }
}

//---------------------------------------------------------------