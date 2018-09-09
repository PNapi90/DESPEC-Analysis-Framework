#include "AIDA_Decay_Event_Store.h"

using namespace std;

//---------------------------------------------------------------

AIDA_Decay_Event_Store::AIDA_Decay_Event_Store(){
    amount = 0;
    
    Energy = new double[max_len];
    z_strip = new int[max_len];
    x_or_y = new bool[max_len];
    x_coord = new int[max_len];
    Time = new ULong64_t[max_len];

    for(int i = 0;i < max_len;++i){
        Energy[i] = 0;
        z_strip[i] = 0;
        x_or_y[i] = false;
        x_coord[i] = 0;
        Time[i] = 0;
    }
}

//---------------------------------------------------------------

AIDA_Decay_Event_Store::~AIDA_Decay_Event_Store(){
    delete[] Energy;
    delete[] z_strip;
    delete[] x_or_y;
    delete[] x_coord;
    delete[] Time;

    Energy = nullptr;
    z_strip = nullptr;
    x_or_y = nullptr;
    x_coord = nullptr;
    Time = nullptr;

    amount = 0;
}

//---------------------------------------------------------------

void AIDA_Decay_Event_Store::Store_Decay_Event(ADCDataItem* Decay_Item){
    Energy[amount] = Decay_Item->get_calibrated_energy();
    z_strip[amount] = Decay_Item->GetLayer();
    x_or_y[amount] = (Decay_Item->GetFront_Back() == 1);
    x_coord[amount] = Decay_Item->GetRealChannelID();
    Time[amount] = Decay_Item->GetTimestamp();

    cout << "AMOUNT " << amount << endl;
    Decay_Item->Print_Event();

    amount++;
}

//---------------------------------------------------------------

void AIDA_Decay_Event_Store::Reset(){
    for(int i = 0;i < amount;++i){
        Energy[i] = 0;
        z_strip[i] = 0;
        x_or_y[i] = false;
        x_coord[i] = 0;
        Time[i] = 0;
    }

    amount = 0;
}

//---------------------------------------------------------------

double* AIDA_Decay_Event_Store::get_Energy(){
    return Energy;
}

//---------------------------------------------------------------

bool* AIDA_Decay_Event_Store::get_x_or_y(){
    return x_or_y;
}

//---------------------------------------------------------------

ULong64_t* AIDA_Decay_Event_Store::get_Time(){
    return Time;
}

//---------------------------------------------------------------

int* AIDA_Decay_Event_Store::get_z_strip(){
    return z_strip;
}

//---------------------------------------------------------------

int* AIDA_Decay_Event_Store::get_x_coord(){
    return x_coord;
}

//---------------------------------------------------------------

int AIDA_Decay_Event_Store::get_amount_of_events(){
    return amount;
}

//---------------------------------------------------------------
