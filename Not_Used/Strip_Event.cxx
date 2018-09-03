#include "Strip_Event.h"

using namespace std;

//---------------------------------------------------------------

Strip_Event::Strip_Event(bool type,int strip_id,int z_strip,double Energy,ULong64_t Time){
    
    //check if type is x or y axis (false -> x,true -> y)
    this->type = type;
    //set z_strip, energy and time of event
    this->z_strip = z_strip;
    this->Energy = Energy;
    this->Time = Time;
    this->strip_id = strip_id;
}

//---------------------------------------------------------------

Strip_Event::~Strip_Event(){}

//---------------------------------------------------------------

ULong64_t Strip_Event::get_Time(){
    return Time;
}

//---------------------------------------------------------------

bool Strip_Event::get_Type(){
    return type;
}

//---------------------------------------------------------------

int Strip_Event::get_z_strip(){
    return z_strip;
}

//---------------------------------------------------------------

double Strip_Event::get_Energy(){
    return Energy;
}

//---------------------------------------------------------------


int Strip_Event::get_strip(){
    return strip_id;
}

//---------------------------------------------------------------


bool Strip_Event::get_Energy_Time_Difference(ULong64_t Time_tmp,double Energy_tmp){
    
    double delta_t = (double) (Time_tmp - Time);
    double delta_E = Energy_tmp - Energy;

    bool cond_t = abs(delta_t - offset_t) < width_t;
    bool cond_E = abs(delta_E - offset_E) < width_E;

    return (cond_E && cond_t);
}

//---------------------------------------------------------------