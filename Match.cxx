#include "Match.h"

using namespace std;

//---------------------------------------------------------------


Match::Match(int Match_ID,int primary_type,int* primary_pos,int* interest_array,int len_interest,ULong64_t WR){
    this->Match_ID = Match_ID;
    Match_ID_Address = &(this->Match_ID);

    this->len_interest = len_interest;
    hit_types = new int[len_interest];
    filled_types = new int[len_interest];
    for(int i = 0;i < len_interest;++i){
        hit_types[i] = interest_array[i];
        filled_types[i] = -1;
    }

    match_filled = 0;

    DATA = new int*[6];
    for(int i = 0;i < 6;++i) DATA[i] = nullptr;

    DATA[primary_type] = primary_pos;
    filled_types[match_filled] = primary_type;
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
    filled_types[match_filled] = type;
    match_filled++;
}

//---------------------------------------------------------------

int* Match::get_Address(){
    return Match_ID_Address;
}

//---------------------------------------------------------------

int** Match::get_Address_Array(){
    cout << "------- DATA --------" << endl;
    for(int i = 0;i < 6;++i) cout << DATA[i] << " ";
    cout << endl;
    return DATA;
}

//---------------------------------------------------------------

int* Match::get_hit_types(){
    return hit_types;
}

//---------------------------------------------------------------

int* Match::get_filled_types(){
    return filled_types;
}

//---------------------------------------------------------------

bool Match::Full(){
    if(match_filled > len_interest){
        cerr << "Match filled too much" << endl;
        exit(0);
    }
    cout << "Addresses" << endl;
    for(int i = 0;i < 6;++i) if(DATA[i]) cout << i << " " <<  DATA[i] << " " << *DATA[i] << endl;
    //cout << endl;
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

bool Match::Check_Time(ULong64_t WR_tmp,int pos){
    double threshold = 10;
    cout << "TD" << endl;
    cout << WR_tmp << " " << WR << " " << WR_tmp - WR << " " << DATA[pos] << " " << pos << endl;
    return ((WR_tmp - WR) > threshold);
}

//---------------------------------------------------------------