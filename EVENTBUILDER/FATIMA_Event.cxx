#include "FATIMA_Event.h"

using namespace std;

//---------------------------------------------------------------

FATIMA_Event::FATIMA_Event(int max_uses,Raw_Event* RAW){
    this->max_uses = max_uses;
    Permission_array = new bool[max_uses];
    Match_IDs = new int*[max_uses];

    for(int i = 0;i < max_uses;++i){
        Match_IDs[i] = NULL;
        Permission_array[i] = false;
    }

    set_DATA(RAW);
}

//---------------------------------------------------------------

FATIMA_Event::~FATIMA_Event(){
    set_Match_ID_address_NULL_ALL();

    delete[] Match_IDs;
    delete[] Permission_array;
}

//---------------------------------------------------------------

void FATIMA_Event::set_Match_ID_address(int* match_id_address){
    Match_IDs[iterator] = match_id_address;
    iterator++;
}

//---------------------------------------------------------------

void FATIMA_Event::set_Match_ID_address_NULL_ALL(){
    for(int i = 0;i < max_uses;++i) Match_IDs[i] = NULL;
}

//---------------------------------------------------------------

bool FATIMA_Event::Permission(int pos){
    Permission_array[pos] = true;
    bool return_val = true;
    for(int i = 0;i < max_uses;++i) return_val = return_val && Permission_array[i];

    return return_val;
}

//---------------------------------------------------------------

inline void FATIMA_Event::set_DATA(Raw_Event* RAW){
    //something
    return;
}

//---------------------------------------------------------------