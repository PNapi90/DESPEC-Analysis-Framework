#include "Events.h"

using namespace std;

//---------------------------------------------------------------

Events::Events(int* positions_tmp,int length){
    this->length = length;

    positions = new int[length];
    Permission_array = new bool[length];
    Match_IDs = new int*[length];

    for(int i = 0;i < length;++i){
        Match_IDs[i] = nullptr;
        Permission_array[i] = false;
        positions[i] = positions_tmp[i];
    }

    iterator = 0;
}

//---------------------------------------------------------------

Events::~Events(){
    set_Match_ID_address_NULL_ALL();

    delete[] positions;
    delete[] Match_IDs;
    delete[] Permission_array;
}

//---------------------------------------------------------------

void Events::set_Match_ID_address(int* match_id_address){
    Match_IDs[iterator] = match_id_address;
    iterator++;
}

//---------------------------------------------------------------

void Events::set_Match_ID_address_NULL_ALL(){
    for(int i = 0;i < length;++i) Match_IDs[i] = nullptr;
}

//---------------------------------------------------------------

int** Events::get_Match_ID_address(){
    return Match_IDs;
}

//---------------------------------------------------------------

bool Events::Permission(int pos){
    if(Permission_array[pos]){
        cerr << "!Multiple coincidence events of same kind in FATIMA!" << endl;
    }
    Permission_array[pos] = true;
    bool return_val = true;
    for(int i = 0;i < length;++i) return_val = return_val && Permission_array[i];

    return return_val;
}

//---------------------------------------------------------------

int Events::get_iterator(){
    return iterator;
}

//---------------------------------------------------------------

int Events::get_Match_ID(int j){
    int return_val = (*Match_IDs[j]);
    return return_val;
}

//---------------------------------------------------------------
