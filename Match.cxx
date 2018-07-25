#include "Match.h"

using namespace std;

//---------------------------------------------------------------


Match::Match(int Match_ID,int primary_type,int* primary_pos,int* interest_array,int len_interest,ULong64_t WR){
    this->Match_ID = Match_ID;
    Match_ID_Address = &(this->Match_ID);

    this->len_interest = len_interest;
    hit_types = new int[len_interest];
    for(int i = 0;i < len_interest;++i) hit_types[i] = interest_array[i];

    match_filled = 0;

    DATA = new int*[6];
    for(int i = 0;i < 6;++i) DATA[i] = nullptr;

    DATA[primary_type] = primary_pos;
    match_filled++;

    this->WR = WR;

}

//---------------------------------------------------------------

Match::~Match(){

    for(int i = 0;i < 6;++i) DATA[i] = nullptr;
    delete[] DATA;
    delete[] hit_types;
}

//---------------------------------------------------------------

void Match::set_Address(int Match_ID){
    cout << this->Match_ID <<" Set match " << Match_ID << endl;
    this->Match_ID = Match_ID;
}

//---------------------------------------------------------------

void Match::set_Data(int type,int* data_pos){
    DATA[type] = data_pos;
    match_filled++;
}

//---------------------------------------------------------------

int* Match::get_Address(){
    return Match_ID_Address;
}

//---------------------------------------------------------------

int** Match::get_Address_Array(){
    return DATA;
}

//---------------------------------------------------------------

int* Match::get_hit_types(){
    return hit_types;
}

//---------------------------------------------------------------

bool Match::Full(){
    if(match_filled > len_interest){
        cerr << "Match filled too much" << endl;
        exit(0);
    }
    return (match_filled == len_interest);
}

//---------------------------------------------------------------

void Match::Write(){
    //has to be implemented
    return;
}

//---------------------------------------------------------------

int Match::get_amount_Hits(){
    return match_filled;
}

//---------------------------------------------------------------

bool Match::Check_Time(ULong64_t WR_tmp){
    double threshold = 10;
    return (WR_tmp - WR) > threshold;
}

//---------------------------------------------------------------